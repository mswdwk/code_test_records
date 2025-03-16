package com.example.demo.restservice;

import java.util.List;
import java.util.concurrent.atomic.AtomicLong;

import com.example.demo.MybatisDemo;
import com.example.demo.mapper.UserMapper;
import com.example.demo.model.Greeting;
import com.example.demo.model.Greeting2;
import com.example.demo.model.User;
import com.example.demo.page.SamplePage;
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
import org.springframework.context.annotation.Bean;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
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

    @Autowired
    private UserMapper userMapper;

    private MybatisDemo mybatisDemo = new MybatisDemo();
    private SamplePage samplePage = new SamplePage();

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

    @GetMapping("/getUser")
    public User getuser(@RequestParam(value = "id", defaultValue = "1") int id) {
        return mybatisDemo.getUser(id);
    }

    @PostMapping("/addUser")
    public User addUser(@RequestParam(value = "name", defaultValue = "1") String name,@RequestParam(value = "name", defaultValue = "1") String dept) {
        return mybatisDemo.addUser(name,dept);
    }

    @GetMapping("/getpage")
    public List<User> getpage(@RequestParam(value = "id", defaultValue = "1") int id) throws Exception {
        log.info("userMapper "+userMapper);
        return samplePage.demo(userMapper);
    }
}