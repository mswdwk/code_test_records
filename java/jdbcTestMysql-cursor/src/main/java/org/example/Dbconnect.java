package org.example;

import io.r2dbc.spi.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import reactor.core.publisher.Mono;
import reactor.core.publisher.UnicastProcessor;
import java.sql.*;
import java.sql.Connection;
import java.sql.Statement;
import java.time.Duration;

import static io.r2dbc.spi.ConnectionFactoryOptions.*;

public class Dbconnect {
    private static Logger log = LoggerFactory.getLogger(Dbconnect.class);

    // reference to https://www.jianshu.com/p/4bf56af986a7
    // http://blog.csdn.net/seven_3306/article/details/9303879
    /* 1、当statement设置以下属性时，采用的是流数据接收方式，每次只从服务器接收部份数据，直到所有数据处理完毕，不会发生JVM OOM。
          setResultSetType(ResultSet.TYPE_FORWARD_ONLY);
          setFetchSize(Integer.MIN_VALUE);
        流式查询
        优点：大数据量时不会有OOM问题。
        缺点：占用数据库时间更长，导致网络拥塞的可能性较大。
    * */
    public  void flow_search() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            System.out.println("数据库驱动加载成功");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        try {
            DriverManager.setLoginTimeout(1);
            Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.31:3310/testdb?characterEncoding=UTF-8&connectTimeout=3000", "root", "123");
            // Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3306?characterEncoding=UTF-8","user","123");
            // stmt = con.createStatement();
            ResultSet rs;
            System.out.println("数据库连接成功");
            Statement statement = con.createStatement(
                    ResultSet.TYPE_FORWARD_ONLY,
                    ResultSet.CONCUR_READ_ONLY);
            //设置为流式查询（每次next都取一条数据）
            statement.setFetchSize(Integer.MIN_VALUE);
            long cur_ns = System.nanoTime();
            rs=statement.executeQuery("select * from sbtest1");
            long search_ns = System.nanoTime();
            long cost_ms = ( search_ns - cur_ns) / 1000 / 1000;
            System.out.println("first fetch cost_ms= " + cost_ms);
            StringBuffer sb=null;
            int count = 0;
            int rows = 10000;
            while (rs.next() && count < rows){
                sb=new StringBuffer("");
                sb.append(rs.getString("id")).append("_").append(rs.getString("k")).append("_").append(rs.getString("c"));
                //System.out.println(sb.toString());
                count++;
            }
            long read_data_ns = System.nanoTime() ;
            cost_ms = ( read_data_ns - search_ns) / 1000 / 1000;
            System.out.println("read data("+rows+" rows) from resultset cost_ms= " + cost_ms);
            statement.close();
            long close_st = System.nanoTime() ;
            cost_ms = ( close_st - read_data_ns) / 1000 / 1000;
            System.out.println("close statement cost_ms= " + cost_ms);
            con.close();
            long close_cn = System.nanoTime() ;
            cost_ms = ( close_cn - close_st) / 1000 / 1000;
            System.out.println("close connection cost_ms= " + cost_ms);
        } catch (SQLException e) {
            System.out.println("error:" + e.toString());
            e.printStackTrace();
        }
    }

    /*  游标查询
        优点：大数据量时不会有OOM问题，相比流式查询对数据库单次占用时间较短。
        缺点：相比流式查询，对服务端资源消耗更大，响应时间更长。
    */
    public  void cursor_search() {
        try {
            log.info("Cursor search Demo start");
            Class.forName("com.mysql.jdbc.Driver");
            System.out.println("数据库驱动加载成功");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        try {
            DriverManager.setLoginTimeout(1);
            Connection con = DriverManager.getConnection(
                    "jdbc:mysql://192.168.79.31:3310/testdb?characterEncoding=UTF-8&connectTimeout=3000&useCursorFetch=true",
                    "root", "123");
            // Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3306?characterEncoding=UTF-8","user","123");
            // stmt = con.createStatement();
            ResultSet rs;
            System.out.println("数据库连接成功");
            Statement statement = con.createStatement(
                    ResultSet.TYPE_FORWARD_ONLY,
                    ResultSet.CONCUR_READ_ONLY);
            //设置为流式查询（每次next都取一条数据）
            statement.setFetchSize(1);
            long cur_ns = System.nanoTime();
            rs=statement.executeQuery("select * from sbtest1");
            long search_ns = System.nanoTime();
            long cost_ms = ( search_ns - cur_ns) / 1000 / 1000;
            System.out.println("first fetch cost_ms= " + cost_ms);
            StringBuffer sb=null;
            int count = 0;
            int rows = 10000;
            while (rs.next() && count < rows){
                sb=new StringBuffer("");
                sb.append(rs.getString("id")).append("_").append(rs.getString("k")).append("_").append(rs.getString("c"));
                //System.out.println(sb.toString());
                count++;
            }
            long read_data_ns = System.nanoTime() ;
            cost_ms = ( read_data_ns - search_ns) / 1000 / 1000;
            System.out.println("read data("+rows+" rows) from resultset cost_ms= " + cost_ms);
            statement.close();
            long close_st = System.nanoTime() ;
            cost_ms = ( close_st - read_data_ns) / 1000 / 1000;
            System.out.println("close statement cost_ms= " + cost_ms);
            con.close();
            long close_cn = System.nanoTime() ;
            cost_ms = ( close_cn - close_st) / 1000 / 1000;
            System.out.println("close connection cost_ms= " + cost_ms);
        } catch (SQLException e) {
            System.out.println("error:" + e.toString());
            e.printStackTrace();
        }
    }

    // TODO: NORMAL SEARCH
    // normal_search
    /* 、普通查询
        优点：应用代码简单，数据量较小时操作速度快。
        缺点：数据量大时会出现OOM问题。
    * */
}
