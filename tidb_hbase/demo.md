# One. create no mapping table
## 1) create table 
create table hbtable(id int primary key,name varchar(64),dt datetime,act decimal(17,2)) data_source=hbase;
## 2）insert 
insert into hbtable values (1,'name 1',now(),1001.01),(2,'name 2',now(),1002.02),(16,'name 16',now(),1003.03);
insert into hbtable values (100,'name 100',now(),1100.99),(200,'name 200',now(),1200.99),(160,'name 160',now(),1160.99);
insert into hbtable values (300,'name 300',now(),1300),(301,'name 300',now(),1301.99),(360,'name 360',now(),1360.99),(361,'name 361',now(),1361.99);

## 3）select 
select * from hbtable;
select * from hbtable where id = 1;
select * from hbtable where id > 101 and id < 210;
select count(*) from hbtable where id > 101 and id < 210;
select max(id) from hbtable where id > 101 and id < 210 group by name;
select sum(id) from hbtable where id> 10 and id < 1000;

## 4） delete
delete from hbtable where id=1
delete from hbtable where id=301

# Two. create mapping table
## 1）建表
create table hbt1(id int primary key,name varchar(64),dt datetime,act decimal(17,2)) data_source=hbase table_mapping=true;
## 2）插入数据
## 3）查询数据
## 4）删除数据

# Three. operate cross different data source 

## 1）union
 select  id,name,act,dt from tktb union select id,name,act,dt from hbtable

## 2）join

select hbt1.id,hbt1.name,hbtable.act htb1 left join hbtable on hbt1.id=hbtable.id 

# Four. View （optional）
## 1) create view 
create view v1 if  dt > 20231010 then select * from  htb1 else select * from hbtable;

## 2) select
select * from v1;

select * from v1 where id = 100;


select * from v1 where id > 101 and id <=1000;
