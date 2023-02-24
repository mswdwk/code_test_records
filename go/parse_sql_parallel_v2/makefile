D=@
.PHONY:get build run

build:go.mod go.sum
	go build -race -ldflags "-s"
cover:
	go build -cover   .
go.mod: 
	go mod init example.com/parsesql
go.sum:
	go mod tidy
help:
	$Decho "1 replace example.com/greetings to greetings"
	$Decho "2 tidy"
get:
	go get -u -v
	
run:
	$Dgo run .  all.sql

clean:
	rm -rf go.* error.sql* success.sql*

replace:
	go mod edit -replace example.com/greetings=../greetings

test:
	go test -cover

statistics:
	rm -rf error.sql* success.sql*
	sh parseAllSqls.sh
	sort error.sql.normal|uniq -c|sort -n -k 1 -r > e.normal
	sort success.sql.normal|uniq -c|sort -n -k 1 -r > s.normal
