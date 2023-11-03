create table hbtable(id int primary key,name varchar(64),dt datetime) data_source=hbase;

insert into hbtable values (1,'name 1',now()),(2,'name 2',now()),(16,'name 16',now());
insert into hbtable values (100,'name 100',now()),(200,'name 200',now()),(160,'name 160',now());
insert into hbtable values (300,'name 300',now()),(301,'name 300',now()),(360,'name 360',now()),(361,'name 361',now()),(362,'name 362',now());

show tables;

show create table hbtable \G

select * from hbtable;

select * from hbtable where id = 1;
