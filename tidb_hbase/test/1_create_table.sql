create database if not exists test;
use test;
create table if not exists test1(id bigint unsigned  primary key,name varchar(64),note text,act double,act2 decimal(17,2),updatedate datetime);
insert into test1 value (1,'name 1','text 1',12345678.341,123456789012345.01,'2023-10-09 12:34:51');
insert into test1 values(10,'name 10','text 10',12345678.342,123456789012345.02,'2023-10-09 12:34:52'),(123,'name 123','text 123',12345678.123,123456789012341.23,'2023-10-09 12:31:23');
