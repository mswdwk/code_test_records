package org.example;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.*;
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
    public void testSql(){
        try {
            Class.forName("com.mysql.jdbc.Driver");  System.out.println("数据库驱动加载成功");
        } catch(ClassNotFoundException e){
            e.printStackTrace();
        }
        try {
            DriverManager.setLoginTimeout(1);

            Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3306/mysql?characterEncoding=UTF-8&connectTimeout=3000","root","Roo123.");
            // Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3306?characterEncoding=UTF-8","user","123");
            // stmt = con.createStatement();
            PreparedStatement stmt = con.prepareStatement("select now()");
            stmt = con.prepareStatement("select sleep(4)");
            ResultSet rs = stmt.executeQuery();
            //con.close();
            rs = stmt.executeQuery();

            Statement st = con.createStatement();
            long cur_ns = System.nanoTime();
            st.execute("select sleep(4)");
            long cost_ms = (System.nanoTime() - cur_ns)/1000/1000;
            System.out.println("cost_ms= "+cost_ms);
            con.close();

            System.out.println("数据库连接成功");
        } catch (SQLException e) {
            System.out.println("error:"+e.toString());
            e.printStackTrace();
        }
    }
}
