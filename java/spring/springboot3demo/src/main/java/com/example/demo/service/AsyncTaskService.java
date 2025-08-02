package com.example.demo.service;

import lombok.extern.slf4j.Slf4j;
import org.springframework.scheduling.annotation.Async;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

@Service
@Slf4j
public class AsyncTaskService {
    private int count = 0;

    @Async("asyncTaskExecutor")
    public void hi(String a) {
        log.info("async execute my task! " + a);
    }

    @Scheduled(fixedDelay = 5_000, scheduler = "taskScheduler", initialDelay = 2_000)
    public void scheduledTask() {
        count += 1;
        log.info("fixedDelayTask count {} ", count);
        hi(" from scheduledTask ");
    }
}
