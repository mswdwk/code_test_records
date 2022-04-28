package com.example;

import com.alibaba.druid.sql.SQLUtils;
import com.alibaba.druid.sql.ast.SQLStatement;
import com.alibaba.druid.sql.dialect.db2.visitor.DB2SchemaStatVisitor;
import com.alibaba.druid.sql.dialect.oracle.visitor.OracleSchemaStatVisitor;
import com.alibaba.druid.util.JdbcConstants;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        // String sql = "alter table T_ORG alter column ORG_NAME set not null";
        // String sql = "SELECT id,value(name,'defname')，to_number(name),chr('x') FROM test1 WHERE name <= TO_CHAR('2022-04-22','yyyyMMdd') FETCH FIRST 2 ROWS ONLY ;";

        String sql = "SELECT decode(id,1,'22222',4,'33333333333') FROM test1 WHERE name <= TO_CHAR('2022-04-22', 'yyyyMMdd') FETCH FIRST 2 ROWS ONLY;";

        SQLStatement stmt = SQLUtils.parseStatements(sql, JdbcConstants.DB2).get(0);
        DB2SchemaStatVisitor visitor = new DB2SchemaStatVisitor();
        stmt.accept(visitor);
        String output = SQLUtils.toSQLString(stmt, JdbcConstants.DB2);
        System.out.println("sql(db2):"+output);
        String mysqlStr = SQLUtils.toMySqlString(stmt);
        System.out.println("\nsql(mysql):"+mysqlStr);
    }
}
