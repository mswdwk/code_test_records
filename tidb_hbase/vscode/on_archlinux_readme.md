
On Archlinux , when attach tidb-server process, it occurs error cannot attach .

solution: sudo echo 0 > /proc/sys/kernel/yama/ptrace_scope
