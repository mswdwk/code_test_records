package com.example;

import org.junit.Test;

import static org.junit.Assert.*;

public class TranslateDB2SqlToMysqlTest {

    @Test
    public void d2m() {
        String sql = "SELECT decode(id,1,'22222',4,'33333333333') FROM test1 WHERE name <= TO_CHAR('2022-04-22', 'yyyyMMdd') FETCH FIRST 2 ROWS ONLY;";
        String mysql = TranslateDB2SqlToMysql.d2m(sql);
        String targetMysql = "SELECT CASE id\n" +
                "\tWHEN 1 THEN '22222'\n" +
                "\tWHEN 4 THEN '33333333333'\n" +
                "END\n" +
                "FROM test1\n" +
                "WHERE name <= TO_CHAR('2022-04-22', 'yyyyMMdd')\n" +
                "FETCH FIRST 2 ROWS ONLY;";
        assertEquals(mysql,targetMysql);
    }

    @Test
    public void db2sqlToMysql(){
        String sql = "select a from b fetch first 20 rows only";
        String mysql = TranslateDB2SqlToMysql.db2sqlToMysql(sql);
        String targetMysql = "SELECT a\n" +
                "FROM b\n" +
                "LIMIT 20";
        assertEquals(targetMysql,mysql);
    }
    // String sql = "SELECT id,value(name,'defname')，to_number(name),chr('x') FROM test1 WHERE name <= TO_CHAR('2022-04-22','yyyyMMdd') FETCH FIRST 2 ROWS ONLY ;";
}