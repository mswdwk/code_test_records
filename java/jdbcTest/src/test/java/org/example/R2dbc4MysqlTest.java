package org.example;

import org.junit.Test;

public class R2dbc4MysqlTest {
    @Test
    public void test1() {
        Dbconnect dbconnect = new Dbconnect();
        dbconnect.test_for_jdbc_login_timeout();
    }

    @Test
    public void test2() {
        Dbconnect dbconnect = new Dbconnect();
        dbconnect.test_r2dbc_mysql_2();
    }
}
