#!/bin/bash

filepath=$(cd "$(dirname "$0")"; pwd)

#monitoring interval(second)
INTERVAL=8

#restart interval(second)
INTERVALRESTART=5

#==================================================================
PROGRAM=tcpdump
#"-restore" for restoring from cache file
PROCESSNUM=1
MAX_USED_DISK=90
ETHNAME=$1
LOG=${filepath}/$PROGRAM.$ETHNAME.log
if [ $# -lt 1 ];then
#echo $#
echo "usage: ./$0 ETHNAME"
exit 0
fi
start_program="tcpdump -i $ETHNAME -w $ethname.pcap -s 0 "
#LOG=/dev/null
OUT=/dev/null

#killall -9 $PROGRAM 1>/dev/null 2>/dev/null
#rm -f $OUT

manual_start=1
lost_times=0
while true
do
	#"**********Now begin to monitor program's status ************"
	pscmd=`ps x | grep $PROGRAM |grep $ETHNAME| grep -v grep | wc -l`
	currentnum=`expr $pscmd`
	if [ $currentnum -lt $PROCESSNUM ]; then
		lost_times=`expr $lost_times + 1`
		if [ $lost_times -ge 3 ]; then
			SYSTIME=$(date)
			if [ $manual_start -eq 1 ]; then
				echo "======================================================" >> $LOG
				echo "Restart time(manual start):" $SYSTIME >> $LOG
				echo "Program name(manual start):" $PROGRAM $PARAMETER >> $LOG
				echo "======================================================" >> $LOG
				manual_start=0
			else
				echo "======================================================" >> $LOG
				echo "Restart time(new process):" $SYSTIME >> $LOG
				echo "Program name(new process):" $PROGRAM $PARAMETER >> $LOG
				echo "======================================================" >> $LOG
			fi
			lost_times=0
			#killall -9 $PROGRAM 1>/dev/null 2>/dev/null
			#rm -rf /tmp/*fifo
			sleep $INTERVALRESTART
            echo ${start_program}
            ${start_program}
#			#${filepath}/$PROGRAM
		fi
	else
		manual_start=0
		lost_times=0
	fi
	#"**********Now end to monitor program's status !**********"
	sleep $INTERVAL
done
	exit 0

