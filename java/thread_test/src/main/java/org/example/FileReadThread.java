package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileInputStream;
import java.io.IOException;
/*
【Java学习】 File类--文件操作
https://mp.weixin.qq.com/s?src=11&timestamp=1701093125&ver=4922&signature=R78Sg6wsmLyZBUXywdzj6JxujSHt2iiCyc-Hx-62NXZfYP9j-*UQuKGCsb2tuH44O*t2BYDE61bTi3B2lnAezoZ2aiWcW8-WVLXISSF7-3hqTDYPocG3AMszspjIeyB-&new=1
* */
public class FileReadThread implements Runnable {
    private static Logger log = LogManager.getLogger();
    public String threadName = "";
    private String fileName = "";
    private int lineSize = 1024;
    public Thread t = null;
    FileReadThread(String threadName,String fileName,int lineSize){
        this.threadName = threadName;
        this.fileName = fileName;
        this.lineSize = lineSize;
        log.info("Init Thread "+threadName);
    }
    public void run (){
        if(fileName  == null || fileName.isEmpty() ) {
            log.fatal("input file name is null or empty");
            return;
        }
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(fileName);
            byte[] buffer = new byte[lineSize];
            int length;
            while ((length = fis.read(buffer)) != -1) {
                System.out.println(new String(buffer, 0, length)); // 将读取到的字节数组转为字符串输出
            }
            log.info("文件读取完成！");
        } catch (IOException e) {
            log.error("文件读取失败：" + e.getMessage());
        } finally {
            if (fis != null) {
                try {
                    fis.close();
                } catch (IOException e) {
                    log.error("close file got ioexception:"+e);
                    e.printStackTrace();
                }
            }
        }
    }
    public void Start () {
        System.out.println("Starting Thread: " +  threadName );
        if (t == null) {
            t = new Thread (this, threadName);
            t.start ();
        }
    }
}
