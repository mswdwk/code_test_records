create database if not exists testdb;
create database if not exists yestdb;
drop table if exists testdb.t1;  -- rename ?
drop table if exists yestdb.t1;
create table testdb.t1(id int primary key,name varchar(64) not null default '');
create table yestdb.t1(id int primary key,name varchar(64) not null default '');
insert into testdb.t1 values(1,'name 1'),(2,'name 2');
insert into yestdb.t1 values(2,'name 2,from yestdb');
select * from testdb.t1;
select * from yestdb.t1;
