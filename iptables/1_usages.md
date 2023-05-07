# 1)限制指定端口并发数
首先输入命令service iptables stop关闭iptables（注意：iptables可能会有问题，貌似在旧版本中不被认为是一个服务，而是防火墙）

限制端口并发数很简单，IPTABLES就能搞定了，假设你要限制端口8388的IP最大连接数为5，两句话命令：

iptables -I INPUT -p tcp --dport 8388 -m connlimit --connlimit-above 5 -j DROP
iptables -I OUTPUT -p tcp --dport 8388 -m connlimit --connlimit-above 5 -j DROP
我再举个例子，比如你想限制从1024-10240的端口

iptables -I INPUT -p tcp --dport 1024:10240 -m connlimit --connlimit-above 5 -j DROP
iptables -I OUTPUT -p tcp --dport 1024:10240 -m connlimit --connlimit-above 5 -j DROP
保存IPTABLES规则即可（service iptables save），其他端口以此类推。

输入命令service iptables start启动

最后用命令查看是否生效

iptables -L -n -v
# 2) 限制指定端口传输速度
首先输入命令 service iptables stop 关闭 iptables

限制端口并发数很简单，IPTABLES就能搞定了，假设你要限制端口5037的最大连接速度为60个包每秒，两句话命令：

iptables -A INPUT -p tcp --sport 5037 -m limit --limit 60/s -j ACCEPT
iptables -A INPUT -p tcp --sport 5037 -j DROP
也就是限制每秒接受60个包，一般来说每个包大小为64—1518字节(Byte)。

# 3) 限制IP的访问速度
原理：每秒对特定端口进行速度控制，比如每秒超过700个的数据包直接DROP，从而限制特定端口的速度

iptables -A FORWARD -m limit -d 208.8.14.53 --limit 700/s --limit-burst 100 -j ACCEPT 
iptables -A FORWARD -d 208.8.14.53 -j DROP
Linux/VPS下通过iptables限制流量，可通过以下脚本即可实现（首先需要将这台机器配置成网关）。下面给出一个脚本的简单示例：

#限制网段
for ((i = 1; i < 253; i++))  
do
/sbin/iptables -A FORWARD -s 192.168.2.$i -m limit \ --limit 60/s -j ACCEPT 
/sbin/iptables -A FORWARD -s 192.168.2.$i -j DROP

done
#限制单个ip 
/sbin/iptables -A FORWARD -s 192.168.1.135/32 -m limit \ --limit 60/s -j ACCEPT 
/sbin/iptables -A FORWARD -s 192.168.1.135/32 -j DROP 
  
#当流量小于60/s的时候，iptables 接受并转发。当流量大于60/s 的时候，iptables丢弃数据包
# 4) 保存防火墙规则
最后说一下如何解决防火墙重启后失败的问题

iptables-save >/etc/sysconfig/iptables
echo 'iptables-restore /etc/sysconfig/iptables' >> /etc/rc.local
chmod +x /etc/rc.d/rc.local
