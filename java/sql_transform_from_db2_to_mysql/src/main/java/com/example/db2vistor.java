package com.example;

import com.alibaba.druid.sql.ast.SQLExpr;
import com.alibaba.druid.sql.ast.statement.SQLExprTableSource;
import com.alibaba.druid.sql.ast.statement.SQLTableSource;
import com.alibaba.druid.sql.dialect.db2.ast.stmt.DB2SelectQueryBlock;
import com.alibaba.druid.sql.dialect.db2.visitor.DB2ASTVisitorAdapter;

import java.util.*;

public class db2vistor  extends DB2ASTVisitorAdapter {
    private List<SQLExpr> exprs = new ArrayList<SQLExpr>();
    public boolean visit(SQLExpr x) {
        exprs.add(x);
        return true;
    }

    public List<SQLExpr> getAllexpr() {
        return exprs;
    }
}