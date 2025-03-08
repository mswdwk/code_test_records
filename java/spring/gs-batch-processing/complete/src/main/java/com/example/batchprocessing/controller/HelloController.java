package com.example.batchprocessing.controller;


import com.example.batchprocessing.Person;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.batch.core.Job;
import org.springframework.batch.core.JobExecution;
import org.springframework.batch.core.JobParameters;
import org.springframework.batch.core.JobParametersInvalidException;
import org.springframework.batch.core.configuration.JobRegistry;
import org.springframework.batch.core.launch.JobLauncher;
import org.springframework.batch.core.repository.JobExecutionAlreadyRunningException;
import org.springframework.batch.core.repository.JobInstanceAlreadyCompleteException;
import org.springframework.batch.core.repository.JobRestartException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.concurrent.atomic.AtomicLong;

@RestController
public class HelloController {
    private static final Logger log = LogManager.getLogger();
    private static final String template = "Hello, %s!";
    private final AtomicLong counter = new AtomicLong();

    @Autowired
    private JobRegistry jobRegistry;

    @Autowired
    private JobLauncher jobLauncher;

    @Autowired
    private Job importPersonJob;

    @GetMapping("/greeting")
    public Person greeting(
            @RequestParam(value = "firstName", defaultValue = "Lee") String firstName,
            @RequestParam(value = "lastName", defaultValue = "lastName") String lastName) {
        return new Person(lastName, String.format(template, firstName));
    }

    @GetMapping("/job1")
    public String job1(
            @RequestParam(value = "jobName", defaultValue = "batch") String jobName) throws JobInstanceAlreadyCompleteException, JobExecutionAlreadyRunningException, JobParametersInvalidException, JobRestartException {
        log.info("jobName "+jobName);
        log.info("Registered Jobs: " + jobRegistry.getJobNames());
        JobExecution je = jobLauncher.run(importPersonJob,new JobParameters());
        String exitCode = je.getExitStatus().getExitCode();
        log.info("run Job exit code: " + exitCode);
        return exitCode;
    }
}
