package org.example;

import java.sql.*;
import java.util.Enumeration;

public class CacheDiffentDbSameTable {
    public static enum UseSchemaMethodType{
        useCataLog,
        useDatabase
    }
    static void query(Connection con,String dbname,UseSchemaMethodType useSchemaMethod,String sql) throws Exception {
        prepare_query(con,dbname,useSchemaMethod,sql);
    }
    static void non_prepare_query(Connection con,String dbname,UseSchemaMethodType useSchemaMethod,String sql) throws Exception{
        PreparedStatement pStmt;
        Statement stmt = con.createStatement();
        long cur_ns = System.nanoTime();
        // con.setSchema(dbname); NOT realize, just an Abstract method
        switch (useSchemaMethod) {
            case useCataLog :
                con.setCatalog(dbname);
                break;
            default :
                stmt.execute("USE "+dbname);
                break;
        }
        ResultSet rs = stmt.executeQuery(sql);
        // System.out.println("execute sql finish.");
        long cost_ms = (System.nanoTime() - cur_ns) / 1000 / 1000;
        System.out.println("DBNAME:"+dbname+",cost_ms= " + cost_ms);
        while(rs.next()) {
            StringBuffer row_str = new StringBuffer("id:");
            int row_id = rs.getRow();
            row_str.append(rs.getInt(1)).append(",name:").append(rs.getString(2));
            System.out.println("row[" + row_id + "]: " + row_str.toString());
        }
        rs.close();
    }
    static void prepare_query(Connection con,String dbname,UseSchemaMethodType useSchemaMethod,String sql) throws Exception{
        PreparedStatement pStmt;
        Statement stmt = con.createStatement();
        long cur_ns = System.nanoTime();
        switch (useSchemaMethod){
            case useCataLog :
                con.setCatalog(dbname);
                break;
            default:
                stmt.execute("USE "+dbname);
                break;
        }
        pStmt = con.prepareStatement(sql);
        pStmt.setInt(1,2);
        ResultSet rs = pStmt.executeQuery();
        // System.out.println("execute sql finish.");
        long cost_ms = (System.nanoTime() - cur_ns) / 1000 / 1000;
        System.out.println("DBNAME:"+dbname+",cost_ms= " + cost_ms);
        while(rs.next()) {
            StringBuffer row_str = new StringBuffer("id:");
            int row_id = rs.getRow();
            row_str.append(rs.getInt(1)).append(",name:").append(rs.getString(2));
            System.out.println("row[" + row_id + "]: " + row_str.toString());
        }
        rs.close();
    }
    public static void test_for_jdbc_cache_config() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            System.out.println("数据库驱动加载成功");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        try {
            DriverManager.setLoginTimeout(1);
            Enumeration<Driver> drivers = DriverManager.getDrivers();
            int id = 0;
            while (drivers.hasMoreElements()){
                Driver d = drivers.nextElement();
                id= id+1;
                System.out.println("Drivers["+id+"]: "+d.getMajorVersion()+"."+d.getMinorVersion());
            }


            String cache_feature = "&useServerPrepStmts=true&cachePrepStmts=true";
            // String cache_feature = "&useServerPrepStmts=true";
            Connection con1 = DriverManager.getConnection("jdbc:mysql://192.168.79.132:8025/testdb?" +
                    "characterEncoding=UTF-8&connectTimeout=3000" + cache_feature, "root", "R1o2o3.");
            Connection con2 = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3310/testdb?" +
                    "characterEncoding=UTF-8&connectTimeout=3000" + cache_feature, "root", "123456");
            System.out.println("mysql 数据库连接成功");

            String same_sql = "select * from t1 where id=?";
            System.out.println("MySQL 5.7, Cache Test, use Catalog");
            query(con2,"testdb", UseSchemaMethodType.useCataLog,same_sql);
            query(con2,"yestdb", UseSchemaMethodType.useCataLog,same_sql);
            System.out.println("MySQL 5.7, Cache Test, use Database");
            query(con2,"testdb", UseSchemaMethodType.useDatabase,same_sql);
            query(con2,"yestdb", UseSchemaMethodType.useDatabase,same_sql);

            System.out.println("\nMySQL 8.0, Cache Test, use Catalog");
            query(con1,"testdb", UseSchemaMethodType.useCataLog,same_sql);
            query(con1,"yestdb", UseSchemaMethodType.useCataLog,same_sql);
            System.out.println("\nMySQL 8.0, Cache Test, use Database");
            query(con1,"testdb", UseSchemaMethodType.useDatabase,same_sql);
            query(con1,"yestdb", UseSchemaMethodType.useDatabase,same_sql);

            con1.close();
            con2.close();
        } catch (Exception e) {
            System.out.println("error:" + e.toString());
            e.printStackTrace();
        }
    }
}
