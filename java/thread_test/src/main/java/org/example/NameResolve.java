package org.example;
import java.net.InetAddress;
/*
* java.net.Inet4AddressImpl 这个jar包
https://www.cnblogs.com/549294286/p/9117124.html
JNIEXPORT jobjectArray JNICALL
Java_java_net_Inet4AddressImpl_lookupAllHostAddr(JNIEnv *env, jobject this, host)
// create the impl
impl = InetAddressImplFactory.create();
// create name service
nameService = createNameService();
* */
public class NameResolve implements Runnable {
    private Thread t;
    private String domainName;
	private int loop_count;

 	NameResolve( String name,int count) {
        domainName = name;
		loop_count = count;
        System.out.println("Creating Thread Name: " +  domainName +" , loop_count: "+count);
    }

    public void run() {
        System.out.println("Running NameResolve: " +  domainName );
		int i =0;
        try {
            for(;i< loop_count; i++) {
			    InetAddress[] addrs = java.net.Inet4Address.getAllByName(domainName);
				if(addrs.length == 0){
                	System.out.printf("Thread: " + domainName + " , " + i);
            		System.out.println(" domain [" + domainName + "] ip addr num: " + addrs.length+" loop_id "+i);
				}
            	for(InetAddress addr: addrs) {
                	//System.out.println("addr:" + addr);
                	// System.out.println("addr1:"+addr.length());
                }
                // 让线程睡眠一会
                Thread.sleep(1);
            }
        }catch (Exception e) {
            System.out.println("Thread " +  domainName + " interrupted. loop_id "+i+", error: "+e.toString());
			e.printStackTrace();
        }
        System.out.println("Thread " +  domainName + " exiting.");
    }

    public void start () {
        System.out.println("Starting Thread: " +  domainName );
        if (t == null) {
            t = new Thread (this, domainName);
            t.start ();
        }
    }
}
