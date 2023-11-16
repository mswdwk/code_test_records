D=@
EXENAME:=parallel
.PHONY:get build run

build:go.mod go.sum
	go build -ldflags "-s"
cover:
	go build -cover   .
go.mod: 
	go mod init example.com/${EXENAME}
go.sum:
	go mod tidy
help:
	$Decho "1 replace example.com/parallel to greetings"
	$Decho "2 tidy"
get:
	go get -u -v
	go get -v github.com/pingcap/tidb/types/parser_driver@fc166ff

run:
	$Dgo run .

clean:
	rm -rf go.* error.sql* success.sql* u.* e.* s.* ${EXENAME}

replace:
	go mod edit -replace example.com/greetings=../greetings

test:
	go test -cover

