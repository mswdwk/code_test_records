
On Archlinux , when attach tidb-server process, it occurs error cannot attach .

# solution 1: sudo echo 0 > /proc/sys/kernel/yama/ptrace_scope
# solution 2: 
```shell
sudo echo "kernel.yama.ptrace_scope=0" > /etc/sysctl.d/debug_vscode.conf

sudo sysctl -p /etc/sysctl.d/debug_vscode.conf 
#or 
sudo sysctl -p --system
```
