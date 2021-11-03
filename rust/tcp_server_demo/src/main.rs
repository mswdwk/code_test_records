use std::net::TcpListener;
use std::{thread, time};
//use std::net::TcpStream;
use std::io::prelude::*;
// use std::io::stdin;
use std::sync::Arc;
use std::sync::Mutex;
// use std::net::Shutdown;
// use std::sync::atomic::{AtomicUsize, Ordering};

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
                println!("current client count: {}", *v);
            }
            println!("client come in !!");
            let mut stream = base_stream;
            println!("client from {:?}, set block {:?}",stream.peer_addr(),stream.set_nonblocking(false));

            const BUFSIZE:u32 = 128;
            let mut v = Vec::new();
            println!("recv_buf capacity: {}", v.capacity());
            v.resize(BUFSIZE as usize,0 as u8); // let v has data, and length eq BUFSIZE
            v.fill(0); // fill v with 0 ,position from 0 to BUFSIZE
            println!("recv_buf capacity: {} {}", v.capacity(),v.len());
            let mut v2 = vec![0 as u8;BUFSIZE as usize];
            let mut s = [0 as u8;BUFSIZE as usize ] ;
            let kind = 3;
            let recv_buf= if 1 == kind {
                v.as_mut_slice()
            } else if 2 == kind {
                v2.as_mut_slice()
            } else {
                &mut s
            };
            println!("len {}",recv_buf.len());
            let mut count:u32 = 0;
            loop {
                match stream.read( recv_buf) {
                    Ok(0) => {
                        thread::sleep(time::Duration::from_secs(2));
                    }
                    Ok(n ) => {
                        //stream.read_to_end(&mut recv_buf);
                        count+=1;
                        // stream.write(&count.to_be_bytes());
                        let mut send_bytes = stream.write(count.to_string().as_bytes()).unwrap_or(0);
                        // stream.write(String::from("1").as_bytes());
                        send_bytes += stream.write(&recv_buf[0..n]).unwrap_or(0);
                        println!("recv {} times, data len {}, raw data:{:?}",count, n, recv_buf);
                        println!("send data len {}",send_bytes);
                        recv_buf.fill(0);
                    }
                    Err(e) => {
                        thread::sleep(time::Duration::from_secs(2));
                        println!("read error: {}",e.to_string());
                    }
                }
            }
            //println!("come end !!");
        });
    }
}