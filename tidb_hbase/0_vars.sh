OS=`uname -o`
if [ $OS = "GNU/Linux" ];then
	#export PD_DIR=~/.tiup/components/pd/v6.6.0
	export PD_DIR=~/repos/pingcap/pd/bin
	export KV_DIR=~/repos/pingcap/tikv/target/release
	export TIDB_DIR=~/repos/pingcap/tidb/bin
	export KV_DATA_DIR=/data/pingcap/tikv
	export HBASE_PATH="127.0.0.1:12181"
else 
	export PD_DIR=~/.tiup/components/pd/v6.6.0
	export KV_DIR=~/repos/pingcap/tikv/target/release
	export TIDB_DIR=~/repos/pingcap/tidb/bin
	#echo "Not support $OS"
	export KV_DATA_DIR=/tmp/pingcap/tikv
	export HBASE_PATH="127.0.0.1:2181"
fi

export PD_DATA_DIR=/tmp/pingcap/pd
export PD_LOG_DIR=/tmp/pingcap/log

export KV_LOG_DIR=/tmp/pingcap/log

export TIDB_DATA_DIR=/tmp/pingcap/tidb
export TIDB_LOG_DIR=/tmp/pingcap/log
