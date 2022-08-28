#生成22个查询语句
for line in {1..22} 
do
sed -i "/^:n.*/d" $line.sql  
./qgen -d $line > d$line.sql
done
