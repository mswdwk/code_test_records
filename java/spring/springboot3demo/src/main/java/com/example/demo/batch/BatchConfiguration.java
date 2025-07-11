package com.example.demo.batch;

import com.example.demo.model.Person2;
import com.example.demo.processor.PersonItemProcessor;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.batch.core.launch.support.RunIdIncrementer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.ClassPathResource;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.jdbc.support.JdbcTransactionManager;

import javax.sql.DataSource;
import org.springframework.batch.core.Job;
import org.springframework.batch.core.Step;
import org.springframework.batch.core.configuration.annotation.EnableBatchProcessing;
import org.springframework.batch.core.job.builder.JobBuilder;
import org.springframework.batch.core.repository.JobRepository;
import org.springframework.batch.core.step.builder.StepBuilder;
import org.springframework.batch.item.database.JdbcBatchItemWriter;
import org.springframework.batch.item.database.JdbcCursorItemReader;
import org.springframework.batch.item.database.builder.JdbcBatchItemWriterBuilder;
import org.springframework.batch.item.database.builder.JdbcCursorItemReaderBuilder;
import org.springframework.batch.item.file.FlatFileItemReader;
import org.springframework.batch.item.file.builder.FlatFileItemReaderBuilder;

@Configuration
@EnableBatchProcessing(dataSourceRef = "managerDataSource", transactionManagerRef = "batchTransactionManager",
        databaseType = "mysql",maxVarCharLength = 10000)
public class BatchConfiguration {
    private static final Logger log = LogManager.getLogger();

    @Autowired
    @Qualifier("primaryDataSource")
    private DataSource primaryDataSource;

    @Autowired
    @Qualifier("secondaryDataSource")
    private DataSource secondaryDataSource;

    @Autowired
    @Qualifier("managerDataSource")
    private DataSource managerDataSource;

    @Bean
    public JdbcTransactionManager batchTransactionManager(@Qualifier("managerDataSource") DataSource dataSource) {
        log.info("batchTransactionManager");
        return new JdbcTransactionManager(dataSource);
    }


    // tag::readerwriterprocessor[]
    @Bean
    public FlatFileItemReader<Person2> fileReader() {
        return new FlatFileItemReaderBuilder<Person2>()
                .name("personItemReader")
                .resource(new ClassPathResource("sample-data.csv"))
                .delimited()
                .names("firstName", "lastName")
                .targetType(Person2.class)
                .build();
    }

    @Bean
    public JdbcCursorItemReader<Person2> jdbcCursorItemReader(){
        return new JdbcCursorItemReaderBuilder<Person2>()
                .name("personItemReader")
                .dataSource(primaryDataSource)
                .sql("SELECT first_name as firstName,last_name as lastName FROM people")
                .rowMapper(new BeanPropertyRowMapper<>(Person2.class))
                .build();
    }

    @Bean
    public PersonItemProcessor processor() {
        log.info("PersonItemProcessor");
        return new PersonItemProcessor();
    }

    @Bean
    public JdbcBatchItemWriter<Person2> writer() {
        return new JdbcBatchItemWriterBuilder<Person2>()
                .sql("INSERT INTO people2 (first_name, last_name) VALUES (:firstName, :lastName)")
                .dataSource(secondaryDataSource)
                .beanMapped()
                .build();
    }

    // end::readerwriterprocessor[]

    // tag::jobstep[]
    @Bean
    public Job importUserJob(JobRepository jobRepository, Step step2, JobCompletionNotificationListener listener) {
        return new JobBuilder("importUserJob", jobRepository)
                .incrementer(new RunIdIncrementer())
                .listener(listener)
                .start(step2)
                .build();
    }

    @Bean
    public Step step1(JobRepository jobRepository, DataSourceTransactionManager transactionManager,
                      FlatFileItemReader<Person2> reader, PersonItemProcessor processor, JdbcBatchItemWriter<Person2> writer) {
        return new StepBuilder("step1", jobRepository)
                .<Person2, Person2>chunk(3, transactionManager)
                .reader(reader)
                .processor(processor)
                .writer(writer)
                .build();
    }

    @Bean
    public Step step2(JobRepository jobRepository, DataSourceTransactionManager transactionManager,
                      JdbcCursorItemReader<Person2> jdbcCursorItemReader, PersonItemProcessor processor, JdbcBatchItemWriter<Person2> writer) {
        return new StepBuilder("step2", jobRepository)
                .<Person2, Person2>chunk(3, transactionManager)
                .reader(jdbcCursorItemReader)
                .processor(processor)
                .writer(writer)
                .build();
    }

	/*@Bean
	public JobLauncher jobLauncher(JobRepository jobRepository) throws Exception {
		TaskExecutorJobLauncher jobLauncher = new TaskExecutorJobLauncher();
		jobLauncher.setJobRepository(jobRepository);
		jobLauncher.afterPropertiesSet();
		return jobLauncher;
	}

	@Bean
	public ThreadPoolTaskExecutor taskExecutor() {
		ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
		executor.setCorePoolSize(5);  // 核心线程数
		executor.setMaxPoolSize(10);  // 最大线程数
		executor.setQueueCapacity(20);  // 等待队列容量
		executor.setThreadNamePrefix("Batch-");  // 线程名称前缀
		return executor;
	}*/

    // end::jobstep[]
}
