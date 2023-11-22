package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

public class DbConfig {
    Logger log = LogManager.getLogger();
    public String dbUrl;
    public String dbUser;
    public String dbPasswd;
    public String dbHostIp;
    public int dbHostPort;
    public String dbName;
    public String jdbcDriver = "com.mysql.jdbc.Driver";
    public String dbUrlOpt = "";

    public int batchSize = 1;

    private Properties prop = null;
    public DbConfig(String dbConfigFile) {
        if(null == prop)
             prop = new Properties();
        log.info("Init DbConfig through file "+dbConfigFile);
        try {
            // 读取属性文件
            prop.load(new FileInputStream(dbConfigFile));

            // 获取属性值
            this.dbUrl = prop.getProperty("dbUrl");
            this.dbHostIp = prop.getProperty("dbHostIp");
            this.dbHostPort = Integer.parseUnsignedInt( prop.getProperty("dbHostPort").trim());
            this.dbUser = prop.getProperty("dbUser");
            this.dbPasswd = prop.getProperty("dbPasswd");
            this.dbName = prop.getProperty("dbName");
            this.batchSize = Integer.parseUnsignedInt(prop.getProperty("batchSize").trim());
            this.dbUrlOpt = prop.getProperty("dbUrlOpt");

            // 输出属性值
            log.info("dbUrl\t" + dbUrl);
            log.info("dbHostIp\t" + dbHostIp);
            log.info("dbHostPort\t" + dbHostPort);
            log.info("dbUser\t" + dbUser);
            log.info("dbPasswd\t" + dbPasswd);
            log.info("dbName\t" + dbName);
            log.info("batchSize\t" + batchSize);
            log.info("dbUrlOpt\t" + dbUrlOpt);
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
    public void saveConfig(String dbConfigFile) throws IOException{
        // 保存属性到文件
        prop.store(new FileOutputStream(dbConfigFile), null);
        log.info("Save Db Config in file "+dbConfigFile);
    }
    public  void setDbConfig(String key,String value){
        // 修改属性值
        prop.setProperty(key, value);
        log.info("set config item\t "+key+"="+value);
    }

}
