DROP TABLE IF EXISTS tutorial;
CREATE TABLE IF NOT EXISTS tutorial (id INT NOT NULL AUTO_INCREMENT, title VARCHAR(255), description VARCHAR(255), published BOOLEAN, PRIMARY KEY (id)) charset=utf8mb4;
set names utf8mb4 collate utf8mb4_bin;
insert into tutorial values (1,"title 1","description 1",1),(100,"title 100","description 100",true);
set global long_query_time=0.5;
set global slow_query_log=on;
set global old_alter_table=off;
set global max_join_size=102410241024;

DROP TABLE IF EXISTS blog;
CREATE TABLE IF NOT EXISTS blog (
    id INT UNSIGNED NOT NULL AUTO_INCREMENT ,
    created_at DATETIME NOT NULL DEFAULT '1900-01-01 00:00:00',
    updated_at DATETIME ,
    title VARCHAR(255) NOT NULL DEFAULT '',
    content text ,
    viewed INT UNSIGNED NOT NULL DEFAULT 0,    -- 读
    forward INT UNSIGNED NOT NULL DEFAULT 0,   -- 转发
    praise  INT UNSIGNED NOT NULL DEFAULT 0,   -- 赞
    trample INT UNSIGNED NOT NULL DEFAULT 0,   -- 踩
    favorite INT UNSIGNED NOT NULL DEFAULT 0,  -- 收藏
    deleted_flag SMALLINT NOT NULL DEFAULT 0,  -- 删除
    UNIQUE INDEX (title,created_at),
    PRIMARY KEY(id,created_at)
) CHARSET=utf8mb4 PARTITION BY RANGE (YEAR(created_at)) (
    PARTITION p2000 VALUES LESS THAN (2000),
    PARTITION p2010 VALUES LESS THAN (2010),
    PARTITION p2016 VALUES LESS THAN (2016),
    PARTITION p2017 VALUES LESS THAN (2017),
    PARTITION p2018 VALUES LESS THAN (2018),
    PARTITION p2019 VALUES LESS THAN (2019),
    PARTITION p2020 VALUES LESS THAN (2020),
    PARTITION p2021 VALUES LESS THAN (2021),
    PARTITION p2022 VALUES LESS THAN (2022),
    PARTITION p2023 VALUES LESS THAN (2023),
    PARTITION p2024 VALUES LESS THAN (2024),
    PARTITION p2025 VALUES LESS THAN (2025),
    PARTITION p2026 VALUES LESS THAN (2026),
    PARTITION pmax VALUES LESS THAN MAXVALUE
) ;
insert blog values (1,now(),null,'my first blog','hello blog',0,0,0,0,0,0);