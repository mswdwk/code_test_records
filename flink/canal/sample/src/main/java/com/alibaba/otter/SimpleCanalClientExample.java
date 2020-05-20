package com.alibaba.otter;

import java.net.InetSocketAddress;
import java.util.List;
import java.util.UUID;
import java.util.ArrayList;

import com.alibaba.otter.canal.client.CanalConnectors;
import com.alibaba.otter.canal.client.CanalConnector;
import com.alibaba.otter.canal.common.utils.AddressUtils;
import com.alibaba.otter.canal.protocol.Message;
import com.alibaba.otter.canal.protocol.CanalEntry;
import com.alibaba.otter.canal.protocol.CanalEntry.Column;
import com.alibaba.otter.canal.protocol.CanalEntry.Entry;
import com.alibaba.otter.canal.protocol.CanalEntry.EntryType;
import com.alibaba.otter.canal.protocol.CanalEntry.EventType;
import com.alibaba.otter.canal.protocol.CanalEntry.RowChange;
import com.alibaba.otter.canal.protocol.CanalEntry.RowData;
import com.google.protobuf.InvalidProtocolBufferException;

public class SimpleCanalClientExample {


public static void main(String args[]) {
    // 创建链接
    CanalConnector connector = CanalConnectors.newSingleConnector(new InetSocketAddress(AddressUtils.getHostIp(),
                                                                                        11111), "example", "", "");
    int batchSize = 1000;
    int emptyCount = 0;
    try {
        connector.connect();
        connector.subscribe(".*\\..*");
        connector.rollback();
        int totalEmptyCount = 120;
        while (emptyCount < totalEmptyCount) {
            Message message = connector.getWithoutAck(batchSize); // 获取指定数量的数据
            long batchId = message.getId();
            int size = message.getEntries().size();
            if (batchId == -1 || size == 0) {
                emptyCount++;
                System.out.println("empty count : " + emptyCount);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                }
            } else {
                emptyCount = 0;
                // System.out.printf("message[batchId=%s,size=%s] \n", batchId, size);
                //printEntry(message.getEntries());
		Analysis(message.getEntries(),emptyCount);
            }

            connector.ack(batchId); // 提交确认
            // connector.rollback(batchId); // 处理失败, 回滚数据
        }

        System.out.println("empty too many times, exit");
    } finally {
        connector.disconnect();
    }
}

	 //TODO 3：将解析出来的数据区分好：增 删 改 发送到kafka
    public static void Analysis(List<CanalEntry.Entry> entries , int emptyCount){
        for(CanalEntry.Entry entry : entries){
            if(entry.getEntryType() == CanalEntry.EntryType.TRANSACTIONBEGIN || entry.getEntryType() == CanalEntry.EntryType.TRANSACTIONEND){
                continue;
            }
 
            try {
                final CanalEntry.RowChange rowChange = CanalEntry.RowChange.parseFrom(entry.getStoreValue());
                final CanalEntry.EventType eventType = rowChange.getEventType();
                final String logfileName = entry.getHeader().getLogfileName();
                final long logfileOffset = entry.getHeader().getLogfileOffset();
                final String dbname = entry.getHeader().getSchemaName();
                final String tableName = entry.getHeader().getTableName();
 
                for(CanalEntry.RowData rowData : rowChange.getRowDatasList()){
                    //区分增删改操作，然后发送给kafka
                    if(eventType == CanalEntry.EventType.DELETE){
                        //删除操作
                        System.out.println("=======删除操作=======");
                        dataDetails(rowData.getAfterColumnsList() , logfileName , logfileOffset , dbname , tableName , emptyCount);
                    }else if (eventType == CanalEntry.EventType.INSERT){
                        //插入操作
                        System.out.println("=======插入操作=======");
                        dataDetails(rowData.getAfterColumnsList() , logfileName , logfileOffset , dbname , tableName , emptyCount);
                    }else {
                        //更改操作
                        System.out.println("=======更改操作=======");
                        dataDetails(rowData.getAfterColumnsList() , logfileName , logfileOffset , dbname , tableName , emptyCount);
                    }
                }
            } catch (InvalidProtocolBufferException e) {
                e.printStackTrace();
            }
        }
    }

	public static void dataDetails(List<CanalEntry.Column> columns , String fileName , Long offset , String DBname , String tableName , int emptyCount){
		List<Object> list = new ArrayList<Object>();
		String sendkey = UUID.randomUUID().toString();
		for(CanalEntry.Column column:columns){
		    List<Object> ll = new ArrayList<Object>();
		    //获取原值
		    ll.add(column.getName());//字段名
		    ll.add(column.getValue());//字段值
		    //是否更改 true代表更改，false代表不更改
		    ll.add(column.getUpdated());
		    list.add(ll);
		} 
		System.out.println(fileName + "#CS#" + offset +"#CS#"+DBname+"#CS#"+tableName+"#CS#"+list+"#CS#"+ emptyCount); 
		//将解析后的数据发送到kafka上 
		MyKafkaProducer.sendMsg("canal" , sendkey , fileName + "#CS#" + offset +"#CS#"+DBname+"#CS#"+tableName+"#CS#"+list+"#CS#"+ emptyCount);
	}

private static void printEntry(List<Entry> entrys) {
    for (Entry entry : entrys) {
        if (entry.getEntryType() == EntryType.TRANSACTIONBEGIN || entry.getEntryType() == EntryType.TRANSACTIONEND) {
            continue;
        }

        RowChange rowChage = null;
        try {
            rowChage = RowChange.parseFrom(entry.getStoreValue());
        } catch (Exception e) {
            throw new RuntimeException("ERROR ## parser of eromanga-event has an error , data:" + entry.toString(),
                                       e);
        }

        EventType eventType = rowChage.getEventType();
        System.out.println(String.format("================&gt; binlog[%s:%s] , name[%s,%s] , eventType : %s",
                                         entry.getHeader().getLogfileName(), entry.getHeader().getLogfileOffset(),
                                         entry.getHeader().getSchemaName(), entry.getHeader().getTableName(),
                                         eventType));

        for (RowData rowData : rowChage.getRowDatasList()) {
            if (eventType == EventType.DELETE) {
                printColumn(rowData.getBeforeColumnsList());
            } else if (eventType == EventType.INSERT) {
                printColumn(rowData.getAfterColumnsList());
            } else {
                System.out.println("-------&gt; before");
                printColumn(rowData.getBeforeColumnsList());
                System.out.println("-------&gt; after");
                printColumn(rowData.getAfterColumnsList());
            }
        }
    }
}
	private static void printColumn(List<Column> columns) {
	    for (Column column : columns) {
		System.out.println(column.getName() + " : " + column.getValue() + "    update=" + column.getUpdated());
	    }
	}

}
