.PHONY:all clean

all:
	GOOS=js GOARCH=wasm go build -o main.wasm toWasm.go

clean:
	rm -rf *.o
