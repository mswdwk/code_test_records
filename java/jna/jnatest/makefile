.PHONY:all clean run asm
all:
	mvn clean package -DskipTests

run:
	java -Djava.library.path=/lib64:. -cp target/jna-1.0-SNAPSHOT.jar com.example.App

asm:
	LD_LIBRARY_PATH=. mvn clean compile assembly:single -DskipTests -Djava.library.path=. -Djna.library.path=.

clean:
	rm -rf target *.o *.so

%.o:%.c
	gcc -g $< -o $@

so:mytest.c
	gcc -g -c $^ -o mytest.o -fPIC 
	gcc -shared -Wl,-soname,libmytest.so.1 -o libmytest.so.1.0 mytest.o 
	mv libmytest.so.1.0 libmytest.so
