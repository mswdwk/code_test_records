use std::io::prelude::*;
use std::net::TcpStream;
use std::io::stdin;
use std::thread;
fn main() {
    let mut stream=TcpStream::connect("127.0.0.1:8080").unwrap();
    stream.set_nodelay(true).unwrap();
    loop{
        let mut test_stream=stream.try_clone().unwrap();
        thread::spawn(move||{
            loop{
                let mut buffer=[0;1024];
                let num=test_stream.read(&mut buffer[..]).unwrap();
                println!("{:?}",num);
                if num==0{
                    println!("连接关闭了");
                    break;
                }
                let result=std::str::from_utf8(&buffer[0..num]);
                print!("{}",result.unwrap());
            }
        });
        let mut response=String::new();
        stdin().read_line(&mut response).ok().expect("nothing");
        //print!("{}",response);
        stream.write(response.as_bytes()).unwrap();
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