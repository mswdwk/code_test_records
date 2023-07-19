OS=`uname -o`
if [ $OS = "GNU/Linux" ];then
	export PD_DIR=~/.tiup/components/pd/v6.6.0
	export KV_DIR=~/.tiup/components/tikv/v6.6.0
	export TIDB_DIR=~/repos/pingcap/tidb/bin
else 
	export PD_DIR=~/.tiup/components/pd/v6.6.0
	export KV_DIR=~/repos/pingcap/tikv/target/release
	export TIDB_DIR=~/repos/pingcap/tidb/bin
	#echo "Not support $OS"
fi

export PD_DATA_DIR=/tmp/pingcap/pd
export PD_LOG_DIR=/tmp/pingcap/log

export KV_DATA_DIR=/tmp/pingcap/tikv
export KV_LOG_DIR=/tmp/pingcap/log

export TIDB_DATA_DIR=/tmp/pingcap/tidb
export TIDB_LOG_DIR=/tmp/pingcap/log
