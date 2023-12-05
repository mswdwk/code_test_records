
.PHONY: all run clean

all:init
	go build -v

run:
	go run .

clean:
	rm -f *.exe *.out main   go.mod go.sum web

go.mod: 
	go mod init example.com/web
go.sum:
	go mod tidy

init: go.mod go.sum
