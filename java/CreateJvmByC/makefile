CC=gcc
J2C_SO=J2C.so
#J2C_SO=j2c.so
J2C_H=J2C.h
J2C_A=libj2c.a
CFLAGS=-I${JAVA_HOME}/include/linux -I${JAVA_HOME}/include -fPIC -L${JAVA_HOME}/jre/lib/amd64/server -ljvm
D=
.PHONY:all

all:main
	@echo “all compile”

main:createJvmTest.o
	gcc $^ -g -o $@ ${CFLAGS}

%.o:%.c
	$D${CC} -c $< -o $@ ${CFLAGS}


J2C.class:
	javac J2C.java
clean:
	rm -rf ${J2C_SO} *.o *.class *.so

run:
	@ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.  && java J2C
	@#java J2C
