package TestGroupId;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class AppTest 
    extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public AppTest( String testName )
    {
	//System.out.println("AppTest");
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
	System.out.println("Test Suite");
        return new TestSuite( AppTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testApp()
    {
	System.out.println("this is test 1");
        assertTrue( true );
    }
    public void testApp2()
    {
	System.out.println("this is test 2");
        assert( 1 +1 == 2 );
    }
    public void testApp3()
    {
	System.out.println("this is test 3");
        assert( 1 +1 != 2 );
	}
}
