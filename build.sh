#!/bin/bash

source /usr/local/bin/color.sh

CMAKE_BUILD_DIR='../out'
PROJECT_NAME='libPrimaryEngine'
LIB_EXTENSION='dll'
BUILD_TYPE=Debug
POSTFIX='-d'
TEST_PROJECT_PATH="test_project/project"
TEST_PROJECT_LIB_PATH="$TEST_PROJECT_PATH/external/lib"
TEST_PROJECT_INCLUDE_PATH="$TEST_PROJECT_PATH/external/primary_engine/include"
INCLUDE_EXPORT_DIR="include"

compile_include_files() {
	EXTERNAL_INCLUDE_DIR="external/include_export"
	INTERNAL_SOURCE_DIR="src"
	FREETYPE_HEADER_PATH="freetype2/ft2build.h"
	PRIMARY_HEADER_PATH="primary_engine.hpp"

	echo "Compiling include files folder..."

	rm -rf $INCLUDE_EXPORT_DIR
	mkdir -p $INCLUDE_EXPORT_DIR

	# copy external header files
	cp -r $EXTERNAL_INCLUDE_DIR/* $INCLUDE_EXPORT_DIR
	# copy all internal source files
	cp -r $INTERNAL_SOURCE_DIR/* $INCLUDE_EXPORT_DIR
	# remove all internal .cpp files
	find $INCLUDE_EXPORT_DIR -type f -name "*.cpp" -delete

	echo "Include files folder compiled ($INCLUDE_EXPORT_DIR)"
}

determine_lib_extension() {
	if [[ $(uname -s) == "Linux" ]]; then
		LIB_EXTENSION='so'
	else
		LIB_EXTENSION='dll'
	fi
}

copy_lib_to_test_project() {
	cp $CMAKE_BUILD_DIR/bin/$PROJECT_NAME$POSTFIX.$LIB_EXTENSION $TEST_PROJECT_LIB_PATH/
	rm -rf $TEST_PROJECT_INCLUDE_PATH
	mkdir -p $TEST_PROJECT_INCLUDE_PATH
	cp -r $INCLUDE_EXPORT_DIR/* $TEST_PROJECT_INCLUDE_PATH/
}

configure() {
	echo -e "Configuring Cmake..."
	cmake -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE -S . -B $CMAKE_BUILD_DIR
}

build() {
	echo -e "Building..."
	compile_include_files
	{ cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE --verbose && copy_lib_to_test_project; } \
	|| { configure && cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE --verbose && copy_lib_to_test_project; } \
	|| { echo -e "${RED}Building failure${NOCOLOR}"; false; }
}

build_test_project() {
	echo -e "Building test project..."
	cd $TEST_PROJECT_PATH
	echo 5 | source ./build.sh
	echo 2 | source ./build.sh
	cd -
}

run_test_project() {
	echo -e "Running..."
	cd $TEST_PROJECT_PATH
	echo 4 | source ./build.sh
	cd -
}

build_and_run() {
	{ build && run_test_project; } || echo -e "${RED}Build&Run failed${NOCOLOR}"
}

clean_all() {
	echo -e "Cleaning..."
	rm -rf $CMAKE_BUILD_DIR/{*,.*} &> /dev/null
	echo -e "${GREEN}All clean!${NOCOLOR}"
}

change_build_type() {
	echo -e "\nBuild type -> ${GREEN}${BUILD_TYPE}${NOCOLOR}"
	echo -e "Choose build type:\n (${RED}d${NOCOLOR})Debug, (${RED}r${NOCOLOR})Release"
	read -n 1 -s input
	case $input in
		d)
			BUILD_TYPE=Debug
			POSTFIX='-d'
			;;
		r)
			BUILD_TYPE=Release
			POSTFIX=''
			;;
		*)
			;;
	esac
}

##### Script Start #####

determine_lib_extension

while true
do
echo -e "\n \
Build type -> ${GREEN}${BUILD_TYPE}${NOCOLOR}\n \
(${RED}1${NOCOLOR}) configure cmake, \n \
(${RED}2${NOCOLOR}) build, \n \
(${RED}3${NOCOLOR}) build & run, \n \
(${RED}4${NOCOLOR}) run test project, \n \
(${RED}5${NOCOLOR}) clean all, \n \
(${RED}6${NOCOLOR}) build test project, \n \
(${RED}b${NOCOLOR}) change build type, \n \
(${GREEN}q${NOCOLOR}) exit\
"

read -n 1 -s input
case $input in
	1)
		configure
		;;
	2)
		build
		;;
	3)
		build_and_run
		;;
	4)
		run_test_project
		;;
	5)
		clean_all
		;;
	6)
		build_test_project
		;;
	b)
		change_build_type
		;;
	*)
		exit
		;;
esac
done