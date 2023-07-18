FILE=go.mod
exist=$(shell if [ -f $(FILE) ]; then echo exist; else echo notexist; fi;)
D=@
.PHONY:get build run init
build:init tidy 
	go build -ldflags "-s"
init:
ifeq (${exist},exist)
	$Decho "already exist $(FILE)"
else
	$Dgo mod init demo
endif

help:
	$Decho "1 replace example.com/greetings to greetings"
	$Decho "2 tidy"
get:
	go get -u -v
run:
	$Dgo run .

clean:
	rm -rf hello  test_mysql go.mod go.sum *.exe db2test test  gethbase demo

replace:
	go mod edit -replace example.com/greetings=../greetings
tidy:
	go mod tidy
