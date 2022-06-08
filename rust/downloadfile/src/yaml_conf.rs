extern crate serde_yaml;
extern crate serde;

use std::path::PathBuf;
use serde::{Serialize, Deserialize};

/// 定义 ResourceLink 类型
#[derive(Debug, Serialize, Deserialize)]
pub struct AResourceConfig {
    pub base_url: String,
    pub save_file_suffix: String,
    pub id_start: u32,
    pub id_stop: u32,
    pub save_dir: PathBuf,
    pub sleep_ms: u32
  //  friends: Vec<String>,
}

#[test]
fn t_read_yaml(){
    // 使用相对路径读取 app.yaml 内容为字符串
    let yaml_str = include_str!("../resource.yml");
    // serde_yaml 解析字符串为 User 对象
    let a_res: AResourceConfig = serde_yaml::from_str(yaml_str)
        .expect("resource.yml read failed!");
    println!("a_res={:?}", a_res);
}