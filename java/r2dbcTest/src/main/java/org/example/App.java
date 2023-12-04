package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;


/**
 * Hello world!
 *
 */
public class App 
{
    private final static Logger log = LogManager.getLogger();

    public static void main( String[] args ) {
        log.info("args: "+args.length);

        // dbconnect.test_r2dbc_connect2();
        // dbconnect.test_for_jdbc_login_timeout();

        // TODO,wait r2dbc thread finish its job!
        try {
            MyConfig myConfig = new MyConfig("mybatis-config.xml");
            Dbconnect dbconnect = new Dbconnect(myConfig);
            //dbconnect.test_r2dbc_mysql("testdb","table1");
            dbconnect.test_r2dbc_create_database_table();
            // at least 200 millisecods to wait the finish of async r2dbc thread to start/run/finish.
            // Xatest.test();
            Thread.sleep(3000);
        } catch (Exception e){
            log.error("error:"+e);
        }
    }
}
