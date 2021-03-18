echo $1
count=10
tablename=test.tname
output_file=insert.sql
sql_start="insert into $tablename values("
sql_end=")"
item_accu=""
for((i=0;i<$count;i++))
do
#echo $i
item="($i),"
item_accu="$item_accu$item"
done
sql=$sql_start"$item_accu($count)"$sql_end
echo $sql > $output_file
