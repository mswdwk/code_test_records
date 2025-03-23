# JAVALIB=${JAVA_HOME}/lib/:~/.m2/repository/mysql/mysql-connector-java/5.1.49:~/.m2/repository/com/mysql/mysql-connector-j
JDBC_JAR=~/.m2/repository/com/mysql/mysql-connector-j/8.0.31
JDBC_JAR2=~/.m2/repository/mysql/mysql-connector-java/5.1.49/mysql-connector-java-5.1.49.jar
JAVALIB=${JDBC_JAR2}
.PHONY:all clean run

all:
	javac src/MySQLDemo.java
run:
	java -cp ${JAVALIB}:./src  MySQLDemo
clean:
	rm src/*.class
