extern crate chrono;
extern crate env_logger;
extern crate log;
use std::io::Write;
use chrono::Local;
use env_logger::Builder;
use log::LevelFilter;

pub fn init(){
    Builder::new()
        .format(|buf, record| {
            writeln!(buf,
                     "{} [{}] - {}",
                     Local::now().format("%Y-%m-%dT%H:%M:%S"),
                     record.level(),
                     record.args()
            )
        })
        .filter(None, LevelFilter::Info)
        .init();
}