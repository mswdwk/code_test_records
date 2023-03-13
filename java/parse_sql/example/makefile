.PHONY:all clean run
all:
	@#package all jar dependencies 
	mvn assembly:assembly  -Dmaven.test.skip=true 
all2:
	mvn clean package

run:
	mvn exec:java -Dexec.mainClass="com.example.App"
clean:
	rm -rf filename_*
	rm -rf target
