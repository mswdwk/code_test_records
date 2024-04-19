#!/usr/bin/expect -f
set timeout 10
spawn ssh user@192.168.79.133
expect "password"
send 'R1e34f\r'
interact
