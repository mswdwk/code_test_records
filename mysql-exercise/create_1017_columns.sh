create_sql="create table t1017("
for((i=0;i<1017;i++))
do
create_sql=$create_sql"col$i int,"
done
echo "$i=$create_sql cend int)engine=innodb"
