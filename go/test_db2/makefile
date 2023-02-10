D=@
.PHONY:get build run init
build:init tidy 
	go build -ldflags "-s"
init:
	$D go mod init examp/db2test
help:
	$Decho "1 replace example.com/greetings to greetings"
	$Decho "2 tidy"
get:
	go get -u -v
run:
	$Dgo run .

clean:
	rm -rf hello  test_mysql go.mod go.sum *.exe

replace:
	go mod edit -replace example.com/greetings=../greetings
tidy:
	go mod tidy
