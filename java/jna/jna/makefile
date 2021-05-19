.PHONY:all clean run asm
all:
	mvn clean package -DskipTests

run:
	java -Djava.library.path=/lib64 -cp target/jna-1.0-SNAPSHOT.jar com.example.App

asm:
	mvn clean compile assembly:single -DskipTests 

clean:
	rm -rf target
