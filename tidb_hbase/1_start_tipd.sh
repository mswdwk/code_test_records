#!/bin/bash
source ./0_vars.sh
 $PD_DIR/pd-server --data-dir=$PD_DATA_DIR --log-file=$PD_LOG_DIR/pd.log &
