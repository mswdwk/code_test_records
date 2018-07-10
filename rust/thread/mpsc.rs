use std::thread;
use std::sync::mpsc;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel();

    for i in 0..10 {
        let tx = tx.clone();

        thread::spawn(move || {
			for j in 0..10{
				let id = 10*i+j ;
            	let answer = id * id;
            	tx.send(answer).unwrap();
			}
        });
    }
	thread::sleep(Duration::from_millis(50));
    for _ in 0..100 {
        println!("{}", rx.recv().unwrap());
    }
}
