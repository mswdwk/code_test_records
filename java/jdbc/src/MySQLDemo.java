/*public class Main {
    public static void main(String[] args) {
        System.out.println("Hello World!");
    }
}*/
// package com.runoob.test;

import java.sql.*;

public class MySQLDemo {

    // MySQL 8.0 以下版本 - JDBC 驱动名及数据库 URL
     static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";
     static final String DB_URL = "jdbc:mysql://192.168.32.133:3306/mysql";

    // MySQL 8.0 以上版本 - JDBC 驱动名及数据库 URL

  /* static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
    static final String DB_URL = "jdbc:mysql://localhost:3306/mysql?useSSL=false&serverTimezone=UTC";
    static final String DB_URL2= DB_URL+"&allowPublicKeyRetrieval=true";
*/
    // 数据库的用户名与密码，需要根据自己的设置
    static final String USER = "root";
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
            sql = "SELECT user,host, plugin, authentication_string FROM user";
            ResultSet rs = stmt.executeQuery(sql);
            conn.setAutoCommit(true);
            String sql2 = "select a from t1 where b = ?";
            ResultSet res;
            PreparedStatement pstm;
            String db = "db1";

            conn.createStatement().execute("use "+db);
            pstm = conn.prepareStatement(sql2);

            pstm.setString(1,db);
            res = pstm.executeQuery();
            while(res.next())
                System.out.println( db+":"+ res.getInt(1));
            //pstm.clearParameters();
            pstm.close();
            res.close();


            db = "db2";
            conn.createStatement().execute("use "+db);
            pstm = conn.prepareStatement(sql2);
            pstm.setString(1,db);
            res = pstm.executeQuery();
            while(res.next())
                System.out.println( db+":"+ res.getInt(1));
            pstm.clearParameters();

            // 展开结果集数据库
            /*while(rs.next()){
                // 通过字段检索
                // int id  = rs.getInt("id");
                String user = rs.getString("user");
                String host = rs.getString("host");
                String authentication_string = rs.getString("authentication_string");

                // 输出数据
                // System.out.print("ID: " + id);
                System.out.print(", 用户名称: " + user);
                System.out.print(", 用户主机: " + host);
                System.out.print(", 用户认证字符: " + authentication_string);
                System.out.print("\n");
            }*/
            // 完成后关闭
            rs.close();
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