 #sysbench /usr/share/sysbench/oltp_read_write.lua --time=40 --mysql-host=192.168.79.133 --mysql-port=3310 --mysql-password='123' --mysql-user=root  --mysql-db=testdb --table-size=500000 --tables=1 --threads=8 --report-interval=5 prepare
 #sysbench /usr/share/sysbench/oltp_read_write.lua --time=40 --mysql-host=192.168.79.133 --mysql-port=3310 --mysql-password='123' --mysql-user=root  --mysql-db=testdb --table-size=100000 --tables=1 --threads=8 --report-interval=5 prepare
 #sysbench /usr/share/sysbench/oltp_read_write.lua --time=40 --mysql-host=127.0.0.1 --mysql-port=3310 --mysql-password='Roo123..' --mysql-user=root  --mysql-db=testdb --table-size=50000000 --tables=1 --threads=8 --report-interval=5 prepare
 #sysbench /usr/share/sysbench/oltp_read_write.lua --time=80 --mysql-host=192.168.32.135 --mysql-port=3306 --mysql-user=root --mysql-password=123 --mysql-db=test --table-size=10000 --tables=5 --threads=4 prepare
PORT=3310
HOST=192.168.79.132
PASSWD='123456'
DB_USER=root
TABLE_SIZE=100000
DB=testd
 sysbench /usr/share/sysbench/oltp_read_write.lua --time=40 --mysql-host=$HOST --mysql-port=$PORT --mysql-password=$PASSWD --mysql-user=$DB_USER --mysql-db=$DB --table-size=$TABLE_SIZE --tables=1 --threads=8 --report-interval=5 prepare 
#sysbench /usr/share/sysbench/oltp_read_write.lua --time=80 --mysql-host=192.168.32.135 --mysql-port=3306 --mysql-user=root --mysql-password=123 --mysql-db=test --table-size=10000 --tables=5 --threads=4 prepare
 #sysbench /usr/share/sysbench/oltp_read_write.lua --time=80 --mysql-host=localhost --mysql-port=3306 --mysql-user=root --mysql-password=123 --mysql-db=test --table-size=10000 --tables=5 --threads=4 prepare
