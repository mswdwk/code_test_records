PORT=3310
dbname=test
tbname=dbname.tbname
create_table_sql="drop table if exists $tbname;create table $tbname (c1 int unsigned not null, c2 char(128) not null,c3 varchar(128) not null,c4 text not null, c5 int not null,c6 char(128) not null,c7 varchar(128) not null,c8 text not null, c9 int not null,c10 char(128) not null, c11 varchar(128) not null, c12 text not null,primary key(c1,c2),unique key (c3,c5) );"

output_file="insert-multi-fileds.sql"
count=35000
sql_start="insert into $tablename values"
sql_end=""
item_accu=""
i=0
def create_item(id):
    i=id
    f1=i
    f2="-----------------------------field2-$i"
    f3="-----------------------------field3-$i"
    f4="-----------------------------field4-$i"
    f5=i
    f6="-----------------------------field6-$i"
    f7="-----------------------------field7-$i"
    f8="-----------------------------field8-$i"
    f9=i
    f10="-----------------------------field10-$i"
    f11="-----------------------------field11-$i"
    f12="-----------------------------field12-$i"
    item="($f1,$f2,$f3,$f4,$f5,$f6,$f7,$f8,$f9,$f10,$f11,$f12),"
    return item


for i in range (0,$count):
do
#echo $i
	item=create_item(i) 
	item_accu="$item_accu$item"
	echo -ne "$i\r"
done
echo "finish create sql"
last_item=create_item $i
sql=$sql_start"$item_accu($last_item)"$sql_end
echo $sql > $output_file

