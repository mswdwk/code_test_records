host=192.168.79.132
sysbench /usr/share/sysbench/oltp_read_write.lua --time=60 --mysql-host=$host --mysql-port=4000 --mysql-password='' --mysql-user=root  --mysql-db=test --table-size=100000 --tables=10 --threads=10 $* 
