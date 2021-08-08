DB_PORT=3310
DB_NAME=db1
DB_USER=root
DB_HOST=localhost
CLIENT_NUM=5
QUERY_NUM=`echo $CLIENT_NUM*1000|bc`
ITERATIONS=2
time mysqlslap -u$DB_USER -h$DB_HOST -P $DB_PORT --protocol=tcp --create-schema=$DB_NAME -p --concurrency=$CLIENT_NUM --number-of-queries=$QUERY_NUM -i $ITERATIONS  --engine=innodb  -q input.sql
