create table customer(id int auto_increment primary key,user_name varchar(32), 
crat_tmtp datetime , apli_stas char(2),take_effect_stas char(2), content varchar(128) )
engine=innodb ;


-- apli_stas 1,2,3,4,5 .  1 申请中  2审批中  3审批通过  4 退回  5 作废

-- take_effect_stas 1,2,3 : 1未生效, 2 已生效,3 已失效 .

-- 生效：  apli_stas=3 and take_effect_stas = 2
-- 在途：  apli_stas in (1,2) & take_effect_stas = 1
-- 其他：

set sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION'

insert into customer(user_name,crat_tmtp,apli_stas,take_effect_stas,content) values
('zhangsan',now(),'1','1','第1次申请，在途');

insert into customer(user_name,crat_tmtp,apli_stas,take_effect_stas,content) values
('zhangsan',now(),'2','1','第2次申请，在途');

insert into customer(user_name,crat_tmtp,apli_stas,take_effect_stas,content) values
('zhangsan',now(),'3','2','第3次申请，生效');

insert into customer(user_name,crat_tmtp,apli_stas,take_effect_stas,content) values
('zhangsan',now(),'3','3','第4次申请，失效');

insert into customer(user_name,crat_tmtp,apli_stas,take_effect_stas,content) values
('zhangsan',now(),'4','1','第5次申请，作废');

insert into customer(user_name,crat_tmtp,apli_stas,take_effect_stas,content) values
('zhangsan',now(),'5','1','第6次申请，退回');

insert into customer(user_name,crat_tmtp,apli_stas,take_effect_stas,content) values
('zhangsan',now(),'5','2','第7次申请，退回 ');

-- 生效
select * from customer t1  where apli_stas='3' and take_effect_stas = '2'

-- 在途
select * from customer t1  where apli_stas in ('1','2') and take_effect_stas = '1'

-- 生效或在途的 客户记录 
select * from customer t1  where (apli_stas in ('1','2') and take_effect_stas = '1') or (apli_stas='3' and take_effect_stas = '2')

-- 没有 生效或在途的 客户记录 。注意：这些记录中的user_name 可以存在 有生效记录的user_name
select * from customer t1  where not ( (apli_stas in ('1','2') and take_effect_stas = '1') or (apli_stas='3' and take_effect_stas = '2'))


-- 查询： 即没有生效 ， 也没有 在途 的客户信息
--  方法1 【不推荐】
-- 计算复杂度:  O(N*N)
select
	*
from
	customer t1
where
	not exists (
	select
		1
	from
		customer t2
	where
		t1.user_name = user_name
		and (apli_stas in ('1', '2')
			and take_effect_stas = '1')
		or (apli_stas = '3'
			and take_effect_stas = '2')
	limit 1)


-- 查询： 即没有生效 ， 也没有 在途 的客户信息
--  方法2 【推荐】
-- 计算复杂度:  O(N)
SELECT
	t1.*
FROM
	customer t1
LEFT JOIN (
	SELECT
		DISTINCT user_name
	FROM
		customer
	WHERE
		(apli_stas IN ('1', '2')
	AND take_effect_stas = '1')
OR (apli_stas = '3'
	AND take_effect_stas = '2')
) t2 ON
	t1.user_name = t2.user_name
WHERE
	t2.user_name IS NULL;
