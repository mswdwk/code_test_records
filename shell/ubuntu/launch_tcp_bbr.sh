#!/bin/bash
KernelVersion=`uname -r`
major=`echo $KernelVersion|cut -d '.' -f 1`
minor=`echo $KernelVersion|cut -d '.' -f 2`
if [  $major -ge 4 ] && [ $minor -ge 9 ];then
sysctl net.core.default_qdisc=fq
sysctl net.ipv4.tcp_congestion_control=bbr
else
	echo "Kernel Version >=4.9 is needed!"
fi
#sysctl -p
