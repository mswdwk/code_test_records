package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.config.ConfigurationSource;
import org.apache.logging.log4j.core.config.Configurator;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;

public class App
{
    private static final Logger log = LogManager.getLogger();
    // Log4j2 will search log4j2.properties itself, so you not need to init Log4j2 yourself.
    public void LogInit(String logConfigFile) throws Exception{
        File logFile = new File(logConfigFile);//"src/main/resources/log4j2.properties"
        BufferedInputStream in = new BufferedInputStream(new FileInputStream(logFile));
        final ConfigurationSource configurationSource = new ConfigurationSource(in);
        Configurator.initialize(null,configurationSource);
    }
    public static void main( String[] args ) throws Exception {
        DbConfig dbConf = new DbConfig("src/main/resources/db.properties");
        log.info("Start Database Flow Query Test");
        DbDataFlowQuery ddfq = new DbDataFlowQuery(dbConf);
        ddfq.FlowQuery(null,null,dbConf.batchSize);
        ddfq.CursorQuery(null,null,dbConf.batchSize);
    }
}
