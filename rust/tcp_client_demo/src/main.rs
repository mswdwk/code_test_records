use std::io::prelude::*;
use std::net::TcpStream;
// use std::io::stdin;
use std::{thread, time};
//use std::borrow::Borrow;

fn main() {
    let mut stream=TcpStream::connect("127.0.0.1:8080").unwrap();
    stream.set_nodelay(true).unwrap();
    //loop{
    let mut test_stream=stream.try_clone().unwrap();
    if let Err(e) = thread::Builder::new().name("test_thread".into()).stack_size(819200).spawn(move||{
        loop{
            let mut buffer=[0;1024];
            let num= test_stream.read(&mut buffer[..]).unwrap();
            println!("thread name[{:?}] recv data len: {:?}",thread::current().name().unwrap_or(""),num);
            if num==0{
                println!("server 连接关闭了");
                break;
            }
            let rcv_str= String::from_utf8_lossy(&buffer[0..num]);
            println!("recv data raw:{:?}",&buffer[0..num]);
            println!("recv data str: {}",rcv_str);

        }
    }){
        println!("create thread error ! {}",e.to_string());
    }

    let mut count = 0;
    let mut response;//= String::from("hello world, ");
    loop {
        // stdin().read_line(&mut response).ok().expect("nothing");
        // print!("{}",response);
        count += 1;
        response = format!("Hello World, this is {} times",count);
        // response.push_str(count.to_string().as_str());
        let send_bytes = stream.write(response.as_bytes()).unwrap();
        println!("send_bytes {}",send_bytes);
        thread::sleep(time::Duration::from_secs(2));
    }
    /*
    loop{
    let mut response=String::new();
    let mut send=String::new();
    stdin().read_line(& mut send).ok().expect("shit");
    stream.write(send.as_bytes()).unwrap();
    //stream.write(send.as_bytes()).unwrap();
    //println!("{:?}",stream.ttl());
    stream.shutdown(Shutdown::Write).unwrap();
    stream.read_to_string(&mut response).unwrap();
    println!("other messages:{:?}",response);
    };
    */
}