source ./common.sh
mysql -u$USER -p$PASSWORD -hlocalhost -P3310 --protocol=tcp $DBNAME -e "source create_partition_table.sql"

cp -f $TEST_CASE.test $MYSQL_TEST/suite/$SUITE_CASE/t
