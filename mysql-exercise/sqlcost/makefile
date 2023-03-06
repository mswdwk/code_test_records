# use g++ version 11 to compile
MYSQL_CXXFLAGS=$(shell /usr/local/mysql/bin/mysql_config --cxxflags --libs)
MYSQL_SRC_ROOT=$(shell echo ~/repos/mysql-server)
MYSQL_BUILD_ROOT=$(shell echo ~/repos/build-mysql)
CXXFLAGS= -Wall -g  -L /usr/lib64/mysql ${MYSQL_CXXFLAGS} -I${MYSQL_SRC_ROOT}/include -I${MYSQL_SRC_ROOT} \
	-I${BOOST_DIR}/boost -L ${BOOST_DIR}/libs -I${MYSQL_BUILD_ROOT}/include  \
	-L${MYSQL_BUILD_ROOT}/sql

LDFLAGS=-lsql_main -lmysqlclient -lmysqlservices -lz

.PHONY: all clean

all:
	g++ ${CXXFLAGS} main.cc -o main ${LDFLAGS} 

clean:
	rm -rf main *.o
