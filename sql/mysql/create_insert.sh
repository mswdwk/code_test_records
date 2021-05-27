echo $1
create_table(){
	create table $tbname (c1 int unsinged primary key, c2 char(256) not null,c3 varchar(256),c4 text not null, c5 int not null,c6 char(256) not null,c7 varchar(256) not null,c8 text not null, c9 int not null,c10 char(256) not null, c11 varchar(256) not null, c12 text not null); 

}
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
