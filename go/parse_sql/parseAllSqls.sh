sqlfile_dir=~/sqldatas/sqls
find $sqlfile_dir -type f -name "*.sql" -exec ./mysql {} \;
exit 0
count=0
# pay attention to space
sqlfiles=$(find $sqlfile_dir -type f -name "*.sql" -printf '"%f"\n' ) 
for sqlfile in $sqlfiles
do
	let count=count+1
	echo "count=$count '$sqlfile'"
done
