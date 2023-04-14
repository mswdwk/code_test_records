# error: The transaction log for the database is full
# https://www.cnblogs.com/blfshiye/p/4567096.html
 db2 update db cfg using LOGFILSIZ 10240
 db2 update db cfg using LOGPRIMARY 100
 db2 update db cfg using LOGSECOND 100
