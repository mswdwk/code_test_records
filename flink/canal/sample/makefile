.PHONY:all clean
all:
	CLASSPATH=${CLASSPATH}:/tmp/canal/lib mvn clean package -DskipTests
clean:
	mvn clean;rm -rf target/

cleanall:
	rm -rf  target/ 

create:
	mvn archetype:generate -DgroupId=com.alibaba.otter -DartifactId=canal.sample
run:
	#CLASSPATH=${CLASSPATH}:/tmp/canal/lib mvn exec:java -Dexec.mainClass="com.alibaba.otter.canal.sample.SimpleCanalClientExample"
	CLASSPATH=${CLASSPATH}:/tmp/canal/lib mvn exec:java -Dexec.mainClass="com.alibaba.otter.SimpleCanalClientExample"

