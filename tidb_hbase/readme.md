# On linux
## problem 1: tikv error
```log
[WARN] [server.rs:578] ["failed to remove space holder on starting: No such file or directory (os error 2)"]
/tmp/pingcap/log/tikv1.log:67:[2023/11/07 17:10:47.775 +08:00] [WARN] [store.rs:1779] ["set thread priority for raftstore failed"] [error="Os { code: 13, kind: PermissionDenied, message: \"Permission denied\" }"]
```
## solution :
	 change	tikv data dir to other path, which has more free disk space , like 10GB.
