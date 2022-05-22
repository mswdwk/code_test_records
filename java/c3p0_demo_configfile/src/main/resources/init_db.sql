create database if not exists testdb;
use testdb;
create table if not exists users(id int not null auto_increment primary key,username varchar(64) not null default '',
    password varchar(128) not null default '',email  varchar(128) not null default '' ,birthday DATE not null
    )engine=InnoDB charset=utf8mb4;