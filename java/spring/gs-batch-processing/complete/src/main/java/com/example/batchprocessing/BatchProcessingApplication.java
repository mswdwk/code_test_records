package com.example.batchprocessing;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.batch.core.Job;
import org.springframework.batch.core.JobParameters;
import org.springframework.batch.core.configuration.JobRegistry;
import org.springframework.batch.core.launch.JobLauncher;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

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

	public static void main(String[] args) {
		log.info("main ");
		System.exit(SpringApplication.exit(SpringApplication.run(BatchProcessingApplication.class, args)));
	}

	@Override
	public void run(String... args) throws Exception {
		log.info("Registered Jobs: " + jobRegistry.getJobNames());
		jobLauncher.run(importPersonJob,new JobParameters());
	}
}
