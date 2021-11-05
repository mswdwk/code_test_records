source ./common.sh

mkdir -p /tmp/qa/test
mkdir -p /tmp/qa/logs
#perl mysql-test-run.pl --extern user=$USER --extern password=$PASSWORD --extern host='localhost' --extern port=3310 funcs_1.ddl_t
cd $MYSQL_TEST
perl $MYSQL_TEST/mysql-test-run.pl --record --extern socket=$BASE/bin/mysql1.sock --extern password=$PASSWORD  $SUITE_CASE.$TEST_CASE
