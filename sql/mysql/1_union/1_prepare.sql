
/* 输出以下语句的执行计划 */

/* create table gt1(id int unsigned auto_increment primary key,name varchar(128),`date` datetime default current_timestamp(),trans_amount decimal(10,3),note text) engine=innodb charset=utf8mb4 collate=utf8mb4_bin; */
drop table if exists gt1;
create table if not exists gt1(id int unsigned auto_increment primary key,uid int unsigned not null,transDate datetime default current_timestamp(),transAmount decimal(10,3) not null default 0.0,note text) engine=innodb charset=utf8mb4 collate=utf8mb4_bin;

drop table if exists ht1;
create table ht1(id int unsigned auto_increment primary key,uid int unsigned not null,transDate datetime default current_timestamp(),transAmount decimal(10,3),note text) engine=innodb charset=utf8mb4 collate=utf8mb4_bin;

insert into gt1(uid,transdate,transamount) values (621,'2023-06-21','10000'),
   (621,'2023-06-22','11000'), (621,'2023-06-23','12000'),(621,'2023-06-24','9000') ;
insert into ht1(uid,transdate,transamount) values (621,'2023-06-17','8000'),
   (621,'2023-06-18','13000'), (621,'2023-06-19','12000'),(621,'2023-06-20','9000'),(621,'2023-06-21','10000');

SELECT uid,transDate,transAmount FROM gt1 WHERE uid=621 AND transDate > '20230620' AND transDate < '20230625'
UNION 
SELECT uid,transDate,transAmount FROM ht1 WHERE uid=621 AND transDate > '20230616' AND transDate < '20230622';

SELECT uid,transDate,transAmount FROM gt1 WHERE uid=621 AND transDate > '20230620' AND transDate < '20230625'
UNION ALL
SELECT uid,transDate,transAmount FROM ht1 WHERE uid=621 AND transDate > '20230616' AND transDate < '20230622';


EXPLAIN  SELECT uid,transDate,transAmount FROM gt1 WHERE uid=621 AND transDate > '20230620' AND transDate < '20230625'
UNION 
SELECT uid,transDate,transAmount FROM ht1 WHERE uid=621 AND transDate > '20230616' AND transDate < '20230622';

alter table gt1 add index(uid,transDate);

EXPLAIN  SELECT uid,transDate,transAmount FROM gt1 WHERE uid=621 AND transDate > '20230620' AND transDate < '20230625'
UNION 
SELECT uid,transDate,transAmount FROM ht1 WHERE uid=621 AND transDate > '20230616' AND transDate < '20230622';

alter table ht1 add index(uid) ;

EXPLAIN  SELECT uid,transDate,transAmount FROM gt1 WHERE uid=621 AND transDate > '20230620' AND transDate < '20230625'
UNION 
SELECT uid,transDate,transAmount FROM ht1 WHERE uid=621 AND transDate > '20230616' AND transDate < '20230622';


