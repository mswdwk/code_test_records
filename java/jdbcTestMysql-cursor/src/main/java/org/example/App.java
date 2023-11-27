package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.config.ConfigurationSource;
import org.apache.logging.log4j.core.config.Configurator;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;

import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.CommandLine;

public class App
{
    private static final Logger log = LogManager.getLogger();
    // Log4j2 will search log4j2.properties itself, so you not need to init Log4j2 yourself.
    public void LogInit(String logConfigFile) throws Exception {
        File logFile = new File(logConfigFile);//"src/main/resources/log4j2.properties"
        BufferedInputStream in = new BufferedInputStream(new FileInputStream(logFile));
        final ConfigurationSource configurationSource = new ConfigurationSource(in);
        Configurator.initialize(null, configurationSource);
    }
    // https://www.cnblogs.com/ontway/p/7455355.html
    private static void parseArgs(String[] args) throws Exception{
        CommandLineParser parser = new BasicParser( );
        Options options = new Options( );
        options.addOption("h", "help", false, "Print this usage information");
        options.addOption("i", "input", false, "input sql file name" );
        options.addOption("o", "output", true, "File to save program output to");
        // Parse the program arguments
        CommandLine commandLine = parser.parse( options, args );
        // Set the appropriate variables based on supplied options
        boolean verbose = false;
        String file = "";

        if( commandLine.hasOption('h') ) {
            System.out.println( "Help Message");
            System.exit(0);
        }
        if( commandLine.hasOption('v') ) {
            verbose = true;
        }
        if( commandLine.hasOption('f') ) {
            file = commandLine.getOptionValue('f');
        }
    }
    public static void main( String[] args ) throws Exception {
        parseArgs(args);
        DbConfig dbc = new DbConfig("src/main/resources/db.properties");
        log.info("start connect database");
        DbDataFlowQuery dbconnect = new DbDataFlowQuery(dbc);
        log.info("start flow search test");
        dbconnect.FlowQuery(null,null,dbc.batchSize);
        log.info("start cursor search test");
        dbconnect.CursorQuery(null,null,dbc.batchSize);
    }
}
