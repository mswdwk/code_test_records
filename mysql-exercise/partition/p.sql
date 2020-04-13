
--
-- Definition for database log
--
DROP DATABASE IF EXISTS log;
CREATE DATABASE IF NOT EXISTS log
CHARACTER SET utf8
COLLATE utf8_general_ci;
 
-- 
-- Set default database
--
USE log;
 
--
-- Definition for table tb_log
--
CREATE TABLE IF NOT EXISTS tb_log (
  id bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  created_at datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  log varchar(512) NOT NULL DEFAULT '',
  PRIMARY KEY (id, created_at)
)
ENGINE = INNODB
AUTO_INCREMENT = 1
AVG_ROW_LENGTH = 16384
CHARACTER SET utf8mb4
COLLATE utf8mb4_general_ci
PARTITION BY RANGE(TO_DAYS(created_at))
(
PARTITION pbasic VALUES LESS THAN (0)
);
 
DELIMITER $$
 
--
-- Definition for procedure sp_create_log_partition
--
CREATE DEFINER = 'uiadmin'@'%'
PROCEDURE sp_create_log_partition (day_value datetime, tb_name varchar(128))
BEGIN
  DECLARE par_name varchar(32);
  DECLARE par_value varchar(32);
  DECLARE _err int(1);
  DECLARE par_exist int(1);
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION, SQLWARNING, NOT FOUND SET _err = 1;
  START TRANSACTION;
    SET par_name = CONCAT('p', DATE_FORMAT(day_value, '%Y%m%d'));
    SELECT
      COUNT(1) INTO par_exist
    FROM information_schema.PARTITIONS
    WHERE TABLE_SCHEMA = 'log' AND TABLE_NAME = tb_name AND PARTITION_NAME = par_name;
    IF (par_exist = 0) THEN
      SET par_value = DATE_FORMAT(day_value, '%Y-%m-%d');
      SET @alter_sql = CONCAT('alter table ', tb_name, ' add PARTITION (PARTITION ', par_name, ' VALUES LESS THAN (TO_DAYS("', par_value, '")+1))');
      PREPARE stmt1 FROM @alter_sql;
      EXECUTE stmt1;
    END IF;
  END
  $$
 
--
-- Definition for procedure sp_drop_log_partition
--
CREATE DEFINER = 'uiadmin'@'%'
PROCEDURE sp_drop_log_partition (day_value datetime, tb_name varchar(128))
BEGIN
  DECLARE str_day varchar(64);
  DECLARE _err int(1);
  DECLARE done int DEFAULT 0;
  DECLARE par_name varchar(64);
  DECLARE cur_partition_name CURSOR FOR
  SELECT
    partition_name
  FROM INFORMATION_SCHEMA.PARTITIONS
  WHERE TABLE_SCHEMA = 'log' AND table_name = tb_name
  ORDER BY partition_ordinal_position;
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION, SQLWARNING, NOT FOUND SET _err = 1;
  DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
  SET str_day = DATE_FORMAT(day_value, '%Y%m%d');
  OPEN cur_partition_name;
  REPEAT
    FETCH cur_partition_name INTO par_name;
    IF (str_day > SUBSTRING(par_name, 2)) THEN
      SET @alter_sql = CONCAT('alter table ', tb_name, ' drop PARTITION ', par_name);
      PREPARE stmt1 FROM @alter_sql;
      EXECUTE stmt1;
    END IF;
  UNTIL done END REPEAT;
  CLOSE cur_partition_name;
END
$$
 
--
-- Definition for event event_log_auto_partition
--
CREATE
DEFINER = 'uiadmin'@'%'
EVENT event_log_auto_partition
ON SCHEDULE EVERY '1' DAY
STARTS '1972-01-01 00:00:00'
ON COMPLETION PRESERVE
DO
BEGIN
  CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL - 3 DAY), 'tb_log');
  CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL - 2 DAY), 'tb_log');
  CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL - 1 DAY), 'tb_log');
  CALL sp_create_log_partition(NOW(), 'tb_log');
  CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL 1 DAY), 'tb_log');
  CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL 2 DAY), 'tb_log');
  CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL 3 DAY), 'tb_log');
  CALL sp_drop_log_partition(DATE_ADD(NOW(), INTERVAL - 30 DAY), 'tb_log');
 
END
$$
 
--
-- Create partitions based on current time
--
CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL - 3 DAY), 'tb_log')$$
CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL - 2 DAY), 'tb_log')$$
CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL - 1 DAY), 'tb_log')$$
CALL sp_create_log_partition(NOW(), 'tb_log')$$
CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL 1 DAY), 'tb_log')$$
CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL 2 DAY), 'tb_log')$$
CALL sp_create_log_partition(DATE_ADD(NOW(), INTERVAL 3 DAY), 'tb_log')$$
 
DELIMITER ;
