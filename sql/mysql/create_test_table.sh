PORT=3310
dbname=test
tbname=$dbname.tbname
create_table_sql="drop table if exists $tbname;create table $tbname (c1 int unsigned not null, c2 char(128) not null,c3 varchar(128) not null,c4 text not null, c5 int not null,c6 char(128) not null,c7 varchar(128) not null,c8 text not null, c9 int not null,c10 char(128) not null, c11 varchar(128) not null, c12 text not null,primary key(c1,c2),unique key (c3,c5) );"

mysql -uroot -p -P$PORT --protocol=tcp -e"select version();create database if not exists $dbname ;$create_table_sql"
output_file=insert-multi-fileds.sql
count=35000
sql_start="insert into $tablename values"
sql_end=""
item_accu=""
i=0
function create_item(){
	local i=$1
    local f1=$i
    local f2="-----------------------------field2-$i"
    local f3="-----------------------------field3-$i"
    local f4="-----------------------------field4-$i"
    local f5=$i
    local f6="-----------------------------field6-$i"
    local f7="-----------------------------field7-$i"
    local f8="-----------------------------field8-$i"
    local f9=$i
    local f10="-----------------------------field10-$i"
    local f11="-----------------------------field11-$i"
    local f12="-----------------------------field12-$i"

	local item="($f1,$f2,$f3,$f4,$f5,$f6,$f7,$f8,$f9,$f10,$f11,$f12),"
	#return $item
	echo $item
}
for((i=0;i<$count-1;i++))
do
	#echo $i
	item=`create_item $i`
	item_accu="$item_accu$item"
	echo -ne "$i\r"
done
echo "finish create sql"
last_item=create_item $i
sql=$sql_start"$item_accu($last_item)"$sql_end
echo $sql > $output_file
