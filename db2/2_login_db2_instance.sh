#Log on to the container:

docker start mydb2
#docker exec -ti mydb2 bash -c "su - ${DB2INSTANCE}"
#docker exec -ti -u  ${DB2INSTANCE} mydb2 bash
docker exec -it -u db2inst1 mydb2  bash
#docker exec -it -u db2inst1 mydb2 bash -c "~/sqllib/bin/db2 connect to testdb"
#where ${DB2INSTANCE} is either db2inst1 or the name chosen via the DB2INSTANCE variable.
