PORT=8025
PASSWORD='R1o2o3..'
MYUSER=root
sysbench /usr/share/sysbench/oltp_read_write.lua --time=60 --report-interval=5 --mysql-host=127.0.0.1 --mysql-port=$PORT --mysql-password=$PASSWORD --mysql-user=$MYUSER --mysql-db=testdb --table-size=1000000 --tables=1 --threads=10 $* 
