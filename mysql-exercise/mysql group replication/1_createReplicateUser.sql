set sql_log_bin = 0;
drop user if exists rpl_user ;
CREATE USER rpl_user@'%' IDENTIFIED BY 'password';
GRANT REPLICATION SLAVE ON *.* TO rpl_user@'%';
flush privileges;
set sql_log_bin = 1;

CHANGE MASTER TO MASTER_USER='rpl_user', MASTER_PASSWORD='password' FOR CHANNEL 'group_replication_recovery';

INSTALL PLUGIN group_replication SONAME 'group_replication.so';

select * from performance_schema.replication_group_members;

show plugins;
