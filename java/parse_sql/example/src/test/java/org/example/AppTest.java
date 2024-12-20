package org.example;

import com.alibaba.druid.DbType;
import com.alibaba.druid.sql.SQLUtils;
import com.alibaba.druid.sql.ast.SQLStatement;
import com.alibaba.druid.sql.parser.SQLStatementParser;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import java.util.List;

/**
 * Unit test for simple App.
 */
public class AppTest 
    extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public AppTest( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( AppTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testApp() throws Exception
    {
        // assertTrue( true );
        String sql = "SELECT 'abc',a,bc,d FROM user u left join post p on u.id=p.id and u.name=p.name WHERE u.id = 1 and p.b>'123' or u.c in ('1','abc') ";
        SQLStatementParser parser = new SQLStatementParser(sql);
        List<SQLStatement> statements = parser.parseStatementList(); // 解析SQL语句


        for (SQLStatement stmt : statements) {
            // 输出格式化的SQL
            String output = SQLUtils.toSQLString(stmt, DbType.mysql);
            // 输出SQL摘要，去除其中的条件变量
            System.out.println("sql degest= "+stmt.toParameterizedString());
        }
    }
}
