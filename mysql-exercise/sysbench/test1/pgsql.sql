select datname from pg_database;
SELECT table_name FROM information_schema.tables WHERE table_schema = 'public';
SELECT column_name FROM information_schema.columns WHERE table_name ='sbtest1';

select count(1) from sbtest1;
select * from sbtest1 limit 10 offset 1001234;

explain select * from sbtest1 limit 100 offset 9001234;
select max(id),min(id) from sbtest1;
