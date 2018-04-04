public class Loop {
    public static int ALOOPTIME=10000;
    public static  int LOOP=10000;
    public  static void loop10k(){
        int i = 0;
        while(i<ALOOPTIME)i++;
    }
    public static void main(String args[]) {
        for(int i =0;i <LOOP;i++)
            loop10k();
        System.out.print("loop " + LOOP + "*" + ALOOPTIME + " times\n");
      
   }
}
