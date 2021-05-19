#### master  my.cnf
server_id = 1                #唯一标识，主库从库不能重复
log_bin = mysql-bin          #开启日志
#binlog_format=MIXED      　　 #日志记录的格式
max_binlog_size = 512M  　　　#单个日志文件最大
expire_logs_days = 3          #日志有效期（天）
binlog_do_db = test1,test2    #日志记录那些数据库
binlog_ignore_db = mysql,performance_schema,information_schema #日志记录忽略那些数据库的
###
grant replication slave on *.* to 'repl'@'%' identified by 'repl';
mysql>grant replication slave on *.* to 'slave'@localhost identified by '123';
mysql>flush tables with read lock;
mysql>show master status;
mysql>unlock tables;
