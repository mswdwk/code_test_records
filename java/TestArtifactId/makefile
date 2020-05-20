.PHONY:all clean test 

all:
	mvn clean package -Dmaven.test.skip=true
	#mvn clean package -DskipTest
run:
	mvn exec:java -Dexec.mainClass="TestGroupId.App"
run2:
	java -jar target/TestArtifactId-1.0-SNAPSHOT.jar
test:
	mvn clean test

test2:
	@#run AppTest::testApp2
	mvn  test -Dtest=AppTest#testApp2

clean:
	mvn clean; rm -rf target/
