# use g++ version 11 to compile
MYSQL_CXXFLAGS=$(shell ~/MySQL-8025/bin/mysql_config --cxxflags --libs)
MYSQL_SRC_ROOT=$(shell echo ~/repos/mysql-server)
MYSQL_BUILD_ROOT=$(shell echo ~/build-mysql8)
BOOST_DIR=/usr/local/boost_1_73_0

CXXFLAGS= -Wall -g  ${MYSQL_CXXFLAGS} -I${MYSQL_SRC_ROOT}/include -I${MYSQL_SRC_ROOT} \
	-I${BOOST_DIR}/boost  -I${MYSQL_BUILD_ROOT}/include  \
	-L ${BOOST_DIR}/libs -L${MYSQL_SRC_ROOT}/sql -L${MYSQL_BUILD_ROOT}/library_output_directory \
	-L /usr/lib64/mysql -I${MYSQL_SRC_ROOT}/extra/rapidjson/include -I${MYSQL_BUILD_ROOT}/extra/zlib \
	-I${MYSQL_SRC_ROOT}/extra/zlib

LDFLAGS= -lmysqlclient -lmysqlservices -lz -lserver_unittest_library
.PHONY: all clean run scl

all:
	g++ ${CXXFLAGS} main.cc -o main ${LDFLAGS} 

run:
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MYSQL_BUILD_ROOT}/library_output_directory ./main

clean:
	rm -rf main *.o core.*
scl:
	scl enable devtoolset-11 zsh
