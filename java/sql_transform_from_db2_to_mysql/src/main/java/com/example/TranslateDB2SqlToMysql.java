package com.example;

import com.alibaba.druid.DbType;
import com.alibaba.druid.sql.SQLUtils;
import com.alibaba.druid.sql.ast.SQLStatement;
import com.alibaba.druid.sql.dialect.db2.visitor.DB2SchemaStatVisitor;
import com.alibaba.druid.util.JdbcConstants;

import java.util.List;

import static com.alibaba.druid.sql.SQLUtils.toStatementList;

public class TranslateDB2SqlToMysql {
        public static String d2m(String sql) {
                List<SQLStatement> stmtList = toStatementList(sql, DbType.db2);
                StringBuilder out = new StringBuilder();
                DB2ToMySqlOutputVistor visitor = new DB2ToMySqlOutputVistor(out);
                for(int i = 0; i<stmtList.size(); ++i) {
                        stmtList.get(i).accept(visitor);
                }

                String mysqlSql = out.toString();
                return mysqlSql;
        }
        public static String db2sqlToMysql(String sql){
                SQLStatement stmt = SQLUtils.parseStatements(sql, JdbcConstants.DB2).get(0);
                DB2SchemaStatVisitor visitor = new DB2SchemaStatVisitor();
                stmt.accept(visitor);
                String mysqlStr = SQLUtils.toMySqlString(stmt);
                // System.out.println("MYSQLSTR:"+mysqlStr);
                return mysqlStr;
        }
}