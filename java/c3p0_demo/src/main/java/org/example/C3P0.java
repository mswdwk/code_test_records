package org.example;
import com.mchange.v2.c3p0.*;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.concurrent.ExecutionException;

public class C3P0 {
    // TODO: Consiser Concurrence
    public  static ComboPooledDataSource cpds ;
    static public void init(){
        try {
            if (cpds == null) {
                cpds = new ComboPooledDataSource();
            }
            // cpds.setDriverClass("org.postgresql.Driver"); //loads the jdbc driver
            //cpds.setJdbcUrl("jdbc:postgresql://192.168.79.133:5432/testdb");
            cpds.setDriverClass("com.mysql.jdbc.Driver"); //loads the jdbc driver
            cpds.setJdbcUrl("jdbc:mysql://192.168.79.133:3310/testdb");
            cpds.setUser("root");
            cpds.setPassword("123");

            // the settings below are optional -- c3p0 can work with defaults
            cpds.setMinPoolSize(5);
            cpds.setAcquireIncrement(5);
            cpds.setMaxPoolSize(10);
            cpds.setMaxStatements( 180 );
            cpds.setIdleConnectionTestPeriod(5);
            cpds.setPreferredTestQuery("select 1234567890");

            Connection conn = cpds.getConnection();
            Statement stmt = conn.createStatement();
            ResultSet res = stmt.executeQuery("select now()");
            if(res.next()) {
                System.out.println("result: "+res.getString(1)+" next: "+res.next());
            }
            stmt.close();
            conn.close();
            System.out.println("init c3p0 finish!");
        }catch (Exception e) {
            System.err.println("init c3p0 error:"+e.toString());
            e.printStackTrace();
        }
    }
    public static void close(){
        if(cpds != null) {
            cpds.close();
        }
    }
}
