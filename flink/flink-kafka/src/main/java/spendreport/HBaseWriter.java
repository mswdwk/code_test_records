//package com.flink;
package spendreport;
 
//import spendreport.DeviceData;
import org.apache.commons.dbcp2.BasicDataSource;
//import org.apache.commons.dbcp.BasicDataSource;
import org.apache.flink.configuration.Configuration;
import org.apache.flink.streaming.api.functions.sink.RichSinkFunction;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.*;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;
 
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
 
/**
 *
 * 写入HBase
 * 继承RichSinkFunction重写父类方法
 *
 * 写入hbase时500条flush一次, 批量插入, 使用的是writeBufferSize
 */
class HBaseWriter extends RichSinkFunction<DeviceData>{
    private static final Logger log = LogManager.getLogger();
 
    private static org.apache.hadoop.conf.Configuration configuration;
    private static Connection connection = null;
    private static BufferedMutator mutator;
    private static int count = 0;
 
    @Override
    public void open(Configuration parameters) throws Exception {
        configuration = HBaseConfiguration.create();
        configuration.set("hbase.master", "192.168.3.101:60020");
        configuration.set("hbase.zookeeper.quorum", "192.168.3.101");
        configuration.set("hbase.zookeeper.property.clientPort", "2181");
        try {
            connection = ConnectionFactory.createConnection(configuration);
            log.info("create flink connection "+connection);
        } catch (IOException e) {
            e.printStackTrace();
        }
        BufferedMutatorParams params = new BufferedMutatorParams(TableName.valueOf("t1"));
        params.writeBufferSize(2 * 1024 * 1024);
        mutator = connection.getBufferedMutator(params);
    }
 
    @Override
    public void close() throws IOException {
        if (mutator != null) {
            mutator.close();
        }
        if (connection != null) {
            connection.close();
        }
    }
 
    @Override
    public void invoke(DeviceData values, Context context) throws Exception {
        //Date 1970-01-06 11:45:55  to 445555000
        long unixTimestamp= 0;
        try {
            String gatherTime = values.GatherTime;
            //毫秒和秒分开处理
            if (gatherTime.length() > 20) {
                long ms = Long.parseLong(gatherTime.substring(20, 23));
                Date date = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(gatherTime);
                unixTimestamp = date.getTime() + ms;
            } else {
                Date date = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(gatherTime);
                unixTimestamp = date.getTime();
            }
        } catch (ParseException e) {
            e.printStackTrace();
        }
        String RowKey = values.MatchID + String.valueOf(unixTimestamp);
        String Key = values.OperationValue;
        String Value = values.OperationData;
        System.out.println("Column Family=f1,  RowKey=" + RowKey + ", Key=" + Key + " ,Value=" + Value);
        Put put = new Put(RowKey.getBytes());
        put.addColumn("f1".getBytes(), Key.getBytes(), Value.getBytes());
        mutator.mutate(put);
        //每满500条刷新一下数据
        if (count >= 500){
            mutator.flush();
            count = 0;
        }
        count = count + 1;
    }
}
