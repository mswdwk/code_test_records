-- https://www.cnblogs.com/guoyinglin/p/13131352.html
/*
https://dev.mysql.com/doc/refman/8.0/en/faqs-general.html
Does MySQL have Sequences?

No. However, MySQL has an AUTO_INCREMENT system, which can also handle inserts in a multi-source replication setup. With the auto_increment_increment and auto_increment_offset system variables, you can set each server to generate auto-increment values that don't conflict with other servers. The auto_increment_increment value should be greater than the number of servers, and each server should have a unique offset.
 */

set global log_bin_trust_function_creators=on;
create table if not exists t_seq(seq_name varchar(32) not null default '', value bigint not null default 0 primary key);
insert into t_seq values('seq1',0);

DELIMITER //
CREATE FUNCTION NEXT_VAL(seq_name varchar(32))
RETURNS BIGINT
BEGIN
	UPDATE t_seq t , (SELECT @current_val:=`value` from t_seq t2 WHERE t2.seq_name=seq_name) t3 set t.value=t.value+1 WHERE t.seq_name=seq_name AND @current_val=t.value;
	RETURN @current_val+1;
END//
DELIMITER ;

select next_val('seq1');
select next_val('seq1');
select next_val('seq1');
select next_val('seq1');
select next_val('seq1');
select * from t_seq;
