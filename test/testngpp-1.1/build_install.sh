#!/bin/bash
# build and install

# You can change this testngpp install dir by yourself, it must be an absolute path.
install_dir=../testngpp
build_dir=../build_testngpp_to_install
# relative to build_dir
testngpp_src_dir=../testngpp-1.1

mkdir -p $install_dir
mkdir -p $build_dir

rm -rf $build_dir/*

function build() {
	mkdir $1 2>/dev/null
	cd $1
	cmake -DCMAKE_INSTALL_PREFIX=$install_dir $2
	make install
}

build $build_dir $testngpp_src_dir

cd $testngpp_src_dir

mv -f -u $install_dir/3rdparty $install_dir/..
