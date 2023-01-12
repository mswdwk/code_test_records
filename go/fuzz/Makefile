D=@
.PHONY: all run clean

build:
	go build .
run:
	go run .
test:build
	$Decho "Run the fuzz test without fuzzing it to make sure the seed inputs pass"
	$Dgo test

fuzz:build
	go test -fuzz=Fuzz

fuzz2:
	go test -run=FuzzReverse 

fuzz3:
	go test -fuzz=Fuzz -fuzztime 30s

clean:
	rm -rf *.exe *.out testdata