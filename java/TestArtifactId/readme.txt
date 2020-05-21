1) use mvn to create project
mvn archetype:generate

2) skip test. 下面命令跳过测试：
mvn install -DskipTest

如果想不仅跳过测试运行，还跳过测试代码的编译，使用下面命令：
mvn package -Dmaven.test.skip=true
maven.test.skip 控制了 maven-compiler-plugin 和 maven-surefire-plugin 两个插件的行为。

3) Maven动态指定测试用例
mvn test -Dtest=RandomTest
只执行 RandomTest 这个测试类.

mvn test -Dtest=RandomTest#myTest
上面命令，只运行 RandomTest 类的 myTest 方法

可以指定多个类，逗号分隔
mvn test -Dtest=RandomTest,Random2Test

也可以用 * 匹配多个
mvn test -Dtest=Random*Test
*和 逗号可以结合使用。

如果不指定或者找不到测试类则构建失败
mvn test -Dtest
failIfNoTests 参数控制没有测试用例不报错
mvn test -Dtest -DfailIfNoTests=false

----------------------------------------------------------------------------------
原文链接：https://blog.csdn.net/yonggang7/java/article/details/79780487


4) mainclass
一般运行包含manifest的jar包，可以使用

java -jar <jar-file-name>.jar
如果jar里没有 manifest，则可以使用

java -cp foo.jar full.package.name.ClassName
当main类依赖多个jar时，可以把多个jar打包到一个目录，然后用-Djava.ext.dirs指定该目录，引用依赖的多个jar。

java -Djava.ext.dirs=<多个jar包的目录> com.test.HelloWordMain
如果用-cp则需要写每一个jar，很麻烦。

https://www.cnblogs.com/coderland/p/5902900.html
