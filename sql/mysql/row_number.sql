-- ok 1 
select
	ROW_NUMBER() over() no,
	salary_level,
	level_count
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
			else '       salary >=80k'
		end salary_level ,
		count(1) AS level_count,
		sum(1)
	from
		salaries
	group by
		salary_level
	order by
		level_count desc) s;

-- ok 2 

select
	ROW_NUMBER() over() no,
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
			else '       salary >=80k'
		end salary_level ,
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
	1 = 1 limit 3 ;


-- ok 3
select
	*
from
	(
	select
		ROW_NUMBER() over() no,
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
				else '       salary >=80k'
			end salary_level ,
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
		1 = 1 )s
where
	no > 2
	and no < 7;
