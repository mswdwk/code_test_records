CC=gcc
J2C_SO=J2C.so
#J2C_SO=j2c.so
J2C_H=J2C.h
J2C_A=libj2c.a
.PHONY:all

all:${J2C_SO} ${J2C_H} J2C.class ${J2C_A}
	@echo “all compile”

${J2C_H}:J2C.class
	javah J2C


%.o:%.c
	@${CC} -c $< -o $@ -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux

${J2C_SO}:J2C.o
	@#gcc -c $^ -o J2C.o -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux
	gcc -shared -Wl,-soname,libj2c.so.1 -o libj2c.so.1.0 J2C.o 
	mv libj2c.so.1.0 libj2c.so
${J2C_A}:J2C.o
	ar -rcs $@ $^

J2C.class:
	javac J2C.java
clean:
	rm -rf ${J2C_SO} *.o *.class *.so

run:
	@ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.  && java J2C
	@#java J2C
