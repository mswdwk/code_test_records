use std::net::TcpListener;
use std::{thread, time};
use std::net::TcpStream;
use std::io::prelude::*;
use std::io::stdin;
use std::sync::Arc;
use std::sync::Mutex;
use std::net::Shutdown;
use std::sync::atomic::{AtomicUsize, Ordering};

fn main() {
    let listener=TcpListener::bind("127.0.0.1:8080").unwrap();
    println!("start listening!");
    let num = Arc::new(Mutex::new(0));
    for stream in listener.incoming(){
        let clild_num = num.clone();
        let base_stream = stream.unwrap().try_clone().unwrap();
        thread::spawn(move||{
            {
                let mut  v = clild_num.lock().unwrap();
                *v +=1;
                println!("{}", *v);
            }
            println!("come in !!");
            let mut stream = base_stream;
            println!("from {:?}",stream.peer_addr());
            let mut bytes: Vec<u8> = Vec::new();;
            stream.read(&mut bytes);
            loop {
                stream.write(String::from("1").as_bytes());
                thread::sleep(time::Duration::from_secs(2));
            }
            println!("come end !!");
        });
    }
}