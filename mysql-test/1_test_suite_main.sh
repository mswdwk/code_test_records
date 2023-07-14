# max-test-fail=0 means no limit
perl /home/user/MySQL5.7_1/mysql-test/mysql-test-run.pl --user=root  --extern socket=/home/user/MySQL5.7_1/bin/mysql1.sock --extern password=123456 --extern port=3310 --big-test --force --enable-disabled --max-test-fail=0 --parallel=20 --suites=main
