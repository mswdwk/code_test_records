package com.example;

// import com.alibaba.druid.sql.SQLTransformUtils;
import com.alibaba.druid.sql.SQLUtils;
import com.alibaba.druid.sql.ast.SQLExpr;
import com.alibaba.druid.sql.ast.expr.*;
import com.alibaba.druid.sql.dialect.db2.visitor.DB2OutputVisitor;
import com.alibaba.druid.util.FnvHash;
import com.alibaba.druid.util.JdbcConstants;


// OracleToMySqlOutputVisitor
public class DB2ToMySqlOutputVistor extends DB2OutputVisitor {

    public DB2ToMySqlOutputVistor(StringBuilder appender, boolean printPostSemi){
        super(appender, printPostSemi);
    }

    public DB2ToMySqlOutputVistor(StringBuilder appender){
        super(appender);
    }

    public boolean visit(SQLMethodInvokeExpr x) {
        if (x.getMethodName().equalsIgnoreCase("decode")) {
            // SQLMethodInvokeExpr decodeExpr = (SQLMethodInvokeExpr) SQLUtils.toSQLExpr(" decode(a, null, \"h1\",2,\"h2\")", JdbcConstants.DB2);
            // SQLExpr expr = SQLTransformUtils.transformDecode(x);
            SQLExpr expr = null;
            String targetSql = SQLUtils.toSQLString(expr, JdbcConstants.MYSQL);
            //println();
            // System.out.println("[decode]:"+targetSql+" finish");
            print0(targetSql);
            return true;
        }
        return super.visit(x);
    }

    // public boolean visit()
}