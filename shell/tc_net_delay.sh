#!/bin/bash
interface=ens33
ip=36.112.131.4
delay=500ms
loss=30%
tc qdisc add dev $interface root handle 1: prio
 # 此命令立即创建了类 : 1:1, 1:2, 1:3 ( 缺省三个子类 ) 
tc filter add dev $interface parent 1:0 protocol ip prio 1 u32 match ip dst $ip flowid 2:1
# 在 1:1 节点添加一个过滤规则 , 优先权 1: 凡是去往目的地址是  $ip( 精确匹配 ) 的 IP 数据包 , 发送到频道 2:1.
tc qdisc add dev $interface parent 1:1 handle 2: netem delay $delay   loss $loss
# 此命令立即创建了类 : 2:1, 2:2, 2:3 ( 缺省三个子类 ) ) 

#上面的脚本实现的是访问36.112.131.4，延时500ms，丢包30%


# 取消限速
tc qdisc del dev $interface root

# https://www.cnblogs.com/mrwuzs/p/12793207.html
