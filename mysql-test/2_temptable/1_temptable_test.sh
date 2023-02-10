MTR=~/build-mysql8/mysql-test/mtr
$MTR --extern user=super --extern password='S1a2b3!!' --extern socket=/var/lib/mysql/mysql.sock temptable temptable_basic
$MTR --user=super --extern password='S1a2b3!!' --extern socket=/var/lib/mysql/mysql.sock  --unit-tests 
