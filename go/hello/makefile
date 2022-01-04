D=@
.PHONY:get build run
help:
	$Decho "1 replace example.com/greetings to greetings"
	$Decho "2 tidy"
get:
	go get -u -v
build:
	go build -ldflags "-s"

run:
	$Dgo run .

clean:
	rm -rf hello

replace:
	go mod edit -replace example.com/greetings=../greetings
tidy:
	go mod tidy
