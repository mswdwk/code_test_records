 fn foo(x: &i32) { 
	println!("x = {}",*&x);
	println!("x = {}",x);
	println!("x = {}",*x);
}


 fn bar() { }

fn main()
{
 
 let option = Some(5);
		if let Some(x) = option {
			foo(&x);
		} else {
			bar();
		}
}
