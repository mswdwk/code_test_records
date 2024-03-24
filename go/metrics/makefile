D=@
FILE=go.mod
.PHONY: all clean run test
exist=$(shell if [ -f $(FILE) ]; then echo exist; else echo not exist; fi;)
BIN=metric_auth
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
	curl -u admin:admin123 http://localhost:8050/metrics
