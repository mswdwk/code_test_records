## Can't open the mysql.plugin table
 solution: add option --initialize when use mysql.server to start,
## ERROR 1045 (28000): Access denied for user 'u'@'localhost' (using password: YES)
 reason: password error
## ERROR 1064 (42000): You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near 'database' at line 1
 reason: syntax error
## ERROR 1820 (HY000): You must reset your password using ALTER USER statement before executing this statement.
 solution
```
mysql> ALTER USER USER() IDENTIFIED BY '123456';
```
