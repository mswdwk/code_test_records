#Log on to the container:

docker start mydb2
docker exec -ti mydb2 bash -c "su - ${DB2INSTANCE}"
#where ${DB2INSTANCE} is either db2inst1 or the name chosen via the DB2INSTANCE variable.
