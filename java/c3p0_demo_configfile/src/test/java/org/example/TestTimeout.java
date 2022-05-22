package org.example;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

public class TestTimeout {
    @Test
    public void testIdleTimeout() {
        Connection conn = null;
        PreparedStatement ps = null;
        conn = C3P0Util.getConnection();
        System.out.println("C3P0: MaxConnectionAge: "+C3P0Util.getDataSource().getMaxConnectionAge());
        System.out.println("C3P0: MaxIdleTime: "+C3P0Util.getDataSource().getMaxIdleTime());
        System.out.println("C3P0: IdleConnectionTestPeriod: "+C3P0Util.getDataSource().getIdleConnectionTestPeriod());
        try {
            int sleep_count = 15;
            ps = conn.prepareStatement("select sleep("+sleep_count+")");
            ps.executeQuery();
            // Thread.sleep(1000*20);
            System.out.println("finish！");
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            System.err.println("error！！！");
        } finally {
            C3P0Util.release(conn, ps, null);
        }
    }
}
