DATADIR=$HOME/db2data
mkdir -p $DATADIR
docker run -itd --name mydb2 --privileged=true -p 5000:50000 -e LICENSE=accept -e DB2INST1_PASSWORD=123 -e DBNAME=testdb -v $DATADIR:/database ibmcom/db2
