package test_group;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import test_group.Auth;

/**
 * Unit test for simple App.
 */
public class AppTest 
{
    /**
     * Rigorous Test :-)
     */
    @Test
    public void shouldAnswerWithTrue()
    {
        assertTrue( true );
    }

    @Test
    public void test_speed(){
        Auth.CLogin.Builder  b = Auth.CLogin.newBuilder();
        int obj_count = 0xff ;
        Auth.CLogin []cLogin = new Auth.CLogin[obj_count+1];
        byte arr[][] = new byte[obj_count+1][];
        for(int i = 0; i <= obj_count; i++) {
            b.setAppVersion("1234567890abcdefglkadjlfksadfkmdklauioewur90jddslkvniosauf8dsufa"+i);
            b.setUserid("1234567890userid_jflkadsjflkjdafkljdslkafjioweuroijiosdaijfioeurq"+i);
            b.setPasswd("password_skldafjlkdsjlfkajdlskfjalkdjfklasdjflksdjfllksdfj"+i);
            b.setPlatform("platform_skladjfkdsjfkljawklejlfewjfoijiojfoi3243ur89wf0ad89fj"+i);

            b.setF5("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz1"+i);
            b.setF6("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz2"+i);
            b.setF7("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz3"+i);
            b.setF8("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz4"+i);

            b.setF9("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz6"+i);
            b.setF10("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz7"+i);
            b.setF11("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz8"+i);
            b.setF12("123456789009876543211357902468abcdefghijklmnopqrstuvwxyz9"+i);
            cLogin[i] = b.build();
            arr[i] = cLogin[i].toByteArray();
        }
        Auth.CLogin cl = b.build();
        //byte [] arr = cl.toByteArray();
        Auth.CLogin a = null;
        int i = 0;
        long loop_count = 100000;
        long start = System.nanoTime();
        try {
            for(;i<loop_count;i++)
                a = Auth.CLogin.parseFrom(arr[i&0xff]);
        } catch (Exception e){
            e.printStackTrace();
        }
        long cost_ns = System.nanoTime() - start;

        long speed = loop_count*1000*1000*1000/cost_ns;
        System.out.println( "Hello World! loop_count= "+loop_count+" cost_ms= "+cost_ns/1000/1000+ " speed="+speed+" parse/s len="+arr.length + "\n a="+a );
        //assertEquals(cl,a);
    }
}
