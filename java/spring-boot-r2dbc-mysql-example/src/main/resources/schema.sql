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
    id INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT '主键',
    crt_dt DATETIME(6) NOT NULL DEFAULT '1900-01-01 00:00:00' COMMENT '创建时间戳',
    upd_dt DATETIME(6) COMMENT '更新时间戳',
    title VARCHAR(255) NOT NULL DEFAULT '' COMMENT '标题',
    vers VARCHAR(10) NOT NULL DEFAULT '版本号',
    cont TEXT COMMENT '博客内容',
    viewed INT UNSIGNED NOT NULL DEFAULT 0  COMMENT  '读',
    forward INT UNSIGNED NOT NULL DEFAULT 0   COMMENT  '转发',
    praise  INT UNSIGNED NOT NULL DEFAULT 0   COMMENT  '赞',
    trample INT UNSIGNED NOT NULL DEFAULT 0   COMMENT  '踩',
    favorite INT UNSIGNED NOT NULL DEFAULT 0  COMMENT  '收藏',
    del_flg SMALLINT NOT NULL DEFAULT 0  COMMENT  '删除',
    UNIQUE INDEX (title,vers,crt_dt),
    PRIMARY KEY(id,crt_dt)
) CHARSET=utf8mb4 PARTITION BY RANGE (YEAR(crt_dt)) (
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
    PARTITION p2027 VALUES LESS THAN (2027),
    PARTITION pmax VALUES LESS THAN MAXVALUE
) ;
insert into blog values (1,now(),null,'my first blog','1.0','hello blog',0,0,0,0,0,0);