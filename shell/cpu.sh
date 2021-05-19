#!/bin/sh

while true
do
        ps -H -eo user,pid,ppid,tid,time,%cpu,cmd --sort=%cpu
        sleep 1
done
