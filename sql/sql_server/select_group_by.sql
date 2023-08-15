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
		END  salary_level ,
		count(1) AS level_count
	from
		salaries
	group by
		case
			when salary < 50000 then '00k <= salary < 50k'
			when salary < 55000 then '50k <= salary < 55k'
			when salary < 60000 then '55k <= salary < 60k'
			when salary < 65000 then '60k <= salary < 65k'
			when salary < 70000 then '65k <= salary < 70k'
			when salary < 75000 then '70k <= salary < 75k'
			when salary < 80000 then '75k <= salary < 80k'
			else                     '       salary >=80k'
		END 
	order by
		level_count desc
