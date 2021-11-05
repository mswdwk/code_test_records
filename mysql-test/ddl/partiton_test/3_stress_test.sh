source ./common.sh
rm -rf /tmp/qa/*
mkdir -p /tmp/qa/test
mkdir -p /tmp/qa/logs
echo "before stress test,you should cp your test case to funcs_1/t"
cd $MYSQL_TEST
time perl mysql-stress-test.pl \
--stress-suite-basedir="$SUITE_BASE" \
--suite=$SUITE_CASE \
--stress-basedir=/tmp/qa/test \
--server-logs-dir=/tmp/qa/logs \
--test-count=100 \
--threads=1 \
--mysqltest=$MYSQLTEST \
--stress-init-file=./suite/stress-init.txt \
--stress-tests-file=./suite/stress-tests.txt \
--server-user=$USER \
--server-password=$PASSWORD \
--server-host='localhost' \
--server-port=3310 \
--server-database=$DBNAME \
--server-socket=$BASE/bin/mysql1.sock \
--cleanup 
