package com.example.batchprocessing;

import javax.sql.DataSource;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
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
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.ClassPathResource;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.jdbc.support.JdbcTransactionManager;

@Configuration
@EnableBatchProcessing(dataSourceRef = "primaryDataSource", transactionManagerRef = "batchTransactionManager")
public class BatchConfiguration {
	private static final Logger log = LogManager.getLogger();

	@Autowired
	@Qualifier("primaryDataSource")
	private DataSource primaryDataSource;

	@Autowired
	@Qualifier("secondaryDataSource")
	private DataSource secondaryDataSource;

	@Bean
	public JdbcTransactionManager batchTransactionManager(DataSource dataSource) {
		log.info("batchTransactionManager");
		return new JdbcTransactionManager(dataSource);
	}


	// tag::readerwriterprocessor[]
	/*@Bean
	public FlatFileItemReader<Person> reader() {
		return new FlatFileItemReaderBuilder<Person>()
			.name("personItemReader")
			.resource(new ClassPathResource("sample-data.csv"))
			.delimited()
			.names("firstName", "lastName")
			.targetType(Person.class)
			.build();
	}*/

	@Bean
	public JdbcCursorItemReader<Person> reader(){
		return new JdbcCursorItemReaderBuilder<Person>()
				.name("personItemReader")
				.dataSource(primaryDataSource)
				.sql("SELECT id, name FROM table1")
				.rowMapper(new BeanPropertyRowMapper<>(Person.class))
				.build()
				;
	}

	@Bean
	public PersonItemProcessor processor() {
		log.info("PersonItemProcessor");
		return new PersonItemProcessor();
	}

	@Bean
	public JdbcBatchItemWriter<Person> writer() {
		return new JdbcBatchItemWriterBuilder<Person>()
			.sql("INSERT INTO people (first_name, last_name) VALUES (:firstName, :lastName)")
			.dataSource(secondaryDataSource)
			.beanMapped()
			.build();
	}
	// end::readerwriterprocessor[]

	// tag::jobstep[]
	@Bean
	public Job importUserJob(JobRepository jobRepository, Step step1, JobCompletionNotificationListener listener) {
		return new JobBuilder("importUserJob", jobRepository)
			.listener(listener)
			.start(step1)
			.build();
	}

	@Bean
	public Step step1(JobRepository jobRepository, DataSourceTransactionManager transactionManager,
					  JdbcCursorItemReader<Person> reader, PersonItemProcessor processor, JdbcBatchItemWriter<Person> writer) {
		return new StepBuilder("step1", jobRepository)
			.<Person, Person>chunk(3, transactionManager)
			.reader(reader)
			.processor(processor)
			.writer(writer)
			.build();
	}

	// end::jobstep[]
}
