SELECT SALARY_LEVEL ,level_count,CONCAT (cast( level_count*100.0/TOTAL_COUNT AS DECIMAL(4,2)),'%') percent  FROM (
		SELECT salary_level,count(1) level_count FROM	(SELECT 
		case
			when salary < 50000 then '00k <= salary < 50k'
			when salary < 55000 then '50k <= salary < 55k'
			when salary < 60000 then '55k <= salary < 60k'
			when salary < 65000 then '60k <= salary < 65k'
			when salary < 70000 then '65k <= salary < 70k'
			when salary < 75000 then '70k <= salary < 75k'
			when salary < 80000 then '75k <= salary < 80k'
			else                     '       salary >=80k'
		end  salary_level
		FROM SALARIES )s  GROUP BY salary_level ORDER BY level_count DESC ) s2
		LEFT JOIN (SELECT count(1) total_count FROM SALARIES )s3 ON 1=1  LIMIT 10
