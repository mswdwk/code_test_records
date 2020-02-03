# slave mysqld my.cnf
server_id = 2                #唯一标识，主库从库不能重复
log_bin = mysql-bin          #开启日志
#binlog_format=MIXED      　　 #日志记录的格式
max_binlog_size = 512M  　　　#单个日志文件最大
expire_logs_days = 3          #日志有效期（天）
binlog_do_db = test1,test2    #日志记录那些数据库
binlog_ignore_db = mysql,performance_schema,information_schema #日志记录忽略那些数据库的

###### login slave mysql
mysql>stop slave;
mysql>change master to master_host='localhost',master_user='slave',master_password='123',master_port=3306,master_log_file='mysql-bin.000001',master_log_pos=830;
mysql>start slave;
