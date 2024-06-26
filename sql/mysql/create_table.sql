CREATE TABLE partitioned_table (
    id INT NOT NULL,
    created_at DATETIME NOT NULL,
    data VARCHAR(255),
    INDEX (created_at)
) PARTITION BY RANGE (YEAR(created_at)) (
    PARTITION p0 VALUES LESS THAN (1991),
    PARTITION p1 VALUES LESS THAN (1992),
    PARTITION p2 VALUES LESS THAN (1993),
    PARTITION p3 VALUES LESS THAN (1994),
    PARTITION pmax VALUES LESS THAN MAXVALUE
);


CREATE TABLE partitioned_table3 (     id INT NOT NULL,     created_at DATETIME NOT NULL,     data VARCHAR(255) ) PARTITION BY RANGE (MONTH(created_at)) (     PARTITION p0 VALUES LESS THAN (199101),     PARTITI
ON p1 VALUES LESS THAN (199201),     PARTITION p2 VALUES LESS THAN (199301),     PARTITION p3 VALUES LESS TH
AN (199401),     PARTITION pmax VALUES LESS THAN MAXVALUE );


CREATE TABLE `partitioned_table2` (
  `id` int(11) NOT NULL,
  `created_at` datetime NOT NULL,
  `data` varchar(255) COLLATE utf8mb4_bin DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin
/*!50100 PARTITION BY RANGE (MONTH(created_at))
(PARTITION p0 VALUES LESS THAN (199101) ENGINE = InnoDB,
 PARTITION p1 VALUES LESS THAN (199201) ENGINE = InnoDB,
 PARTITION p2 VALUES LESS THAN (199301) ENGINE = InnoDB,
 PARTITION p3 VALUES LESS THAN (199401) ENGINE = InnoDB,
 PARTITION pmax VALUES LESS THAN MAXVALUE ENGINE = InnoDB) */
