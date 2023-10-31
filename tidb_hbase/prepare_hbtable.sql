create table hbtable(id int primary key,name varchar(64),dt datetime) data_source=hbase;

insert into hbtable values (1,'name 1',now()),(2,'name 2',now()),(16,'name 16',now());

show tables;

show create table hbtable \G

select * from hbtable;

select * from hbtable where id = 1;
