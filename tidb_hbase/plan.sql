use test;
set @@tidb_analyze_version = 1;
create table if not exists hb1 (id int primary key, name varchar(64));
create table if not exists tb1 (id int primary key, name varchar(64));
insert into tb1 values (10,'name 10'),(30,'name 30'),(60,'name 60'),(90,'name 90');
insert into hb1 values (100,'name 100'), (150,'name 150'),(200,'name 200'),(300,'name 300');
create view v1 if id >= 100 then select * from hb1 else select * from tb1;
analyze table hb1;
analyze table tb1;
explain format = 'brief' select * from tb1 where  id > 50 and id < 200;
explain format = 'brief' select tb1.id,tb1.name from tb1 where  id > 50 and id < 200 union all select hb1.id,hb1.name from hb1 where  hb1.id > 50 and hb1.id < 200;;
