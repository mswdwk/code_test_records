D=@
.PHONY:get build run

build:go.mod go.sum
	go build -ldflags "-s"
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
	$D#go get -v github.com/pingcap/tidb/types/parser_driver@fc166ff
	go get -v github.com/pingcap/tidb/pkg/types/parser_driver@069631e

run:
	$Dgo run .

clean:
	rm -rf go.* error.sql* success.sql* u.* e.* s.*

cleangocache:
	go clean -modcache

replace:
	go mod edit -replace example.com/greetings=../greetings

test:
	go test -cover

statistics:
	rm -rf error.sql* success.sql*
	sh parseAllSqls.sh
	sort error.sql.normal|uniq -c|sort -n -k 1 -r > e.normal
	sort success.sql.normal|uniq -c|sort -n -k 1 -r > s.normal
