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
        thdemo R1 = new thdemo( "Thread-1");
        R1.start();

        thdemo R2 = new thdemo( "Thread-2");
        R2.start();
    }
}
