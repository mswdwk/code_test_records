.PHONY: all clean run test
all:
	mvn clean package -DskipTests
run:
	mvn exec:java -Dexec.mainClass="org.example.App"
test:
	mvn test
clean:
	rm -rf target
