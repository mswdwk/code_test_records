DROP TABLE if exists `test_order`;
CREATE TABLE if not exists `test_order` (
  `id` int(11) NOT NULL,
  `pk1` int(11) NOT NULL,
  `A` char(32) CHARACTER SET utf8 NOT NULL,
  `B` int(11) NOT NULL,
  `C` varchar(32) CHARACTER SET utf8 NOT NULL,
  `D` char(64) CHARACTER SET utf8 NOT NULL,
  `content` varchar(128) CHARACTER SET utf8 DEFAULT NULL,
  PRIMARY KEY (`id`,`pk1`),
  KEY `idx` (`A`,`B`,`C`,`D`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

insert into test_order values(1,1,'A',1,'c','d','content 1'),(2,2,'a',2,'c','d','content 2'),(3,3,'a',3,'C','d','content 3'),(4,4,'a',4,'c','d','content 4'),(5,5,'a',5,'c','d','content 5');
insert into test_order values(11,11,'a',11,'c','D','content 11'),(12,12,'a',12,'c','d','content 12'),(13,13,'a',13,'c','d','content 13'),(14,14,'a',14,'c','d','content 14'),(15,15,'a',15,'c','d','content 15');
insert into test_order values(21,21,'A',21,'c','d3','content 21'),(22,22,'a',22,'c','d','content 22'),(23,23,'a',23,'c','D','content 23'),(24,24,'a',14,'C','d','content 24'),(25,25,'a',25,'c','d','content 25');
insert into test_order values(31,31,'a3',31,'c3','D3','content 31'),(32,32,'a3',22,'c3','d3','content 32'),(33,33,'a3',23,'C3','d3','content 33'),(34,34,'A3',34,'c3','d3','content 34'),(35,35,'a3',35,'C3','d3','content 35');
