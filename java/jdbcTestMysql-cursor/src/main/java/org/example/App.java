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
    private static final Logger log = LogManager.getLogger(App.class);
    public static void main( String[] args ) throws Exception {
        File logFile = new File("src/main/resources/log4j2.properties");
        BufferedInputStream in = new BufferedInputStream(new FileInputStream(logFile));
        final ConfigurationSource configurationSource = new ConfigurationSource(in);
        Configurator.initialize(null,configurationSource);

        log.info("start connect database");
        Dbconnect dbconnect = new Dbconnect();
        log.info("start flow search test");
        dbconnect.flow_search();
        log.info("start cursor search test");
        dbconnect.cursor_search();
    }
}
