sysbench /usr/share/sysbench/oltp_read_write.lua --time=60  --pgsql-host=192.168.79.133 --pgsql-port=5432 --pgsql-user='root' --pgsql-password='123' --pgsql-db='testdb' --table-size=100000 --tables=10 --threads=10  --db-driver=pgsql $*
