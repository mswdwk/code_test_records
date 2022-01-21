# sysbench /usr/share/sysbench/oltp_read_write.lua --time=60 --mysql-host=127.0.0.1 --mysql-port=3310 --mysql-password='Roo123..' --mysql-user=root  --mysql-db=testdb --table-size=100000 --tables=10 --threads=10 $* 
HOST=192.168.79.133
PORT=3310
sysbench /usr/share/sysbench/oltp_read_write.lua --time=60 --mysql-host=$HOST --mysql-port=$PORT --mysql-password='123' --mysql-user=root  --mysql-db=testdb --table-size=100000 --tables=10 --threads=10 $* 
