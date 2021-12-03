res=`awk -f output_multi_var.awk show_slave_status.txt`
echo "$res"
a=`echo $res|awk '{print $1}'`
b=`echo $res|awk '{print $2}'`
c=`echo $res|awk '{print $3}'`

echo $a
echo $b
echo $c
