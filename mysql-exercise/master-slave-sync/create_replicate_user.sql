-- test on mysql 8.0.25
-- drop user if exist  repl@'%';
drop user repl@'%';
create user repl@'%' identified with 'mysql_native_password' by 'repl123.';
-- grant replication slave,replication client  on *.* to 'repl'@'%' identified by 'repl';
grant replication slave,replication client  on *.* to 'repl'@'%' ;
flush privileges;
