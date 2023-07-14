package main

import (
    "testing"
   _ "regexp"
)

// TestHelloName calls greetings.Hello with a name, checking
// for a valid return value.
/*func TestHelloName(t *testing.T) {
    name := "Gladys"
    want := regexp.MustCompile(`\b`+name+`\b`)
    msg, err := Hello("Gladys")
    if !want.MatchString(msg) || err != nil {
        t.Fatalf(`Hello("Gladys") = %q, %v, want match for %#q, nil`, msg, err, want)
    }
}*/


func Test_mysql(t *testing.T){
	test_mysql()
	if 1 != 1 {
		t.Errorf("test string 1")
	}	
}

func TestListDir(t *testing.T){
	list_dir()
	if 1 != 1 {
		t.Errorf("test string 2")
	}	
}

func TestMain(t *testing.T){
	main()
	if 1 != 1 {
		t.Errorf("test string 3")
	}	
}
