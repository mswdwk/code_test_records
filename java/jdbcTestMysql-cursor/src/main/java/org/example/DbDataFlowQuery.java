package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.sql.*;
import java.sql.Connection;
import java.sql.Statement;

public class DbDataFlowQuery {
    private static final Logger log = LogManager.getLogger();
    private String dbFlowUrl = "jdbc:mysql://localhost:8031/testdb?characterEncoding=UTF-8&connectTimeout=3000&useSSL=false";
    private String dbCursorUrl = "jdbc:mysql://localhost:8031/testdb?characterEncoding=UTF-8&connectTimeout=3000&useSSL=false&useCursorFetch=true";
    private String querySql = "select * from sbtest1";
    /*private String dbUrl;
    private String dbHostIp;
    private int dbHostPort;
    private String dbUser = "super";
    private String dbPasswd = "1A3d5g7j";
    private String dbName;*/
    private DbConfig dbc ;
    private Connection con;

    public DbDataFlowQuery(DbConfig dbc) {
        // this(dbc.dbUrl,dbc.dbHostIp,dbc.dbHostPort,dbc.dbUser,dbc.dbPasswd,dbc.dbName);
        this.dbc = dbc;
        this.dbFlowUrl = "jdbc:mysql://"+dbc.dbHostIp+":"+dbc.dbHostPort+"/"+dbc.dbName+dbc.dbUrlOpt;
        this.dbCursorUrl = "jdbc:mysql://"+dbc.dbHostIp+":"+dbc.dbHostPort+"/"+dbc.dbName+dbc.dbUrlOpt
                +"&useCursorFetch=true";
        try {
            Class.forName(dbc.jdbcDriver);
            log.info("数据库驱动 "+dbc.jdbcDriver+" 加载成功 ");
            log.info("Init dbFlowUrl "+dbFlowUrl);
            log.info("Init dbCursorUrl "+dbCursorUrl);
            // this.con = GetDbConn(dbc.dbUrl);
            // log.info("数据库连接成功 "+this.con);
        } catch (ClassNotFoundException e) {
            log.error("Load mysql driver failed: "+e);
            e.printStackTrace();
        }
        log.info("Init Db Data Flow Query");
    }
    public Connection GetDbConn(String dbUrl) throws SQLException {
        DriverManager.setLoginTimeout(1);
        return DriverManager.getConnection(dbUrl, dbc.dbUser, dbc.dbPasswd);
    }
    /*public DbDataFlowQuery(String dbUrl,String dbHostIp,int dbHostPort,String dbUser,String dbPasswd,String dbName){
        this.dbUrl = dbUrl;
        this.dbHostIp = dbHostIp;
        this.dbHostPort = dbHostPort;
        this.dbUser = dbUser;
        this.dbPasswd = dbPasswd;
        this.dbName = dbName;
        log.info("Db data flow query");
    }*/

    // reference to https://www.jianshu.com/p/4bf56af986a7
    // http://blog.csdn.net/seven_3306/article/details/9303879
    /* 1、当statement设置以下属性时，采用的是流数据接收方式，每次只从服务器接收部份数据，直到所有数据处理完毕，不会发生JVM OOM。
          setResultSetType(ResultSet.TYPE_FORWARD_ONLY);
          setFetchSize(Integer.MIN_VALUE);
        流式查询
        优点：大数据量时不会有OOM问题。
        缺点：占用数据库时间更长，导致网络拥塞的可能性较大。
    * */
    public  void FlowQuery(Connection con,String querySql,int rows ) {
        log.info("Flow Query Demo Start");
        try {
            if( null == con)
                con = GetDbConn(dbFlowUrl);
            if (null == querySql || querySql.length() < 2)
                querySql = this.querySql;
            // con.setClientInfo("abc","123");
            ResultSet rs;
            Statement statement = con.createStatement(ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY);
            //设置为流式查询（每次next都取一条数据）
            statement.setFetchSize(Integer.MIN_VALUE);
            // System.exit(0);
            Query(con,statement,querySql,rows);
        } catch (SQLException e) {
            log.error("error:" + e);
            e.printStackTrace();
        }
        log.info("Flow Query Demo Finish");
    }
    /*  游标查询
        优点：大数据量时不会有OOM问题，相比流式查询对数据库单次占用时间较短。
        缺点：相比流式查询，对服务端资源消耗更大，响应时间更长。
    */
    public  void CursorQuery(Connection con,String querySql,int rows ) {
        log.info("Cursor Query Demo Start");
        try {
            if( null == con)
                con = GetDbConn(dbCursorUrl);
            if (null == querySql || querySql.isEmpty())
                querySql = this.querySql;
            Statement statement = con.createStatement(ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY);
            //设置为流式查询（每次next都取一条数据）
            statement.setFetchSize(this.dbc.batchSize);
            Query(con,statement,querySql,rows);
        } catch (SQLException e) {
            log.error("error:" + e.toString());
            e.printStackTrace();
        }
        log.info("Cursor Query Demo Finish");
    }
    public void Query(Connection con,Statement statement,String querySql,int rows) throws SQLException {
        if (null == con || null == statement ) {
            log.fatal("Connection or statement is null ");
            return;
        }
        log.info("Current Statement Fetch Size "+statement.getFetchSize());
        ResultSet rs;
        long cur_ns = System.nanoTime();
        rs = statement.executeQuery(querySql);
        long search_ns = System.nanoTime();
        long cost_ms = ( search_ns - cur_ns) / 1000 / 1000;
        log.info("First fetch cost " + cost_ms+" ms, column count "+rs.getMetaData().getColumnCount());

        StringBuffer sb = null;
        int count = 0;

        while (rs.next() && count < rows){
            sb = new StringBuffer("");
            sb.append(rs.getString("id")).append("_").append(rs.getString("k")).append("_").append(rs.getString("c"));
            //log.info(sb.toString());
            count++;
        }
        long read_data_ns = System.nanoTime() ;
        long read_data_cost_ms = ( read_data_ns - search_ns) / 1000 / 1000;

        statement.close();
        long close_st = System.nanoTime() ;
        long close_statement_cost_ms = ( close_st - read_data_ns) / 1000 / 1000;

        con.close();
        long connection_close_cost_ms = ( System.nanoTime() - close_st) / 1000 / 1000;

        log.info("Read "+rows+" rows from resultSet. cost " + read_data_cost_ms+" ms");
        log.info("Close statement cost " + close_statement_cost_ms+" ms");
        log.info("Close connection cost " + connection_close_cost_ms+" ms");
    }

    // TO GET TOTAL ROW NUMBER
    // Reference https://www.cnblogs.com/hailexuexi/p/14933573.html
    // ATTENTION: Operation not supported for streaming result sets
    public long GetTotalRowCount(ResultSet rs) throws SQLException {
        long start_ns = System.nanoTime() ;
        rs.last(); //移到最后一行
        long move2last_ns = System.nanoTime() ;
        int rowCount = rs.getRow();//得到当前行号，也就是记录数
        rs.beforeFirst(); //如果还要用结果集，就把指针再移到初始化的位置
        long move2first_ns = System.nanoTime() ;
        long move2LastRowCostUs = (move2last_ns - start_ns)/1000;
        long move2FirstRowCostUs = (move2first_ns - move2last_ns)/1000;
        log.info("ResultSet Move to last row cost "+move2LastRowCostUs+" us, move to first row cost "+ move2FirstRowCostUs+" us");
        return rowCount;
    }
    // TODO: NORMAL SEARCH
    // normal_search
    /* 、普通查询
        优点：应用代码简单，数据量较小时操作速度快。
        缺点：数据量大时会出现OOM问题。
    * */
}
