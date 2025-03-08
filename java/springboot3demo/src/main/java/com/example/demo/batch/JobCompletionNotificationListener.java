package com.example.demo.batch;

import com.example.demo.model.Person;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.batch.core.BatchStatus;
import org.springframework.batch.core.JobExecution;
import org.springframework.batch.core.JobExecutionListener;
import org.springframework.jdbc.core.DataClassRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;

@Component
public class JobCompletionNotificationListener implements JobExecutionListener {
    //private static final Logger log = LoggerFactory.getLogger(JobCompletionNotificationListener.class);
    private static final Logger log = LogManager.getLogger();
    private final JdbcTemplate jdbcTemplate;

    public JobCompletionNotificationListener(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }

    @Override
    public void afterJob(JobExecution jobExecution) {
        if (jobExecution.getStatus() == BatchStatus.COMPLETED) {

            log.info("!!! JOB FINISHED! Time to verify the results");

            jdbcTemplate
                    .query("SELECT first_name, last_name FROM people", new DataClassRowMapper<>(Person.class))
                    .forEach(person -> log.info("Found <{}> in the database.", person));
        }
    }
}