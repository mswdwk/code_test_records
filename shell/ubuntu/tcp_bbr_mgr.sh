#!/bin/bash
KernelVersion=`uname -r`

major=`echo $KernelVersion|cut -d '.' -f 1`
minor=`echo $KernelVersion|cut -d '.' -f 2`

enable_tcp_bbr()
{
	#sysctl net.core.default_qdisc=fq
	#sysctl net.ipv4.tcp_congestion_control=bbr
	echo "net.core.default_qdisc=fq" >> /etc/sysctl.conf
	echo "sysctl net.ipv4.tcp_congestion_control=bbr" >> /etc/sysctl.conf
	return $?
}

disable_tcp_bbr()
{
	sed -i '/net\.core\.default_qdisc=fq/d' /etc/sysctl.conf
	sed -i '/net\.ipv4\.tcp_congestion_control=bbr/d' /etc/sysctl.conf
	return $?
}

start_tcp_bbr()
{
	if [  $major -ge 4 ] && [ $minor -ge 9 ];then
		disable_tcp_bbr
		ret=enable_tcp_bbr
		sysctl -p
	else
		echo "Kernel Version >=4.9 is needed!"
	fi
}

stop_tcp_bbr()
{
	disable_tcp_bbr
	sysctl -p
}

if [ $# -lt 1 ];then
	echo "start tcp_bbr by default"
	start_tcp_bbr
else
	case $1 in
	start)
		start_tcp_bbr
		;;
	stop)
		stop_tcp_bbr
		;;
	esac;
fi
