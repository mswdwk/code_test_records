package org.example;

import com.alibaba.druid.DbType;
import com.alibaba.druid.sql.SQLUtils;
import com.alibaba.druid.sql.ast.SQLStatement;

import java.io.FileReader;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Scanner;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        if(args.length < 1) {
            System.out.println("args len:"+args.length+", args[]:"+args);
            System.out.println("Usage: ./program input_sql_file");
            return;
        }
        String fileName = args[0];
        int sql_count = 0;
        int ignore_sql_count = 0;
        int error_sql_count = 0;
        int total_sql_len = 0;
        List<SQLStatement> stmtList;
        try {
            LocalDateTime start = LocalDateTime.now();
            Scanner sc = new Scanner(new FileReader(fileName));
            while (sc.hasNextLine()) {  //按行读取字符串
                String sql = sc.nextLine().trim();
                if(sql.length() < 3 || sql.startsWith("--")){
                    ignore_sql_count++;
                    continue;
                }
                try {
                    stmtList = SQLUtils.parseStatements(sql, DbType.mysql);
                    sql_count++;
                }catch (Exception e){
                    // System.err.println("caught an parse error:"+e.toString());
                    // e.printStackTrace();
                    error_sql_count++;
                }
                total_sql_len += sql.length();
            }
            LocalDateTime end = LocalDateTime.now();
            Duration dur = Duration.between(start, end);
            if (sql_count == 0)
                return;
            long cost_ms = dur.toMillis();
            long avg_sql_cost_us = dur.toNanos() / 1000 / (error_sql_count+sql_count);
            int avg_sql_len = total_sql_len / (error_sql_count+sql_count);
            System.out.println("    Total cost time: "+cost_ms + " ms.\n"
                              +" Parse ok_sql_count: " + sql_count +"\n"
                              +"    error_sql_count: " + error_sql_count +"\n"
                              +"   ignore_sql_count: " + ignore_sql_count +"\n"
                              +"avg parse cost time: " + avg_sql_cost_us + " us/sql" +"\n"
                              +"        avg_sql_len: " + avg_sql_len + " bytes/sql"
            );
        } catch (Exception e){
            System.err.println("caught an error:"+e.toString());
            e.printStackTrace();
        }
    }
}
