package com.example;

import com.alibaba.druid.DbType;
import com.alibaba.druid.sql.SQLTransformUtils;
import com.alibaba.druid.sql.SQLUtils;
import com.alibaba.druid.sql.ast.SQLExpr;
import com.alibaba.druid.sql.ast.SQLStatement;
import com.alibaba.druid.sql.ast.expr.SQLMethodInvokeExpr;
import com.alibaba.druid.sql.dialect.db2.visitor.DB2SchemaStatVisitor;
import com.alibaba.druid.sql.dialect.mysql.visitor.MySqlSchemaStatVisitor;
import com.alibaba.druid.sql.dialect.oracle.visitor.OracleSchemaStatVisitor;
import com.alibaba.druid.sql.dialect.oracle.visitor.OracleToMySqlOutputVisitor;
import com.alibaba.druid.sql.transform.SQLTranform;
import com.alibaba.druid.util.JdbcConstants;

import java.util.List;

import static com.alibaba.druid.sql.SQLUtils.toStatementList;
import static org.junit.Assert.assertEquals;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void t(){
        SQLMethodInvokeExpr decodeExpr = (SQLMethodInvokeExpr) SQLUtils.toSQLExpr("decode(a, null, c, d)", JdbcConstants.ORACLE);
        SQLExpr expr = SQLTransformUtils.transformDecode(decodeExpr);
        String targetSql = SQLUtils.toSQLString(expr, JdbcConstants.MYSQL);
        assertEquals("if(a IS NULL, c, d)", targetSql);
    }

    public static void t2(){
        SQLMethodInvokeExpr decodeExpr = (SQLMethodInvokeExpr) SQLUtils.toSQLExpr(" decode(a, null, \"h1\",2,\"h2\")", JdbcConstants.DB2);
        System.out.println("Method: "+  decodeExpr.getMethodName());
        SQLExpr expr = SQLTransformUtils.transformDecode(decodeExpr);
        String targetSql = SQLUtils.toSQLString(expr, JdbcConstants.MYSQL);
        System.out.println("targetSQL:"+targetSql);
        // assertEquals("if(a IS NULL, c, d)", targetSql);
    }

    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        // String sql = "alter table T_ORG alter column ORG_NAME set not null";
        String sql = "SELECT decode(id,1,'22222',4,'33333333333') FROM test1 WHERE name <= TO_CHAR('2022-04-22', 'yyyyMMdd') FETCH FIRST 2 ROWS ONLY;";

        System.out.println("sql(mysql):"+TranslateDB2SqlToMysql.d2m(sql));
    }
}
