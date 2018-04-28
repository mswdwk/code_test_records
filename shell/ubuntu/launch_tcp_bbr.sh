#!/bin/bash
KernelVersion=`uname -r`

major=`echo $KernelVersion|cut -d '.' -f 1`
minor=`echo $KernelVersion|cut -d '.' -f 2`

enable_tcp_bbr()
{
	sysctl net.core.default_qdisc=fq
	sysctl net.ipv4.tcp_congestion_control=bbr
}

if [  $major -ge 4 ] && [ $minor -ge 9 ];then
	enable_tcp_bbr
else
	echo "Kernel Version >=4.9 is needed!"
fi

disable_tcp_bbr()
{
	sed -i '/net\.core\.default_qdisc=fq/d' /etc/sysctl.conf
	sed -i '/net\.ipv4\.tcp_congestion_control=bbr/d' /etc/sysctl.conf
	sysctl -p
}
