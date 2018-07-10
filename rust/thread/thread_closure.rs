use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

fn main() {
    let data = Arc::new(Mutex::new(vec![1, 2, 3]));

    for i in 0..3 {
        let data = data.clone();
        thread::spawn(move || {
            let mut data = data.lock().unwrap();
			println!("data[0] = {}",data[0]);
            data[0] += i;
        });
    }

    thread::sleep(Duration::from_millis(50));
}
