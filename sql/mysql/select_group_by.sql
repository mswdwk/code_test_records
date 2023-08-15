
select case when salary < 60000 then '<60k'  when salary < 65000 then '<65k'  when salary <70000 then '<70k' when salary < 75000 then '<75k' else  '>=75k' end as 'level',count(1) as 'level_count' from salaries group by `level` limit 10;

select level,level_count, level_count/sum(level_count) as percent from (select case when salary < 60000 then '<60k'  when salary < 65000 then '<65k'  when salary <70000 then '<70k' when salary < 75000 then '<75k' else  '>=75k' end as 'level',count(1) as 'level_count' from salaries group by `level` limit 10) as t1;

select level,level_count, level_count/sum(level_count) as percent from (select case when salary < 60000 then '<60k'  when salary < 65000 then '<65k'  when salary <70000 then '<70k' when salary < 75000 then '<75k' else  '>=75k' end as 'level',count(1) as 'level_count' from salaries group by `level` ) as t1;

-- ok -- 
select level,level_count, round(level_count/total,2) rate from (select case when salary < 60000 then '<60k'  when salary < 65000 then '<65k'  when salary <70000 then '<70k' when salary < 75000 then '<75k' else  '>=75k' end as 'level',count(1) as 'level_count' from salaries group by `level` ) as t1 left join (select count(1) as total from salaries) t3 on 1=1;

select level,level_count, concat(round(level_count*100/total,2),'%') rate from (select case when salary < 60000 then '<60k'  when salary < 65000 then '<65k'  when salary <70000 then '<70k' when salary < 75000 then '<75k' else  '>=75k' end as 'level',count(1) as 'level_count' from salaries group by `level` ) as t1 left join (select count(1) as total from salaries) t3 on 1=1;

select level,level_count, lpad(concat(round(level_count*100/total,2),'%'),6,' ') rate from (select case when salary < 55000 then '<55k' when salary < 60000 then '<60k'  when salary < 65000 then '<65k'  when salary <70000 then '<70k' when salary < 75000 then '<75k' else  '>=75k' end as 'level',count(1) as 'level_count' from salaries group by `level` order by level_count  desc ) as t1 left join (select count(1) as total from salaries) t3 on 1=1;

select level,level_count, lpad(concat(round(level_count*100/total,2),'%'),6,' ') rate from (select case when salary <50000 then '<50k' when salary < 55000 then '<55k' when salary < 60000 then '<60k'  when salary < 65000 then '<65k'  when salary <70000 then '<70k' when salary < 75000 then '<75k' when salary <80000 then '<80k' else  '>=80k' end as 'level',count(1) as 'level_count' from salaries group by `level` order by level_count  desc ) as t1 left join (select count(1) as total from salaries) t3 on 1=1;

select
	salary_level,
	level_count,
	lpad(concat(round(level_count * 100 / total, 2), '%'), 6, ' ') percent
from
	(
	select
		case
			when salary < 50000 then '00k <= salary < 50k'
			when salary < 55000 then '50k <= salary < 55k'
			when salary < 60000 then '55k <= salary < 60k'
			when salary < 65000 then '60k <= salary < 65k'
			when salary < 70000 then '65k <= salary < 70k'
			when salary < 75000 then '70k <= salary < 75k'
			when salary < 80000 then '75k <= salary < 80k'
			else                     '       salary >=80k'
		end  salary_level ,
		count(1) AS level_count
	from
		salaries
	group by
		salary_level
	order by
		level_count desc ) t1
left join (
	select
		count(1) total
	from
		salaries) t2 on
	1 = 1;
