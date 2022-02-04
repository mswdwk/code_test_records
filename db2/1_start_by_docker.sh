docker run -itd --name mydb2 --privileged=true -p 50000:50000 -e LICENSE=accept -e DB2INST1_PASSWORD=123 -e DBNAME=testdb -v /data1/db2_1:/database ibmcom/db2
