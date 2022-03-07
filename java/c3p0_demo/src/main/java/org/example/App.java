package org.example;


import java.sql.Connection;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        try {
            C3P0.init();
            System.out.println("C3P0: MaxConnectionAge: "+C3P0.cpds.getMaxConnectionAge());
            System.out.println("C3P0: MaxIdleTime: "+C3P0.cpds.getMaxIdleTime());
            System.out.println("C3P0: IdleConnectionTestPeriod: "+C3P0.cpds.getIdleConnectionTestPeriod());
            Connection conn = C3P0.cpds.getConnection();
            com.mysql.jdbc.Connection mysql_con ;
            mysql_con.getClientInfo()
            System.out.println("C3P0: clientInfo: clientInfo size "+ conn.getClientInfo().size());
            System.out.println("C3P0: clientInfo: port "+ conn.getClientInfo().getProperty("client_port"));
            System.out.println("C3P0: connection min pool size is "+C3P0.cpds.getMinPoolSize()+", max pool size:"+C3P0.cpds.getMaxPoolSize() );
            System.out.println("C3P0: connection number is "+C3P0.cpds.getNumConnections() +
                    ", Idle Connection number is "+C3P0.cpds.getNumIdleConnections());

            Thread.sleep(1000 * 30000);
            System.out.println("Hello World!");
        }catch (Exception e){
            System.out.println("error:"+e.toString());
            e.printStackTrace();
        }
    }
}
