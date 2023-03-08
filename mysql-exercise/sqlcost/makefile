# use g++ version 11 to compile
MYSQL_CXXFLAGS=$(shell /usr/local/mysql/bin/mysql_config --cxxflags --libs)
MYSQL_SRC_ROOT=$(shell echo ~/repos/mysql-server)
MYSQL_BUILD_ROOT=$(shell echo ~/repos/build-mysql)
CXXFLAGS= -Wall -g  ${MYSQL_CXXFLAGS} -I${MYSQL_SRC_ROOT}/include -I${MYSQL_SRC_ROOT} \
	-I${BOOST_DIR}/boost  -I${MYSQL_BUILD_ROOT}/include  \
	-L ${BOOST_DIR}/libs -L${MYSQL_BUILD_ROOT}/sql -L${MYSQL_BUILD_ROOT}/library_output_directory \
	-L /usr/lib64/mysql -I${MYSQL_SRC_ROOT}/extra/rapidjson/include -I${MYSQL_BUILD_ROOT}/extra/zlib \
	-I${MYSQL_SRC_ROOT}/extra/zlib/zlib-1.2.13 

LDFLAGS= -lmysqlclient -lmysqlservices -lz -lserver_unittest_library
.PHONY: all clean run scl

all:
	g++ ${CXXFLAGS} main.cc -o main ${LDFLAGS} 

run:
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MYSQL_BUILD_ROOT}/library_output_directory ./main

clean:
	rm -rf main *.o
scl:
	scl enable devtoolset-11 zsh
