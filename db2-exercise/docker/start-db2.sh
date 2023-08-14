#docker run -it --name mydb2 --privileged=true --network=host -p50000:50000 -e DB2INST1_PASSWORD=db2inst1-pwd -e LICENSE=accept docker.io/ibmcom/db2:latest bash
mkdir -p /data/db2/data
docker run -itd --name mydb2 --privileged=true --network=host -p50000:50000 -e DB2INST1_PASSWORD=db2inst1-pwd -e LICENSE=accept -e DBNAME=testdb -v /data/db2/data:/database ibmcom/db2:latest 
