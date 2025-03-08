package com.example.batchprocessing;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.batch.core.Job;
import org.springframework.batch.core.JobExecution;
import org.springframework.batch.core.JobParameters;
import org.springframework.batch.core.configuration.JobRegistry;
import org.springframework.batch.core.launch.JobLauncher;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ConfigurableApplicationContext;

import java.util.Arrays;

@SpringBootApplication
public class BatchProcessingApplication implements CommandLineRunner {
    // https://spring-doc.cn/spring-batch/5.2.0/step.html
    private static final Logger log = LogManager.getLogger();

    @Autowired
    private JobRegistry jobRegistry;

    @Autowired
    private JobLauncher jobLauncher;

    @Autowired
    private Job importPersonJob;

    @Value("${spring.batch.job.name}")
    private String jobname;

    public static void main(String[] args) throws InterruptedException {
        log.info("main ");
        // System.exit(SpringApplication.exit(SpringApplication.run(BatchProcessingApplication.class, args)));
        ConfigurableApplicationContext ctx = SpringApplication.run(BatchProcessingApplication.class, args);
        Thread.sleep(30_000);
        log.info("sleep finish");
        System.exit(SpringApplication.exit(ctx));
    }

    @Override
    public void run(String... args) throws Exception {
        log.info("cmd , jobname " + jobname);
        log.info("args length: " + args.length);
        log.info("args : {}", args);
        log.info("Registered Jobs: " + jobRegistry.getJobNames());
        JobExecution je = jobLauncher.run(importPersonJob, new JobParameters());
        String exitCode = je.getExitStatus().getExitCode();
        log.info("2 run Job exit code: " + exitCode);
    }
}
