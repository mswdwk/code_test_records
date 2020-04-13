#1 mysql自动分区自动清理
from https://blog.csdn.net/nuptuser/article/details/49908291

Mysql数据按天分区,定期删除
https://www.cnblogs.com/garfieldcgf/p/10143367.html
#2 grant user uiadmin@'%' some privilege like alter,drop

#3 execute this auto partition sql 
mysql> source grant_user_uiadmin.sql;
mysql> source p.sql;

#4  查看事件和事件开关
show events;
SHOW VARIABLES LIKE 'event_scheduler';
