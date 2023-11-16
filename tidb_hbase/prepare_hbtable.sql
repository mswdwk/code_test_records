create table hbtable(id int primary key,name varchar(64),dt datetime,act decimal(17,2)) data_source=hbase;

insert into hbtable values (1,'name 1',now(),1001.01),(2,'name 2',now(),1002.02),(16,'name 16',now(),1003.03);
insert into hbtable values (100,'name 100',now(),1100.99),(200,'name 200',now(),1200.99),(160,'name 160',now(),1160.99);
insert into hbtable values (300,'name 300',now(),1300),(301,'name 300',now(),1301.99),(360,'name 360',now(),1360.99),(361,'name 361',now(),1361.99);

show tables;

show create table hbtable \G

select * from hbtable;

select * from hbtable where id = 1;

select * from hbtable where id = 100;

select * from hbtable where id > 101 and id < 210;
