package org.example;


/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        try {
            C3P0.init();
            Thread.sleep(1000 * 30000);
            System.out.println("Hello World!");
        }catch (Exception e){
            System.out.println("error:"+e.toString());
            e.printStackTrace();
        }
    }
}
