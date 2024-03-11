# use g++ version 11 to compile
# mysql-server version is mysql-8.0.25
MYSQL_CXXFLAGS=$(shell ~/MySQL-8025-rel/bin/mysql_config --cxxflags --libs)
MYSQL_SRC_ROOT=$(shell echo ~/repos/mysql-server)
MYSQL_BUILD_ROOT=$(shell echo ~/build-mysql8-rel)
BOOST_DIR=/usr/local/boost_1_73_0
MYSQL_INSTALL_ROOT=$(shell echo ~/MySQL-8025)

CXXFLAGS= -Wall -g  -I${MYSQL_SRC_ROOT}/include -I${MYSQL_SRC_ROOT} \
	-I${BOOST_DIR}/boost  -I${MYSQL_BUILD_ROOT}/include  \
	-L${BOOST_DIR}/libs -L${MYSQL_BUILD_ROOT}/lib \
	-I${MYSQL_SRC_ROOT}/extra/rapidjson/include -I${MYSQL_BUILD_ROOT}/extra/zlib \
	-I${MYSQL_SRC_ROOT}/extra/zlib -I${MYSQL_SRC_ROOT}/extra/protobuf/protobuf-3.11.4/src \
	-I${MYSQL_SRC_ROOT}/unittest/gunit -I/home/user/mysql-8.0.25-src


LDFLAGS= -lz -lserver_unittest_library -lmysqlclient
.PHONY: all clean run scl

all:main.o init.o
	g++ ${CXXFLAGS} $^ -o main ${LDFLAGS} 

%.o:%.cc
	$(CXX) ${CXXFLAGS} -c $^ -o $@ ${LDFLAGS} 
%.o:%.cpp
	$(CXX) ${CXXFLAGS} -c $^ -o $@ ${LDFLAGS} 
run:
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MYSQL_BUILD_ROOT}/library_output_directory ./main

clean:
	rm -rf main *.o core.*
scl:
	scl enable devtoolset-11 zsh
