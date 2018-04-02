#!/bin/bash
FILENAME=a.log
FILENAME1=dict.txt
>log.txt
arr=("1");
declare -i count=0
while read LINE1
do
str1_txt=`echo $LINE1 | cut -d ' '  -f 1`
str3_txt=`echo -n $LINE1 | cut -d ' '  -f 2`
b=`expr $str1_txt \* 1`
#echo $b
arr[$b]=$str3_txt
#if [ $a == $b ];then
#	#data=${str1_txt}${str3_txt}${str3_log}
#	#echo $data >> ./log.txt
#	#break
#fi
#echo $LINE1
#count=$[$count + 1]
count+=1
#echo $count
done < $FILENAME1

echo "count = "$count
echo "arr size=" ${#arr[*]}
:<<!
for v in ${arr[@]};do
echo $v;
done
!

cat $FILENAME | while read LINE
do
    str1_log=`echo $LINE | cut -d \| -f 1` 
    str3_log=`echo ${LINE:10}`
    str2_log=`echo $str1_log | cut -d \- -f 2`
    a=`expr $str2_log \* 1`
	
	#data=${str1_txt}${str3_txt}${str3_log}
	echo -e "app-"${arr[$a]}\t$str3_log >> ./log.txt
done
