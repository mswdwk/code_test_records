package org.example;

import java.sql.*;
import java.util.Enumeration;

public class CacheDiffentDbSameTable {
    public static enum UseSchemaMethodType{
        useCataLog,
        useDatabase
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
    static void prepare_query(PreparedStatement pStmt,Statement stmt,Connection con,String dbname,UseSchemaMethodType useSchemaMethod) throws Exception{
        long cur_ns = System.nanoTime();
        switch (useSchemaMethod){
            case useCataLog :
                con.setCatalog(dbname);
                break;
            default:
                stmt.execute("USE "+dbname);
                break;
        }

        pStmt.setInt(1,2);
        ResultSet rs = pStmt.executeQuery();
        // System.out.println("execute sql finish.");
        long cost_ms = (System.nanoTime() - cur_ns) / 1000 / 1000;
        System.out.printf("DBNAME:"+dbname+",cost_ms= " + cost_ms+"\t");
        while(rs.next()) {
            StringBuffer row_str = new StringBuffer("id:");
            int row_id = rs.getRow();
            row_str.append(rs.getInt(1)).append(",name:").append(rs.getString(2));
            System.out.println("row[" + row_id + "]: " + row_str.toString());
        }
        System.out.println("catalog 1 name: "+pStmt.getMetaData().getCatalogName(1));
        System.out.println("catalog name: "+pStmt.getConnection().getCatalog());
        // System.out.println("schema name: "+pStmt.getConnection().getSchema());
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
                id = id + 1;
                System.out.println("Drivers["+id+"]: "+d.getMajorVersion()+"."+d.getMinorVersion());
            }


            String Host = "192.168.79.31";
            String cache_feature = "&useServerPrepStmts=false";
            // Only useServerPrepStmts=false take effect in this condition.
            // useCursorFetch / cachePrepStmts does not take effect.
            // &useCursorFetch=true&cachePrepStmts=false
            // String cache_feature = "&useServerPrepStmts=true";
            Connection con1 = DriverManager.getConnection("jdbc:mysql://"+Host+":8025/testdb?" +
                    "characterEncoding=UTF-8&connectTimeout=3000" + cache_feature, "root", "R1o2o3.");
            Connection con2 = DriverManager.getConnection("jdbc:mysql://"+Host+":3310/testdb?" +
                    "characterEncoding=UTF-8&connectTimeout=3000" + cache_feature, "root", "123456");
            System.out.println("mysql 数据库连接成功");

            String  same_sql= "select * from t1 where id = ?";
            PreparedStatement prepStmt = con2.prepareStatement(same_sql);
            Statement sTmt = con2.createStatement();
            /*System.out.println("MySQL 5.7, Cache Test, use Catalog");
            prepare_query(prepStmt,sTmt,con2,"testdb", UseSchemaMethodType.useCataLog);
            prepare_query(prepStmt,sTmt,con2,"yestdb", UseSchemaMethodType.useCataLog);*/
            System.out.println("MySQL 5.7, Cache Test, use Database");
            prepare_query(prepStmt,sTmt,con2,"testdb", UseSchemaMethodType.useDatabase);
            prepare_query(prepStmt,sTmt,con2,"yestdb", UseSchemaMethodType.useDatabase);
            prepStmt.close();
            sTmt.close();

            prepStmt = con1.prepareStatement(same_sql);
            sTmt = con1.createStatement();
            /*System.out.println("\nMySQL 8.0, Cache Test, use Catalog");
            prepare_query(prepStmt,sTmt,con1,"testdb", UseSchemaMethodType.useCataLog);
            prepare_query(prepStmt,sTmt,con1,"yestdb", UseSchemaMethodType.useCataLog);*/
            System.out.println("\nMySQL 8.0, Cache Test, use Database");
            prepare_query(prepStmt,sTmt,con1,"testdb", UseSchemaMethodType.useDatabase);
            prepare_query(prepStmt,sTmt,con1,"yestdb", UseSchemaMethodType.useDatabase);

            con1.close();
            con2.close();
        } catch (Exception e) {
            System.out.println("error:" + e.toString());
            e.printStackTrace();
        }
    }
}
