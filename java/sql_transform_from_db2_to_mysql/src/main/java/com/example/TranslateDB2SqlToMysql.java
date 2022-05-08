package com.example;

import com.alibaba.druid.DbType;
import com.alibaba.druid.sql.ast.SQLStatement;

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
}