#!/bin/bash
# https://www.modb.pro/db/54842
source ./0_vars.sh
clear

echo "`date` 1: kill existed tidb-server"
killall -9 tidb-server
echo "`date` 2: start new tidb-server"
#$TIDB_DIR/tidb-server --store=tikv --path="127.0.0.1:2379"  --log-file=$TIDB_LOG_DIR/tidb.log &
$TIDB_DIR/tidb-server --store=tikv --plugin-dir=/tmp/pingcap/plugin --path="127.0.0.1:2379"  --log-file=$TIDB_LOG_DIR/tidb.log --hbase-path=$HBASE_PATH &
echo "tidb-server processId $! return $?"
