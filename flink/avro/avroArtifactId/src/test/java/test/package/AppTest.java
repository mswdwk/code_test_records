package test.packag;

import java.io.File;
import java.io.IOException;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;
import user_machine_learning.product;

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
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( AppTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testApp()
    {
//	Test_avro();
//    }
//}


//private class Test_avro {
  //  public static void main(String[] args) throws IOException {

        //TODO 序列化操作
        product pro = product.newBuilder().build();
        pro.setProductId("1");
        pro.setCompanyName("这是一个测试");
        pro.setProductInfo("测试的详细说明");
        pro.setDirection("1");
        //将生成的数据保存到本地文件中
        File file = new File("user.avro");
      /*  DatumWriter<product> productDatumWriter = new SpecificDatumWriter<product>(product.class);
        DataFileWriter<product> dataFileWriter = new DataFileWriter<product>(productDatumWriter);
        dataFileWriter.create(product.getClassSchema() , file);
        dataFileWriter.append(pro);
        dataFileWriter.close();

        //TODO 反序列
        DatumReader<product> productDatumReader = new SpecificDatumReader<product>(product.class);
        DataFileReader<product> productDataFileReader = new DataFileReader<product>(file , productDatumReader);
        product pro_reader = null;
        while (productDataFileReader.hasNext()){
            pro_reader = productDataFileReader.next();
            System.out.println(pro_reader);
        }*/
        assertTrue( true );
    }
}
