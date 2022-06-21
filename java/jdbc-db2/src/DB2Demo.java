/*public class Main {
    public static void main(String[] args) {
        System.out.println("Hello World!");
    }
}*/
// package com.runoob.test;

import java.sql.*;

public class DB2Demo {

    // MySQL 8.0 以下版本 - JDBC 驱动名及数据库 URL
     static final String JDBC_DRIVER = "com.ibm.db2.jcc.DB2Driver";
     static final String DB_URL = "jdbc:db2://192.168.79.132:50000/testdb";

    // 数据库的用户名与密码，需要根据自己的设置
    static final String USER = "db2inst1";
    static final String PASS = "123";

    public static void main(String[] args) {
        Connection conn = null;
        Statement stmt = null;
        try{
            // 注册 JDBC 驱动
            Class.forName(JDBC_DRIVER);

            // 打开链接
            System.out.println("连接数据库...");
            conn = DriverManager.getConnection(DB_URL,USER,PASS);
            System.out.println( "jdbc driver version:"+
            conn.getMetaData().getDriverVersion());
            System.out.println( "database version:"+
            conn.getMetaData().getDatabaseProductVersion());

            // 执行查询
            System.out.println(" 实例化Statement对象...");
            stmt = conn.createStatement();
            String sql;
            conn.setAutoCommit(true);
            String sql2 = "select * from t2 where id = ?";
            ResultSet res;
            PreparedStatement pstm;
            String db = "1";

            // conn.createStatement().execute("use "+db);
            pstm = conn.prepareStatement(sql2);

            pstm.setString(1,db);
            res = pstm.executeQuery();
            while(res.next())
                System.out.println( db+":"+ res.getInt(1));
            //pstm.clearParameters();

            // 完成后关闭
            pstm.close();
            stmt.close();
            conn.close();
        }catch(SQLException se){
            // 处理 JDBC 错误
            se.printStackTrace();
        }catch(Exception e){
            // 处理 Class.forName 错误
            e.printStackTrace();
        }finally{
            // 关闭资源
            try{
                if(stmt!=null) stmt.close();
            }catch(SQLException se2){
            }// 什么都不做
            try{
                if(conn!=null) conn.close();
            }catch(SQLException se){
                se.printStackTrace();
            }
        }
        System.out.println("Goodbye!");
    }
}