DB_PORT=3306
DB_USER=root
DB_HOST=localhost
QUERY_NUM=20000
ITERATIONS=2
time mysqlslap -u$DB_USER -h$DB_HOST -P $DB_PORT --protocol=tcp -p -a --concurrency=50,100 --number-of-queries $QUERY_NUM -i $ITERATIONS  --auto-generate-sql-add-autoincrement --engine=innodb 
