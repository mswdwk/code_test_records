echo "limit net speed"
sudo iptables -A OUTPUT -p tcp -d 192.168.79.31 --dport 3310 -m limit --limit 50/s --limit-burst 1 -j ACCEPT

sudo iptables -A OUTPUT -p tcp -d 192.168.79.31 --dport 3310 -j DROP


echo "add net delay and packet loss"
sudo tc qdisc add dev ens33 root netem delay 150ms  50ms loss 5% corrupt 1%
sudo tc qdisc del dev ens33 root netem delay 150ms  50ms loss 5% corrupt 1%
tc -s qdisc show dev ens33
sudo tc qdisc del dev ens33 root netem
