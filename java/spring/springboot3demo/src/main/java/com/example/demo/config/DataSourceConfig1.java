package com.example.demo.config;

import org.apache.ibatis.session.SqlSessionFactory;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.mybatis.spring.SqlSessionTemplate;
import org.mybatis.spring.annotation.MapperScan;
import org.mybatis.spring.mapper.MapperScannerConfigurer;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.jdbc.DataSourceBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;

import javax.sql.DataSource;

// @MapperScan(
//        basePackages = "com.example.mapper.primary",
//        sqlSessionFactoryRef = "primarySqlSessionFactory"
//)
@Configuration
@MapperScan(basePackages = "com.example.demo.mapper.datasource1", sqlSessionFactoryRef = "primarySqlSessionFactory")
public class DataSourceConfig1 { // PrimaryDataSourceConfig
    @Bean("primaryDataSource")
    @ConfigurationProperties(prefix = "spring.datasource.primary")
    public DataSource primaryDataSource() {
        return DataSourceBuilder.create().build();
    }

    @Bean("primarySqlSessionFactory")
    public SqlSessionFactory primarySqlSessionFactory(
            @Qualifier("primaryDataSource") DataSource dataSource) throws Exception {
        SqlSessionFactoryBean sessionFactory = new SqlSessionFactoryBean();
        sessionFactory.setDataSource(dataSource);
        // 指定主数据源的mapper.xml位置
        sessionFactory.setMapperLocations(
                new PathMatchingResourcePatternResolver()
                        .getResources("classpath*:mapper/datasource1/*.xml") // "classpath*:mapper/*.xml"
        );
        return sessionFactory.getObject();
    }

    @Bean("primarySqlSessionTemplate")
    public SqlSessionTemplate primarySqlSessionTemplate(
            @Qualifier("primarySqlSessionFactory") SqlSessionFactory sqlSessionFactory) {
        return new SqlSessionTemplate(sqlSessionFactory);
    }

//    @Bean
//    public MapperScannerConfigurer mapperScannerConfigurer1(){
//        MapperScannerConfigurer msc = new MapperScannerConfigurer();
//        msc.setSqlSessionFactoryBeanName("sqlSessionFactory1");
//        msc.setBasePackage("com.example.demo.mapper.datasource1.UserMapper");
//        return msc;
//    }
}