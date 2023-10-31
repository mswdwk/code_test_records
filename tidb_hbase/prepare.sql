create table if not exists tb2(id int primary key,name varchar(64),dt datetime) data_source=hbase,table_mapping=true;

insert into tb2 values (1,'name 1',now()),(2,'name 2',now()),(16,'name 16',now());

show tables;

show create table tb2 \G

select * from tb2;

select * from tb2 where id = 1;
