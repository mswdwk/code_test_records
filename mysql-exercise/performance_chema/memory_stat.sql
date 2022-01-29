SELECT SUBSTRING_INDEX(event_name,'/',2) AS code_area, sys.format_bytes(SUM(current_alloc)) AS current_alloc FROM sys.x$memory_global_by_current_bytes GROUP BY SUBSTRING_INDEX(event_name,'/',2) ORDER BY SUM(current_alloc) DESC;
 
 
  SELECT event_name AS code_area, sys.format_bytes(SUM(current_alloc)) AS current_alloc FROM sys.x$memory_global_by_current_bytes GROUP BY event_name ORDER BY SUM(current_alloc) DESC;
