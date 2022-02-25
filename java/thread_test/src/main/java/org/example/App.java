package org.example;

import sun.security.action.GetPropertyAction;

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
		
        NameResolve R3 = new NameResolve( "Name.Resolve",2000);
        R3.start();

        CreateFileThread R4 = new CreateFileThread("CreateFileThread",2000);
        R4.start();

		String hostfile = GetPropertyAction.privilegedGetProperty("jdk.net.hosts.file");
        System.out.println("hostfile: "+hostfile);
        System.out.println("negative.ttl "+GetPropertyAction.privilegedGetProperty("networkaddress.cache.negative.ttl"));
    }
}
