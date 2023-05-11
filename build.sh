#!/bin/bash

source /usr/local/bin/color.sh

CMAKE_BUILD_DIR='./bin'
PROJECT_NAME='PrimaryEngine'
SYSTEM=''
BUILD_TYPE=Debug
POSTFIX='-d'
C_COMPILER=clang
CXX_COMPILER=clang++
TEST_PROJECT_PATH="test_project"
TEST_PROJECT_LIB_PATH="$TEST_PROJECT_PATH/external/lib"
TEST_PROJECT_INCLUDE_PATH="$TEST_PROJECT_PATH/external/primary_engine/include"
INCLUDE_EXPORT_DIR="include"

compile_include_files() {
	# path to manually compiled external lib headers
	EXTERNAL_HEADER_DIR="external/include_export"
	# path to internal source files
	INTERNAL_SOURCE_DIR="src"

	echo "Compiling include files folder..."

	rm -rf $INCLUDE_EXPORT_DIR
	mkdir -p $INCLUDE_EXPORT_DIR

	# copy external header files
	cp -r $EXTERNAL_HEADER_DIR/* $INCLUDE_EXPORT_DIR
	# copy all internal source files
	cp -r $INTERNAL_SOURCE_DIR/* $INCLUDE_EXPORT_DIR
	# remove all internal .cpp files
	find $INCLUDE_EXPORT_DIR -type f -name "*.cpp" -delete

	echo "Include files folder compiled ($PWD/$INCLUDE_EXPORT_DIR)"
}

determine_system() {
	if [[ $(uname -s) == "Linux" ]]; then
		SYSTEM='Linux'
	else
		SYSTEM='Windows'
	fi
}

copy_lib_to_test_project() {
	# create lib dir for test project if it doesn't exist
	mkdir -p $TEST_PROJECT_LIB_PATH

	# copy library
	if [[ $SYSTEM == "Linux" ]]; then
		cp "$CMAKE_BUILD_DIR/lib$PROJECT_NAME$POSTFIX.so" $TEST_PROJECT_LIB_PATH/
	else
		cp "$CMAKE_BUILD_DIR/$PROJECT_NAME$POSTFIX.dll" $TEST_PROJECT_LIB_PATH/
		cp "$CMAKE_BUILD_DIR/$PROJECT_NAME$POSTFIX.lib" $TEST_PROJECT_LIB_PATH/
		if [[ $BUILD_TYPE == "Debug" ]]; then
			cp "$CMAKE_BUILD_DIR/$PROJECT_NAME$POSTFIX.pdb" $TEST_PROJECT_LIB_PATH/
		fi
	fi

	# copy include files
	rm -rf $TEST_PROJECT_INCLUDE_PATH
	mkdir -p $TEST_PROJECT_INCLUDE_PATH
	cp -r $INCLUDE_EXPORT_DIR/* $TEST_PROJECT_INCLUDE_PATH/
}

build_test_project() {
	echo -e "Building test project..."
	cd $TEST_PROJECT_PATH
	echo 5 | source ./build.sh
	echo 2 | source ./build.sh
	cd -
}

configure() {
	echo -e "Configuring Cmake..."
	CMAKE_VARS=( \
		"CMAKE_BUILD_TYPE=$BUILD_TYPE" \
		"CMAKE_EXPORT_COMPILE_COMMANDS=1" \
		"CMAKE_C_COMPILER=$C_COMPILER" \
		"CMAKE_CXX_COMPILER=$CXX_COMPILER" \
		"CMAKE_COLOR_DIAGNOSTICS=ON"
	)

	if [[ $SYSTEM == "Windows" ]]; then
		CMAKE_VARS+=("CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON")
	fi

	CMAKE_VARS_STRING=""
	for v in "${CMAKE_VARS[@]}"; do
		CMAKE_VARS_STRING+=" -D$v"
	done

	echo "CMake variables: $CMAKE_VARS_STRING"

	cmake -G Ninja $CMAKE_VARS_STRING -S . -B $CMAKE_BUILD_DIR
}

build() {
	echo -e "Building..."
	compile_include_files
	{ cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE && copy_lib_to_test_project; } \
	|| { configure && cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE && copy_lib_to_test_project; } \
	|| { echo -e "${RED}Building failure${NOCOLOR}"; false; }
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

determine_system

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