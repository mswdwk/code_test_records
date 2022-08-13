# 1) user privileges manager


# 2 Database Manager
## create database
 ```create database testdb```
##  list database
 ```LIST DATABASE DIRECTORY```
## use database
```connect to testdb```
## drop database
```sql
drop database testdb
```
# 3 Table Manager
## create table
```create table testtb1(id int ,name varchar(32))
```
## show table define
### by db2look command
db2look -d databaseName -t tableName -e
###  columns
SYSCAT.COLUMNS 
### tables
SYSCAT.TABLES 
## drop table

```drop table testtb```

## alter table
```sql
alter table sales alter column sales set data type smallint
alter table sales alter column region set data type varchar(14)
```
## list table
``` list tables```

# 4 explain sql paln
## db2expln
```shell
 db2expln -database fox -i -g -stmtfile t1.sql -terminator ';' -output t1.exp
```

# 5 sequence
## create seq seq_test
```sql
CREATE SEQUENCE SEQ_TEST AS INT START WITH 1 INCREMENT BY 1 MINVALUE 1 NO MAXVALUE NO CYCLE NO CACHE ORDER;
```
## get next seq value
```
select (nextval for seq_test) from sysibm.sysdummy1;
```
## get current seq value
```
values previous value for seq_test
```
## drop seq
``` drop sequence seq_test```
