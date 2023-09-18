package org.example;


import org.apache.commons.configuration.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.Connection;
import org.apache.hadoop.hbase.client.ConnectionFactory;

import java.io.IOException;
import java.util.Properties;

import org.apache.hadoop.hbase.Cell;
import org.apache.hadoop.hbase.CellScanner;
import org.apache.hadoop.hbase.CellUtil;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.filter.Filter;
import org.apache.hadoop.hbase.filter.PrefixFilter;
import org.apache.hadoop.hbase.util.Bytes;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * Hello world!
 *
 */
public class App 
{
    public static Table getTable(String str){
        Table table = null;
        try {
            // 创建一个链接
            Connection connection = ConnectionFactory.createConnection();
            // 获取数据表
            table = connection.getTable(TableName.valueOf(str));
        } catch (Exception e){
            e.printStackTrace();
        }
        return table;

    }

    /**
     * 根据rowkey获取数据
     * @param tablename
     * @param rowkey
     */
    public static void getDataRow(String tablename,String rowkey){
        Table table = null;
        try {
            table = getTable(tablename);
            // get 'student','row1'
            Get get = new Get(Bytes.toBytes(rowkey));
            get.readVersions(1);
            Result result = table.get(get);

            // 返回每一列的全集
            CellScanner cellScanner = result.cellScanner();
            // 获取每一列数据的信息
            while (cellScanner.advance()){
                Cell cell = cellScanner.current();

                System.out.println("rowkey="+Bytes.toString(CellUtil.cloneRow(cell)) // rowkey
                        + " " +Bytes.toString(CellUtil.cloneFamily(cell)) // 列簇名
                        + ":" +Bytes.toString(CellUtil.cloneQualifier(cell)) // 列名
                        + "=" +Bytes.toString(CellUtil.cloneValue(cell)) // 列值
                );
            }

            // 获取每一列数据的信息(写法二)
            for (Cell cell:result.rawCells()){
                System.out.println("rowkey="+Bytes.toString(CellUtil.cloneRow(cell)) // rowkey
                        + " " +Bytes.toString(CellUtil.cloneFamily(cell)) // 列簇名
                        + ":" +Bytes.toString(CellUtil.cloneQualifier(cell)) // 列名
                        + "=" +Bytes.toString(CellUtil.cloneValue(cell)) // 列值
                );
            }

        } catch (Exception e){
            e.printStackTrace();
        } finally {
            if (table != null){
                try {
                    table.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * HBase写入数据
     * @param tablename
     * @param rowkey
     * @param value
     */
    public static void putData(String tablename,String rowkey,Map<String,Object> value){
        Table table = getTable(tablename);

        // put 'stu','002','info:username','ben'
        Put put = new Put(Bytes.toBytes(rowkey));
        for (String key: value.keySet()){
            put.addColumn(Bytes.toBytes(HBASE_CF_INFO), // 列簇名
                    Bytes.toBytes(key), // 列名
                    Bytes.toBytes(value.get(key).toString())); // 列值
        }
        try {
            table.put(put);
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    /**
     * HBase删除数据
     * @param tablename
     * @param rowkey
     */
    public static void deleteData(String tablename,String rowkey){
        Table table = getTable(tablename);

        // delete 'stu','002','info:addres'
        Delete delete = new Delete(Bytes.toBytes(rowkey));
        delete.addColumn(Bytes.toBytes(HBASE_CF_INFO),Bytes.toBytes("addres"));

        try {
            table.delete(delete);
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    /**
     * HBase扫描数据
     * @param tablename
     */
    public static void getScanData(String tablename){
        Table table = getTable(tablename);

        Scan scan = new Scan();

        //scan.setStartRow()  :  scan.setStopRow()
        //file过滤，前置匹配
        Filter filter = new PrefixFilter(Bytes.toBytes("001"));
        scan.setFilter(filter);
        //分页 new PageFilter();

        ResultScanner resultScanner = null;
        try {
            resultScanner = table.getScanner(scan);
            // 获取每一行的数据
            for (Result result : resultScanner){
                // 获取每一行中每一列数据的信息
                for (Cell cell:result.rawCells()){
                    System.out.println("rowkey="+Bytes.toString(CellUtil.cloneRow(cell)) // rowkey
                            + " " +Bytes.toString(CellUtil.cloneFamily(cell)) // 列簇名
                            + ":" +Bytes.toString(CellUtil.cloneQualifier(cell)) // 列名
                            + "=" +Bytes.toString(CellUtil.cloneValue(cell)) // 列值
                    );
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static String HBASE_TABLE = "student";
    public static String HBASE_CF_INFO = "info";
    public static void main(String[] args) {
        // 要写入HBase的数据
        Map<String,Object> map = new HashMap<String, Object>();
        map.put("addres","beijing");
        map.put("age","32");
        map.put("username","jack");

        // 根据rowkey获取数据
        getDataRow(HBASE_TABLE,"row1");
        // HBase写入数据
        // putData(HBASE_TABLE,"003",map);
        // HBase删除数据
        // deleteData(HBASE_TABLE,"002");
        // HBase扫描数据
        // getScanData(HBASE_TABLE);
    }
}
