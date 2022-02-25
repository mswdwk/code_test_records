import java.net.InetAddress;

public class NameResolve
{
    public static void main( String[] args )
    {
        try {
            // InetAddress[] addrs = java.net.Inet4Address.getAllByName("x.123.123.baidu.com.com.ba");
			String ns="www.baidu.com";
			if(args.length>0)
				ns=args[0];	
            System.out.println("args:" +args.length );
            System.out.println("yu min:" +ns);
            //System.out.println("args:" + args[0]);
            InetAddress[] addrs = java.net.Inet4Address.getAllByName(ns);
            System.out.println("addr2:" + addrs.length);
            for(InetAddress addr: addrs) {
                System.out.println("addr:" + addr);
                // System.out.println("addr1:"+addr.length());
            }
        } catch (Exception e){
            System.out.println("error:"+e.toString());
            e.printStackTrace();
        }
    }
}

