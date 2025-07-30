use hyper::{body::HttpBody, client::HttpConnector, Body, Client, Request};
use hyper_tls::HttpsConnector;
use std::error::Error;
use tokio::{io::AsyncWriteExt, time};

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let url = "https://www.elastic.co/guide/en/elasticsearch/reference/7.17/index.html";
    
    // 1. 创建带重试机制的客户端
    let client = create_http2_client()?;

    // 2. 带指数退避的重试逻辑
    let mut retries = 0;
    let max_retries = 4;
    let mut backoff = 4;
    
    loop {
        match try_fetch(&client, url).await {
            Ok(body) => {
                tokio::fs::write("es_7.17_index.html", &body).await?;
                println!("Successfully downloaded {} bytes", body.len());
                return Ok(());
            }
            Err(e) if retries >= max_retries => {
                return Err(format!("After {} retries, final error: {}", max_retries, e).into());
            }
            Err(e) => {
                println!("Attempt {} failed: {} (retrying in {}s)", retries + 1, e, backoff);
                retries += 1;
                time::sleep(time::Duration::from_secs(backoff)).await;
                backoff *= 2; // 指数退避
            }
        }
    }
}

fn create_http2_client() -> Result<Client<HttpsConnector<HttpConnector>>, Box<dyn Error>> {
    let https = HttpsConnector::new();
    Ok(Client::builder()
        .http2_only(true)
        // 关键配置：确保完全消费响应体
        .http2_keep_alive_interval(time::Duration::from_secs(30))
        .http2_keep_alive_timeout(time::Duration::from_secs(90))
        .http2_max_concurrent_reset_streams(30)
        .http2_max_frame_size(1<<20)
        // 调整窗口大小
        .http2_initial_stream_window_size(1024 * 1024 * 10) // 10MB
        .http2_initial_connection_window_size(1024 * 1024 * 15) // 15MB
        .build(https))
}

async fn try_fetch(
    client: &Client<HttpsConnector<HttpConnector>>,
    url: &str,
) -> Result<Vec<u8>, Box<dyn Error>> {
    let req = Request::builder()
        .uri(url)
        .header("user-agent", "Mozilla/5.0")
        .header("accept", "text/html")
        .body(Body::empty())?;

    let mut response = client.request(req).await?;
    
    // 确保完全读取响应体
    let mut body = Vec::new();
    while let Some(chunk) = response.body_mut().data().await {
        let chunk = chunk?;
        body.extend_from_slice(&chunk);
        // 模拟curl的完全消费行为
        if body.len() > 10_000_000 {
            return Err("Response too large".into());
        }
    }

    // 显式检查流结束
    if !response.body_mut().is_end_stream() {
        return Err("Stream not properly terminated".into());
    }

    Ok(body)
}