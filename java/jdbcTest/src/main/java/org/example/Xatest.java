package org.example;

import com.mysql.jdbc.ConnectionImpl;
import com.mysql.jdbc.jdbc2.optional.MysqlXAConnection;
import com.mysql.jdbc.jdbc2.optional.MysqlXid;

import javax.sql.XAConnection;
import javax.transaction.xa.XAException;
import javax.transaction.xa.XAResource;
import javax.transaction.xa.Xid;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

/***
 * @Description mysql分布式事务XAConnection模拟
 */

public class Xatest {
    public static void test() throws SQLException {
        final String passwd ="Roo123.";
        //true表示打印XA语句,，用于调试
        boolean logXaCommands = true;
        // 获得资源管理器操作接口实例 RM1
        Connection conn1 = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3310/testdb", "root", passwd);
        XAConnection xaConn1 = new MysqlXAConnection((ConnectionImpl) conn1, logXaCommands);
        XAResource rm1 = xaConn1.getXAResource();

        // 获得资源管理器操作接口实例 RM2
        Connection conn2 = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3310/testdb", "root", passwd);
        XAConnection xaConn2 = new MysqlXAConnection((ConnectionImpl) conn2, logXaCommands);
        XAResource rm2 = xaConn2.getXAResource();
        // AP请求TM执行一个分布式事务，TM生成全局事务id
        String gtid = "g12345";
        byte[] gtrid = gtid.getBytes();
        int formatId = 1;
        try {
            // ==============分别执行RM1和RM2上的事务分支====================
            // TM生成rm1上的事务分支id
            byte[] bqual1 = "b00001".getBytes();
            Xid xid1 = new MysqlXid(gtrid, bqual1, formatId);
            // 执行rm1上的事务分支 One of TMNOFLAGS, TMJOIN, or TMRESUME.
            rm1.start(xid1, XAResource.TMNOFLAGS);
            // 业务1：插入user表
//            PreparedStatement ps1 = conn1.prepareStatement("INSERT into t1 VALUES (102,102,1)");
//            ps1.execute();
            rm1.end(xid1, XAResource.TMSUCCESS);
            System.out.println("xid1: "+xid1.toString());
            // TM生成rm2上的事务分支id
            byte[] bqual2 = "b00002".getBytes();
            Xid xid2 = new MysqlXid(gtrid, bqual2, formatId);
            // 执行rm2上的事务分支
            rm2.start(xid2, XAResource.TMNOFLAGS);
            // 业务2：插入user_msg表
            PreparedStatement ps2 = conn2.prepareStatement("INSERT into t1 VALUES (104,104,1)");
            ps2.execute();
            rm2.end(xid2, XAResource.TMSUCCESS);
            System.out.println("xid2: "+xid2.toString());
            // ===================两阶段提交================================
            // phase1：询问所有的RM 准备提交事务分支
            int rm1Prepare = rm1.prepare(xid1);
            int rm2Prepare = rm2.prepare(xid2);
            // phase2：提交所有事务分支
            boolean onePhase = false;
            //TM判断有2个事务分支，所以不能优化为一阶段提交
            if (rm1Prepare == XAResource.XA_OK
                    && rm2Prepare == XAResource.XA_OK
            ) {
                //所有事务分支都prepare成功，提交所有事务分支
                rm1.commit(xid1, onePhase);
                rm2.commit(xid2, onePhase);
                System.out.println("commit! ");
            } else {
                //如果有事务分支没有成功，则回滚
                rm1.rollback(xid1);
                rm1.rollback(xid2);
            }
        } catch (XAException e) {
            // 如果出现异常，也要进行回滚
            System.err.println("XA error: " + e.toString());
            e.printStackTrace();
        }
    }
}