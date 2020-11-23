package com.example;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
/**
 * Hello world!
 *
 */
public class App 
{
    public interface CLibrary extends Library {
        CLibrary INSTANCE = (CLibrary)
            Native.load("mytest",
CLibrary.class);

        String stripconst(String origin);
    }

    public static void main( String[] args )
    {
		String a="this is a";
		String b = CLibrary.INSTANCE.stripconst(a);
        System.out.println( "Hello World!"+a+" "+b );
    }
}
