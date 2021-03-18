#mysql --login-path=root -e"select 1;"
sql_file=insert.sql
tablename=test.tname
#echo "prepare insert data"
mysql --login-path=root -e"truncate table $tablename;"
mysql --login-path=root -e"show create table $tablename\G"
echo "current table record number "
mysql --login-path=root -e"select count(*) from $tablename"
echo "start insert sql"
mysql --login-path=root -e"source $sql_file;"
echo "finish insert sql"
echo "current table record number "
mysql --login-path=root -e"select count(*) from $tablename"
