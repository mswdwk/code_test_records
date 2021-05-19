echo $1
count=35000
tablename=test.tname
output_file=insert.sql
sql_start="insert into $tablename values"
sql_end=""
item_accu=""
i=0
for((i=0;i<$count-1;i++))
do
#echo $i
item="($i),"
item_accu="$item_accu$item"
done
sql=$sql_start"$item_accu($i)"$sql_end
echo $sql > $output_file
