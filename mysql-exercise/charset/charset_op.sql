# utf8mb4字符集可以表示： 表情、少数民族文字、生僻汉字、特殊符号
# 1) 表情
-- 微笑
-- laught
-- F09F9982
select convert(unhex("F09F9982") using utf8mb4);

-- this covert generate warnings when character_results is utf8
select convert(unhex("F09F9982") using utf8mb3);
select convert(unhex("F09F9982") using utf8);

# 2) chinese character to hex
select hex('你');
select hex('a'); -- 䶮字音yǎn

# 3) hex character to chinese 
-- you: 你
select unhex('E4BDA0');
-- '䶮字音yǎn'
select unhex('EEA1A320');
<<<<<<< HEAD
# 4)
CREATE TABLE `t1` (
  `name` varchar(1) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

insert into t1 values ( unhex('F09F9982')); -- failed
"ERROR 1366 (HY000): Incorrect string value: '\xF0\x9F\x99\x82' for column 'name' at row 1"

insert into t1 values ( unhex('EEA1A320')); -- ok
