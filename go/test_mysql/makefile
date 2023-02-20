D=@
.PHONY:get build run

build:go.mod go.sum
	go build -ldflags "-s"
cover:
	go build -cover   .
go.mod: 
	go mod init example.com/mysql
go.sum:
	go mod tidy
help:
	$Decho "1 replace example.com/greetings to greetings"
	$Decho "2 tidy"
get:
	go get -u -v
	
run:
	$Dgo run .

clean:
	rm -rf hello  test_mysql go.*

replace:
	go mod edit -replace example.com/greetings=../greetings


test:
	go test -cover
