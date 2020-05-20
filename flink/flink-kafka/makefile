.PHONY:all clean
all:
	mvn clean package
clean:
	mvn clean;rm -rf target/
run:
	flink run -d target/frauddetection-0.1.jar 
list:
	flink list --bootstrap-server localhost:9092 
