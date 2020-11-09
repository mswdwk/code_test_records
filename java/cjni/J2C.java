import java.lang.management.ManagementFactory;
import java.lang.management.RuntimeMXBean;

public class J2C
{
     static
     {
          try{
               // 此处即为本地方法所在链接库名
               System.loadLibrary("j2c");
          } catch(UnsatisfiedLinkError e)
          {
               System.err.println( "Cannot load j2c library:\n " +
               e.toString() );
          }
     }

     //声明的本地方法
     public static native int write2proc(int pid);

     public static void main(String[] args){

          //获取本进程(即主线程)的pid
          final RuntimeMXBean runtime = ManagementFactory.getRuntimeMXBean();
          final String info = runtime.getName();
          final int index = info.indexOf("@");


          if (index != -1) {
               final int pid = Integer.parseInt(info.substring(0, index));
               System.out.println(info);
               System.out.println(pid);

               write2proc(pid);
          }


          try{
               Thread.sleep(8000);
          } catch(InterruptedException e){
               e.printStackTrace();
          }
     }
}
