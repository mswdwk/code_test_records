create database if not exists testdb;
drop table if exists t1;
CREATE TABLE `t1` (
  `id` int(11) NOT NULL,
  `c2` int(11) DEFAULT NULL,
  `c3` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `c2` (`c2`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

insert into t1 value (1,1,1),(2,2,2),(99,99,99),(100,100,100);