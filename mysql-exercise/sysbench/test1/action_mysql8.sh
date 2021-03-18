PORT=3306
PASSWORD='Roo123..'
MYUSER=root2
 sysbench /usr/share/sysbench/oltp_read_write.lua --time=60 --mysql-host=127.0.0.1 --mysql-port=$PORT --mysql-password=$PASSWORD --mysql-user=$MYUSER --mysql-db=testdb --table-size=100000 --tables=10 --threads=10 $* 
