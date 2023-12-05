#!/usr/bin/perl
MYSQL_TEST_DIR=~/mysql-8.0.31/mysql-test
MTR=$MYSQL_TEST_DIR/mtr

cd $MYSQL_TEST_DIR
perl $MTR --verbose --user=root --extern password=R1o2o3.. --extern port=8031 --extern host=localhost --extern protocol=tcp  --skip-rpl  --skip-sys-schema --skip-ndb  collations.chinese
