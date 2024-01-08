FILE=go.mod
.PHONY: all clean run test
exist=$(shell if [ -f $(FILE) ]; then echo exist; else echo not exist; fi;)
BIN=gt1
all:init
	go build .	

init:
ifeq (${exist},exist)
	$Decho "already exist $(FILE)"
else
	$Dgo mod init examp/${BIN}
	$Dgo mod tidy
endif

run:
	./${BIN} &

clean:
	rm -rf ${BIN} go.mod go.sum

test:
	curl http://localhost:8081/albums
	curl http://localhost:8081/albums --include \
    --header "Content-Type: application/json" \
    --request "POST" \
    --data '{"id": "4","title": "The Modern Sound of Betty Carter","artist": "Betty Carter","price": 49.99}'
