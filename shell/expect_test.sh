#!/usr/bin/expect -f
set timeout 10
spawn passwd abc
expect "New password"
send 'R1e34f\r'
expect "Retype new password"
send 'R1e34f\r'
interact
