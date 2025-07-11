package com.example.demo.config;

import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.mybatis.spring.SqlSessionTemplate;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.jdbc.DataSourceBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.jdbc.core.JdbcTemplate;

import javax.sql.DataSource;

@Configuration
@MapperScan(basePackages = "com.example.demo.mapper.manager", sqlSessionFactoryRef = "managerSqlSessionFactory")
public class DataSourceConfig0 {
    private static final Logger log = LogManager.getLogger();

    @Bean("managerDataSource")
    @ConfigurationProperties(prefix = "spring.datasource.manager")
    public DataSource managerDataSource() {
        return DataSourceBuilder.create().build();
    }

    @Bean("managerSqlSessionFactory")
    public SqlSessionFactory managerSqlSessionFactory(
            @Qualifier("managerDataSource") DataSource dataSource) throws Exception {
        SqlSessionFactoryBean sessionFactory = new SqlSessionFactoryBean();
        sessionFactory.setDataSource(dataSource);
        // 指定主数据源的mapper.xml位置
        sessionFactory.setMapperLocations(
                new PathMatchingResourcePatternResolver()
                        .getResources("classpath*:mapper/manager/*.xml") // "classpath*:mapper/*.xml"
        );
        return sessionFactory.getObject();
    }

    @Bean("managerSqlSessionTemplate")
    public SqlSessionTemplate managerSqlSessionTemplate(
            @Qualifier("managerSqlSessionFactory") SqlSessionFactory sqlSessionFactory) {
        return new SqlSessionTemplate(sqlSessionFactory);
    }

    @Bean("managerJdbcTemplate")
    public JdbcTemplate  managerJdbcTemplate(@Qualifier("managerDataSource") DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

//    // 给mybatis用
//    @Bean(name = "sqlSessionFactory")
//    public SqlSessionFactory sqlSessionFactory(DataSource dataSource) throws Exception {
//        SqlSessionFactoryBean sessionFactory = new SqlSessionFactoryBean();
//        sessionFactory.setDataSource(dataSource);
//        return sessionFactory.getObject();
//    }
}