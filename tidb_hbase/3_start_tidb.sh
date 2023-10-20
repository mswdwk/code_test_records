#!/bin/bash
# https://www.modb.pro/db/54842
source ./0_vars.sh

#$TIDB_DIR/tidb-server --store=tikv --path="127.0.0.1:2379"  --log-file=$TIDB_LOG_DIR/tidb.log &
$TIDB_DIR/tidb-server --store=tikv --path="127.0.0.1:2379"  --log-file=$TIDB_LOG_DIR/tidb.log --hbase-path=$HBASE_PATH &
