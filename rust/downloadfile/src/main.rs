//#[macro_use]
#![feature(async_closure)]
mod logini;
mod yaml_conf;

use anyhow::{anyhow, Result};
use futures::stream::StreamExt;
use std::{borrow::Cow, fs, path::{Path, PathBuf}, thread};
use std::time::Duration;

use log::{debug, info, warn};
use structopt::StructOpt;
use tokio::io::AsyncBufReadExt;
use tokio::io::BufReader;
use tokio::{fs::OpenOptions, io::AsyncWriteExt};
use yaml_conf::AResourceConfig;

fn basename(path: &str, sep: char) -> Cow<str> {
    let mut pieces = path.rsplit(sep);
    match pieces.next() {
        Some(p) => p.into(),
        None => path.into(),
    }
}

/// A basic example
#[derive(StructOpt, Debug)]
#[structopt(name = "basic")]
struct Opt {
    /// Input file
    #[structopt(short, long, parse(from_os_str))]
    input: PathBuf,
    /// Activate debug mode
    #[structopt(short, long)]
    debug: bool,
    // #[structopt(short,long,parse(from_os_str),default_value = ".")]
    // savedir: PathBuf
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    logini::init();

    let opt = Opt::from_args();
    let inputfile = opt.input;
    let debug = opt.debug;


    // 使用相对路径读取 app.yaml 内容为字符串
    // let yaml_str = include_str!(input_file_str.as_str());
    let yaml_str = fs::read_to_string(&inputfile)?;

    info!("yaml_str  {}",yaml_str);
    // let yaml_str = include_str!(yaml_str);
    // serde_yaml 解析字符串为 AResourceConfig 对象
    let a_res: AResourceConfig = serde_yaml::from_str(&yaml_str)
        .expect(&format!("resource yaml file: {} resolve failed! ",yaml_str));
    let save_dir = a_res.save_dir.as_path();
    let sleep_ms = a_res.sleep_ms;
    info!("xxxx save_dir {:?}",a_res.save_dir.as_path());
    info!("begin to download file by urls in path {:?} sleep_ms {} ", inputfile,sleep_ms);
    debug!("test debug");
    // return   Ok(());
    // let urls: Vec<String> = read_lines(inputfile).await?;
    let urls = create_urls(&a_res.base_url, &a_res.save_file_suffix, a_res.id_start,a_res.id_stop)?;
    let length = urls.len();
    // let http_header = new reqwest::header;
    let fetches = futures::stream::iter(urls.into_iter().enumerate().map(
        |(index, url)| async move {
            // limit concurrent ?
            thread::sleep(Duration::from_millis(sleep_ms as u64));
            let mut response = get(&url).await?;
            let filename = basename(&url, '/');
            if debug {
                println!("[{:6}/{}] {}", index, length, filename);
            }
            save(save_dir,filename.as_ref(), &mut response,debug).await?;
            Ok::<(), Box<dyn std::error::Error>>(())
        },
    ))
    .buffered(7)
    .collect::<Vec<_>>();
    fetches.await;

    Ok(())
}

async fn read_lines<P: AsRef<Path>>(path: P) -> std::io::Result<Vec<String>> {
    let file = OpenOptions::new().read(true).open(path).await?;
    let reader = BufReader::new(file);
    let mut lines = reader.lines();
    let mut res = Vec::new();
    while let Some(line) = lines.next_line().await? {
        if let Ok((base_url, suffix, start,end)) = split_one_line(line) {
            println!("base_url:{},suffix:{} ,start:{} ,end:{}",base_url,suffix,start,end);
            res.extend(create_urls(&base_url, &suffix, start,end).unwrap());
        }
    }
    Ok(res)
}

fn split_one_line(url:String) -> Result<(String,String,u32,u32)>{
    let  res  = url.split(",").collect::<Vec<&str>>();
    debug!("line spilt {} fields",res.len());
    if res.len() < 4 {
        return Err(anyhow!("spilt failed"));
    }

    Ok((res[0].to_string(),res[1].to_string(),str::parse::<u32>(res[2]).unwrap(),str::parse::<u32>(res[3]).unwrap()))
}

async fn get(url: &str) -> Result<reqwest::Response> {
    reqwest::get(url)
        .await
        .map_err(|e| anyhow!("Request url {} error: {}", url, e))
}

async fn save(save_dir: &Path,filename: &str, response: &mut reqwest::Response,debug: bool) -> Result<()> {
    // if dir not exist, then create it
    if ! save_dir.exists(){
        fs::create_dir_all(save_dir)?;
        if debug {
            println!("Create dir {:?} Ok",save_dir);
        }
    }
    let path_filename = save_dir.join(filename);
    if path_filename.exists(){
        let err_msg = format!("file {:?} already exists!",path_filename);
        warn!("{}",err_msg);
        return Err(anyhow!(err_msg));
    }
    debug!("start save file {:?}",path_filename.as_path());
    let mut options = OpenOptions::new();
    let mut file = options
        .append(true)
        .create(true)
        .write(true)
        .open(path_filename.as_path())
        .await?;
    info!("create file {:?} ok",path_filename.as_path());
    while let Some(chunk) = &response.chunk().await.expect("Failed") {
        debug!("chunk size({})  ",chunk.len());
        if chunk.len() < 1 {
            warn!("chunk size({}) is too small ! ",chunk.len());
            return Ok(());
        }
        match file.write_all(&chunk).await {
            Ok(_) => {}
            Err(e) => {
                let err_msg = format!("File {} save error: {}", filename, e);
                warn!("{}",err_msg);
                return Err(anyhow!(err_msg));
            }
        }
    }
    info!("save file {} Ok",filename);
    Ok(())
}

fn create_urls(url: &str, file_suffix: &str, start: u32,end: u32) -> Result<Vec<String>> {
    let mut res = Vec::new();
    let mut i = start;
    while i <= end {
        let url_ = if i == 0 {
            format!("{}{}{}", url, i, file_suffix)
        } else {
            format!("{}{:03}{}", url, i, file_suffix)
        };
        debug!("this url is {:?}", url_);
        i += 1;
        res.push(url_)
    }
    Ok(res)
}

#[test]
fn basic_create_urls() {
    let res = create_urls("url", ".suf", 1,2).unwrap();
    let nres = vec!["url001.suf", "url002.suf"];
    assert_eq!(res, nres)
}

#[test]
fn format_int() {
    let res = format!("{}{:03}{}", "url", 5, ".suf");
    println!("this is {}", res);
    assert_eq!(res, "url005.suf")
}

#[test]
fn t_split_line(){
    let res = split_one_line("http://a.com/b/,.suf,1,2".to_string()).unwrap();
    assert_eq!(res,("http://a.com/b/".to_string(),".suf".to_string(),1,2))
}

/*#[test]
 fn t_read_lines(){
    let _ = async
    let inputfile = PathBuf::new(r".\src\urls.txt");
    println!("begin to download file {:?} ...", inputfile);
    let urls: Vec<String> = read_lines(inputfile).await?;
}*/



#[test]
fn t_create_file() {
    let c = async ||->Result<()> {
        let path_filename = Path::new("E:/x/y.cc");
        let mut options = OpenOptions::new();
        let file = options
            .append(true)
            .create(true)
            .write(true)
            .open(path_filename).await?;
            println!("create file {:?}", file);
            Ok(())
        };
    c();
    thread::sleep(Duration::from_secs(1));
}