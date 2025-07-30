use std::{
    collections::HashSet,
    error::Error,
    fs::{self, create_dir_all},
    path::{Path, PathBuf},
    sync::{
        atomic::{AtomicUsize, Ordering},
        Arc, Mutex,
    },
    time::{Duration,Instant}
};

use indicatif::{MultiProgress, ProgressBar, ProgressStyle};
use reqwest::{
    blocking::{Body, Client, Response}, header, redirect::Policy, ResponseBuilderExt, Version
};
use select::{
    document::Document,
    predicate::{Attr, Name},
};
use threadpool::ThreadPool;
use url::{Position, Url};
use std::io::Read;

// 配置常量
const BASE_URL: &str = "https://www.elastic.co/guide/en/elasticsearch/reference/7.17/";
const OUTPUT_DIR: &str = "elasticsearch-reference-7.17";
const CONCURRENT_DOWNLOADS: usize = 10;
const USER_AGENT: &str = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36";
const MAX_RETRIES: usize = 5;
const RETRY_DELAY: u64 = 30; // seconds

fn main() -> Result<(), Box<dyn Error>> {
    // 初始化目录
    create_dir_all(OUTPUT_DIR)?;

    // 创建线程池
    let pool = ThreadPool::new(CONCURRENT_DOWNLOADS);

    // 初始化进度条
    let m = MultiProgress::new();
    let main_pb = m.add(ProgressBar::new_spinner());
    main_pb.set_style(
        ProgressStyle::default_spinner()
            .template("{spinner} {msg}")?,
    );
    main_pb.set_message("Initializing downloader...");

    // 共享状态
    let visited_urls = Arc::new(Mutex::new(HashSet::new()));
    let total_files = Arc::new(AtomicUsize::new(0));
    let downloaded_files = Arc::new(AtomicUsize::new(0));

    // 创建HTTP客户端
    let client = Client::builder()
        .user_agent(USER_AGENT)
        .redirect(Policy::limited(5))
        // .cookie_store(true)
        .timeout(Duration::from_secs(120))
        .https_only(true) // 强制使用HTTPS
        .http2_prior_knowledge() // 强制尝试HTTP/2
        .http2_max_frame_size(10240000)
        .no_gzip() // 禁用gzip压缩，避免大帧
        .tls_built_in_root_certs(true) // 使用系统根证书
        // .use_rustls_tls() // 使用rustls替代默认的native-tls
        .pool_max_idle_per_host(0) // 禁用连接池，避免重用坏连接
        .tcp_keepalive(Duration::from_secs(30)) // 启用TCP keepalive
        .build()?;

    // 下载索引页面
    main_pb.set_message("Fetching index page...");
    let index_url = format!("{}index.html", BASE_URL);
    let body = fetch_url(&client, &index_url)?;
    let body_content_len = body.content_length().unwrap_or(0);
    println!("body_content_len {}",body_content_len);
    if body_content_len < 10 {
        println!("body_content_len less than 10, can not parse!");
        return Ok(());
    }
    // 解析索引页面
    main_pb.set_message("Parsing index page...");
    let document = Document::from_read(body)?;
    let links = collect_links(&document, BASE_URL);

    total_files.store(links.len(), Ordering::SeqCst);

    // 创建下载进度条
    let download_pb = m.add(ProgressBar::new(links.len() as u64));
    download_pb.set_style(
        ProgressStyle::default_bar()
            .template("{msg}\n{spinner} [{elapsed_precise}] [{wide_bar}] {pos}/{len} ({eta})")?
            .progress_chars("#>-"),
    );
    download_pb.set_message("Downloading pages...");

    // 添加初始URL到已访问集合
    visited_urls.lock().unwrap().insert(index_url.clone());

    // 下载所有页面
    for link in links {
        let client = client.clone();
        let visited_urls = visited_urls.clone();
        let downloaded_files = downloaded_files.clone();
        let download_pb = download_pb.clone();

        pool.execute(move || {
            if let Err(e) = download_and_process_page(
                &client,
                &link,
                OUTPUT_DIR,
                visited_urls,
                downloaded_files,
                download_pb,
            ) {
                eprintln!("Error downloading {}: {}", link, e);
            }
        });
    }

    // 等待所有任务完成
    main_pb.set_message("Waiting for downloads to complete...");
    pool.join();

    download_pb.finish_with_message("Download completed!");
    main_pb.finish_with_message("All done!");

    println!(
        "Downloaded {} files to {}",
        total_files.load(Ordering::SeqCst),
        OUTPUT_DIR
    );

    // 创建本地访问的index.html
    create_local_index(OUTPUT_DIR)?;

    Ok(())
}


fn download_and_process_page(
    client: &Client,
    url: &str,
    output_dir: &str,
    visited_urls: Arc<Mutex<HashSet<String>>>,
    downloaded_files: Arc<AtomicUsize>,
    pb: ProgressBar,
) -> Result<(), Box<dyn Error>> {
    // 检查是否已访问
    {
        let mut visited = visited_urls.lock().unwrap();
        if visited.contains(url) {
            return Ok(());
        }
        visited.insert(url.to_string());
    }

    // 下载页面
    let response = fetch_url(client, url)?;
    let content_type = response
        .headers()
        .get(header::CONTENT_TYPE)
        .and_then(|v| v.to_str().ok())
        .unwrap_or("")
        .to_string();

    // 获取相对路径
    let relative_path = get_relative_path(url, BASE_URL)?;
    let output_path = Path::new(output_dir).join(&relative_path);

    // 创建目录
    if let Some(parent) = output_path.parent() {
        create_dir_all(parent)?;
    }

    // 处理不同内容类型
    if content_type.contains("text/html") {
	 // 读取响应内容为字符串
        let  html_content = response.text()? ; 
        // 从字符串创建Document
        let document = Document::from(html_content.as_str());
        // 保存原始HTML文件
        fs::write(&output_path, &html_content)?;
        
        // 处理资源并修改链接
        process_html_resources(client, &document, &html_content,url, output_dir, &output_path)?;
    } else {
        // 保存二进制文件（图片、CSS、JS等）
        fs::write(&output_path, response.bytes()?)?;
    }

    // 更新进度
    let downloaded = downloaded_files.fetch_add(1, Ordering::SeqCst);
    pb.set_position((downloaded + 1) as u64);

    Ok(())
}

fn process_html_resources(
    client: &Client,
    document: &Document,
    orginal_html_content: &str,
    base_url: &str,
    output_dir: &str,
    output_path: &Path,
) -> Result<(), Box<dyn Error>> {
    let base_url = Url::parse(base_url)?;
    let mut html_content = orginal_html_content.to_string();

    // 处理图片
    for node in document.find(Name("img")) {
        if let Some(src) = node.attr("src") {
            if let Ok(relative_path) = download_resource(client, &base_url, output_dir, src) {
                html_content = html_content.replace(
                    &format!("src=\"{}\"", src),
                    &format!("src=\"{}\"", relative_path),
                );
            }
        }
    }

    // 处理CSS
    for node in document.find(Name("link")) {
        if let (Some(rel), Some(href)) = (node.attr("rel"), node.attr("href")) {
            if rel == "stylesheet" {
                if let Ok(relative_path) = download_resource(client, &base_url, output_dir, href) {
                    html_content = html_content.replace(
                        &format!("href=\"{}\"", href),
                        &format!("href=\"{}\"", relative_path),
                    );
                }
            }
        }
    }

    // 处理JS
    for node in document.find(Name("script")) {
        if let Some(src) = node.attr("src") {
            if let Ok(relative_path) = download_resource(client, &base_url, output_dir, src) {
                html_content = html_content.replace(
                    &format!("src=\"{}\"", src),
                    &format!("src=\"{}\"", relative_path),
                );
            }
        }
    }

    // 处理a标签的href
    for node in document.find(Name("a")) {
        if let Some(href) = node.attr("href") {
            if href.starts_with("http") || href.starts_with("#") {
                continue;
            }
            if let Ok(relative_path) = download_resource(client, &base_url, output_dir, href) {
                html_content = html_content.replace(
                    &format!("href=\"{}\"", href),
                    &format!("href=\"{}\"", relative_path),
                );
            }
        }
    }

    // 更新HTML文件
    fs::write(output_path, html_content)?;

    Ok(())
}

fn download_resource(
    client: &Client,
    base_url: &Url,
    output_dir: &str,
    resource_path: &str,
) -> Result<String, Box<dyn Error>> {
    // 构建完整URL
    let resource_url = base_url.join(resource_path)?.to_string();

    // 获取相对路径
    let relative_path = get_relative_path(&resource_url, BASE_URL)?;
    let output_path = Path::new(output_dir).join(&relative_path);

    // 如果文件已存在，跳过下载
    if output_path.exists() {
        return Ok(relative_path);
    }

    // 下载资源
    let response = client.get(&resource_url).send()?;
    if !response.status().is_success() {
        return Err(format!("Failed to download resource {}: {}", resource_url, response.status()).into());
    }

    // 创建目录
    if let Some(parent) = output_path.parent() {
        create_dir_all(parent)?;
    }

    // 保存文件
    fs::write(&output_path, response.bytes()?)?;

    Ok(relative_path)
}

fn fetch_url(client: &Client, url: &str) -> Result<Response, Box<dyn Error>> {
    return fetch_with_retry2(client, url);
    // let response = client.get(url).timeout(Duration::from_secs(120)).send()?;
    // if !response.status().is_success() {
    //     let err  = format!("Failed to fetch {}: {}", url, response.status());
    //     println!("{}",&err);
    //     return Err(err.into());
    // }
    // Ok(response)
}

fn fetch_with_retry(client: &Client, url: &str) -> Result<Response, Box<dyn Error>> {
    // 第一次请求获取可能的cookies
    let first_resp = client.get(url)
        .header(header::ACCEPT, "*/*")
        .header(header::ACCEPT_LANGUAGE, "en-US,en;q=0.9")
        .header(header::HOST, "www.elastic.co")
        .version(Version::HTTP_2) // 明确指定HTTP/2
        .send()?;
    
    // 提取cookies
    let cookies: Vec<String> = first_resp.headers()
        .get_all(header::SET_COOKIE)
        .iter()
        .filter_map(|h| h.to_str().ok())
        .map(|s| s.to_string())
        .collect();
    
    // 第二次请求带上cookies
    let mut request = client.get(url)
        .header(header::ACCEPT, "text/html")
        .header(header::ACCEPT_LANGUAGE, "en-US,en;q=0.9");
    
    for cookie in &cookies {
        if let Some(key_value) = cookie.split(';').next() {
            request = request.header(header::COOKIE, key_value);
        }
    }
    
    let response = request.send()?;
    
    // 检查最终URL
    let final_url = response.url().to_string();
    if !final_url.starts_with(BASE_URL) {
        return Err(format!("Unexpected redirect to: {}", final_url).into());
    }

    // let body = response.text()?;
    if  response.content_length().unwrap_or(0) < 1 {
        return Err(format!("Empty response body! url  {}",final_url).into());
    }

    Ok(response)
}

fn fetch_with_retry2(client: &Client, url: &str) -> Result<Response, Box<dyn Error>> {
    let mut last_error = None;

    for attempt in 0..MAX_RETRIES {
        match attempt_fetch(client, url) {
            Ok(content) => return Ok(content),
            Err(e) => {
                last_error = Some(e);
                if attempt < MAX_RETRIES - 1 {
                    println!("Attempt {} failed, retrying in {} seconds...", attempt + 1, RETRY_DELAY);
                    std::thread::sleep(Duration::from_secs(RETRY_DELAY));
                }
            }
        }
    }

    Err(last_error.unwrap_or_else(|| "All attempts failed".into()))
}

fn attempt_fetch(client: &Client, url: &str) -> Result<Response, Box<dyn Error>> {
    let start_time = Instant::now();
    let mut response = client.get(url)
        .header(header::ACCEPT, "text/html")
        .header(header::ACCEPT_LANGUAGE, "en-US,en;q=0.9")
        .send()?;

    println!("Request took {:?}", start_time.elapsed());

    // 检查响应状态
    if !response.status().is_success() {
        return Err(format!("HTTP error: {}", response.status()).into());
    }

    // 手动分块读取响应
    let mut body = Vec::new();
    let mut chunk = vec![0; 8192]; // 8KB chunks
    loop {
        let bytes_read = response.read(&mut chunk)?;
        if bytes_read == 0 {
            break;
        }
        body.extend_from_slice(&chunk[..bytes_read]);
    }

    // 验证内容完整性
    if body.is_empty() {
        return Err("Empty response body".into());
    }

    // String::from_utf8(body).map_err(|e| e.into())
    // 重建Response对象
    let reconstructed_response = rebuild_response(response, body)?;
    
    Ok(reconstructed_response)
}

fn rebuild_response(original: Response, body: Vec<u8>) -> Result<Response, Box<dyn std::error::Error>> {
    use http::response::Builder;

    let mut builder = Builder::new()
        .status(original.status())
        .url(original.url().clone())
        .version(original.version());
    // 复制headers
    for (key, value) in original.headers() {
        builder = builder.header(key, value);
    }
    
    // 构建新Response
    builder
    .body(body)
    .map(|r| Response::from(r))
    .map_err(|e| e.into())
}

fn collect_links(document: &Document, base_url: &str) -> Vec<String> {
    document
        .find(Attr("id", "reference-documents"))
        .next()
        .iter()
        .flat_map(|section| section.find(Name("a")))
        .filter_map(|a| a.attr("href"))
        .filter(|href| !href.starts_with("http") && !href.starts_with("#"))
        .map(|href| format!("{}{}", base_url, href))
        .collect()
}

fn get_relative_path(url: &str, base_url: &str) -> Result<String, Box<dyn Error>> {
    let base = Url::parse(base_url)?;
    let url = Url::parse(url)?;
    
    if url.domain() != base.domain() {
        return Err("URL domain doesn't match base domain".into());
    }
    
    let base_path = base.path();
    let url_path = url.path();
    
    if !url_path.starts_with(base_path) {
        return Err("URL path doesn't start with base path".into());
    }
    
    Ok(url_path[base_path.len()..].to_string())
}

fn create_local_index(output_dir: &str) -> Result<(), Box<dyn Error>> {
    let index_content = r#"<!DOCTYPE html>
<html>
<head>
    <title>Elasticsearch 7.17 Reference (Local Copy)</title>
    <style>
        body { font-family: Arial, sans-serif; line-height: 1.6; max-width: 800px; margin: 0 auto; padding: 20px; }
        h1 { color: #333; }
        ul { list-style-type: none; padding: 0; }
        li { margin-bottom: 10px; }
        a { color: #0066cc; text-decoration: none; }
        a:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <h1>Elasticsearch 7.17 Reference (Local Copy)</h1>
    <p>This is a local copy of the Elasticsearch 7.17 reference documentation.</p>
    <p>To browse the documentation, click the link below:</p>
    <ul>
        <li><a href="index.html">View Documentation</a></li>
    </ul>
</body>
</html>
"#;

    let index_path = Path::new(output_dir).join("_index.html");
    fs::write(index_path, index_content)?;

    Ok(())
}
