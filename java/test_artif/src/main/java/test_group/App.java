package test_group;

/**
 * Hello world!
 *
 */

import test_group.Auth;

public class App 
{
    public static void main( String[] args )
    {
        Auth.CLogin.Builder  b = Auth.CLogin.newBuilder();
        b.setAppVersion("123");
        b.setUserid("1234567890userid");
        b.setPasswd("password");

        Auth.CLogin cl = b.build();
        byte [] arr = cl.toByteArray();
        Auth.CLogin a = null;
        try {
            a = Auth.CLogin.parseFrom(arr);
        } catch (Exception e){
            e.printStackTrace();
        }

        System.out.println( "Hello World! len="+arr.length + " a="+a );
    }
}
