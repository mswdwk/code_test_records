CREATE TABLE `person` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `score` int(11) NOT NULL,
  `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) comment "主键，在创建表插入数据后会成为聚集索引",
  KEY `name_score` (`name`(191),`score`) comment "又创建一个key",
  KEY `create_time` (`create_time`) comment "又创建一个key"
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/* https://blog.csdn.net/qq_36963950/article/details/108428696 */
