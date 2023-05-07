sudo iptables -A OUTPUT -p tcp --dport 3310 -d 192.168.79.134 -m limit --limit 20/s -j ACCEPT
sudo iptables -A OUTPUT -p tcp --dport 3310 -d 192.168.79.134 -j DROP 
sudo iptables -nL OUTPUT --line
