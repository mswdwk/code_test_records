DBUSER=test
DBNAME=tpch
if [ -f tpch_ok.txt ];then
	mv tpch_ok.txt tpch_ok.txt.old
fi
if [ -f tpch_err.txt ];then
	mv tpch_err.txt tpch_err.txt.old
fi
for ((i=1;i<=22;++i));do
	echo "[`date`]Test SQL $i" >> tpch_ok.txt
	time mysql --login-path=$DBUSER -D$DBNAME -e "source queries/d$i.sql"	>> tpch_ok.txt
	if [ $? -ne 0 ];then
		echo "[`date`] $i $?" >> tpch_err.log
	fi
done

