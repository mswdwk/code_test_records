package org.example;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        // 创建类A和类B的实例
        HasPropertyC objA = new ClassA(15); // c=15
        HasPropertyC objB = new ClassB(-5); // c=-5

        // 定义策略
        CModifyStrategy strategy1 = new GreaterThan10Strategy();
        CModifyStrategy strategy2 = new LessThan0Strategy();

        // 处理对象
        PropertyCHandler.handle(objA, strategy1); // objA的c变为0（因15>10）
        PropertyCHandler.handle(objB, strategy2); // objB的c变为100（因-5<0）

        System.out.println(objA.getC()); // 输出：0
        System.out.println(objB.getC()); // 输出：100
    }
}
