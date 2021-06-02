set sql_log_bin = 0;
CREATE USER rpl_user@'%' IDENTIFIED BY 'password';
CREATE USER rpl_user@'%' IDENTIFIED BY 'password';
flush privileges;
set sql_log_bin = 1;

CHANGE MASTER TO MASTER_USER='rpl_user', MASTER_PASSWORD='password' FOR CHANNEL 'group_replication_recovery';


CHANGE MASTER TO MASTER_USER='rpl_user', MASTER_PASSWORD='password' FOR CHANNEL 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa';

performance_schema.replication_group_members


INSTALL PLUGIN group_replication SONAME 'group_replication.so';

select * from performance_schema.replication_group_members;

show plugins;

SET GLOBAL group_replication_bootstrap_group=ON;
START GROUP_REPLICATION;
SET GLOBAL group_replication_bootstrap_group=OFF;

/*---------- on node 1---------------------*/
create databaese test;
use test;
CREATE TABLE t1 (c1 INT PRIMARY KEY, c2 TEXT NOT NULL);
INSERT INTO t1 VALUES (1, 'Luis');
/* on node 2*/
START GROUP_REPLICATION;
select * from performance_schema.replication_group_members;
