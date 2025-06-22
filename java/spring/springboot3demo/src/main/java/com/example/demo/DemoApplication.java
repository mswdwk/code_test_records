package com.example.demo;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;
import org.springframework.context.annotation.ComponentScan;

// @MapperScan(basePackages = "com.example.demo.mapper")
@ComponentScan(basePackages = "com.example.demo")
@SpringBootApplication // (exclude = {DataSourceAutoConfiguration.class}) // , DruidDataSourceAutoConfigure.class
public class DemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
    }

}
