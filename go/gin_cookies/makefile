D=@
GOFLAGS=-ldflags "-s" -race
FILE=go.mod
.PHONY: all clean run test 
exist=$(shell if [ -f $(FILE) ]; then echo exist; else echo not exist; fi;)

TARGETS:=$(subst .go,,$(wildcard *.go))
PROTO_GOS:=$(wildcard gRPC/*.go)
BIN=cok
OS=$(shell uname -o)

all:init ${TARGETS}
	@echo targets are: ${TARGETS}
	@echo proto go files are: ${PROTO_GOS}

init:
ifeq (${exist},exist)
	$Decho "already exist $(FILE)"
else
	$Dgo mod init examp/${BIN}
	$Dgo mod tidy
endif

%:%.go
	go build ${GOFLAGS} -o $@   $< 

run:
	./${BIN} &

clean:
	rm -rf ${TARGETS} go.mod go.sum

test:
	curl http://localhost:8081/albums
	curl http://localhost:8081/albums --include \
    --header "Content-Type: application/json" \
    --request "POST" \
    --data '{"id": "4","title": "The Modern Sound of Betty Carter","artist": "Betty Carter","price": 49.99}'
	curl http://localhost:8081/albums/1
	curl http://localhost:8081/albums/12

# install protoc-gen-go on MacOS
install_gen_go_grpc:
	@if [ $(OS) = "Darwin" ]; then \
		brew install protobuf protoc-gen-go protoc-gen-go-grpc ; \
	fi

# generate protobuf go file , grpc file
pb:
	#protoc -I . --go_out=plugins=grpc:. ./gRPC/proto/user.proto
	protoc --proto_path=gRPC/proto --go_out=gRPC --go_opt=paths=source_relative user.proto
	protoc --proto_path=gRPC/proto --go-grpc_out=gRPC --go-grpc_opt=paths=source_relative,require_unimplemented_servers=false user.proto