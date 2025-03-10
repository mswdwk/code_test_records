package com.example.demo.restservice;

import java.util.concurrent.atomic.AtomicLong;

import com.example.demo.model.Greeting;
import com.example.demo.model.Greeting2;
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

@RestController
public class GreetingController {
    private static final Logger log = LogManager.getLogger();

    @Autowired
    private JobRegistry jobRegistry;

    @Autowired
    private JobLauncher jobLauncher;

    @Autowired
    private Job importPersonJob;

    private static final String template = "Hello, %s!";
    private final AtomicLong counter = new AtomicLong();

    @GetMapping("/greeting")
    public Greeting greeting(@RequestParam(value = "name", defaultValue = "World") String name) {
        return new Greeting(counter.incrementAndGet(), String.format(template, name));
    }

    @GetMapping("/greeting2")
    public Greeting2 greeting2(@RequestParam(value = "name", defaultValue = "World") String name) {
        return new Greeting2(counter.incrementAndGet(), String.format(template, name));
    }

    @GetMapping("/job1")
    public String job1(
            @RequestParam(value = "jobName", defaultValue = "batch") String jobName) throws JobInstanceAlreadyCompleteException, JobExecutionAlreadyRunningException, JobParametersInvalidException, JobRestartException, JobParametersInvalidException, JobRestartException {
        log.info("jobName "+jobName);
        log.info("Registered Jobs: " + jobRegistry.getJobNames());
        JobExecution je = jobLauncher.run(importPersonJob,new JobParameters());
        String exitCode = je.getExitStatus().getExitCode();
        log.info("run Job exit code: " + exitCode);
        return exitCode;
    }
}