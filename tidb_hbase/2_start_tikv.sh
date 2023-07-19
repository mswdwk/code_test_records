#!/bin/bash
source ./0_vars.sh
mkdir -p $KV_DATA_DIR
echo "KV_DIR=$KV_DIR"
$KV_DIR/tikv-server --pd-endpoints="127.0.0.1:2379" --addr="127.0.0.1:20160" --data-dir=$KV_DATA_DIR --log-file=$KV_LOG_DIR/tikv1.log &
