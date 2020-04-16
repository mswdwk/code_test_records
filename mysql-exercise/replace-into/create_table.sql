CREATE TABLE `auto` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `k` int(10) unsigned NOT NULL,
  `v` varchar(100) DEFAULT NULL,
  `extra` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_k` (`k`)
) ENGINE=InnoDB;
 INSERT INTO auto (k, v, extra) VALUES (1, '1', 'extra 1'), (2, '2', 'extra 2'), (3, '3', 'extra 3');
SHOW CREATE TABLE auto\G
