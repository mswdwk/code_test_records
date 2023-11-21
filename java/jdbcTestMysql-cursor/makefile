# log4j2 usage: https://www.panziye.com/back/10371.html
.PHONY:all run clean

all:
	mvn clean package
run:
	mvn exec:java -Dexec.mainClass="org.example.App" -Dlog4j2.configurationFile=file:/tmp/log4j2.properties 
