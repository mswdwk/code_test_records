DBUSER=test
DBNAME=tpch
if [ -f tpch_ok.txt ];then
	mv tpch_ok.txt tpch_ok.txt.old
fi
if [ -f tpch_err.txt ];then
	mv tpch_err.txt tpch_err.txt.old
fi
if [ -f tpch_sql_cost.txt ];then
	mv tpch_sql_cost.txt tpch_sql_cost.txt.old
fi
for ((i=1;i<=22;++i));do
	start_s=`date +%s`
	echo "[`date`]Test SQL $i" >> tpch_ok.txt
	time mysql --login-path=$DBUSER -D$DBNAME -e "source queries/d$i.sql"	>> tpch_ok.txt
	end_s=`date +%s`
	cost_s=`echo "$end_s - $start_s"|bc`
	echo "SQL $i cost $cost_s seconds" >> tpch_sql_cost.txt
	if [ $? -ne 0 ];then
		echo "[`date`] $i $?" >> tpch_err.log
	fi
done

