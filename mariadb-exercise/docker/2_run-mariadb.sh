DATA_DIR=~/mariadb_data
mkdir -p $DATA_DIR
docker run --name maria -p 13306:3306 -e MYSQL_ROOT_PASSWORD=123 -v $DATA_DIR:/var/lib/mysql -d mariadb:10.1.2
