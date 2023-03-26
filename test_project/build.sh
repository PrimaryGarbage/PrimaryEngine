#!/bin/bash

CMAKE_BUILD_DIR='./bin'
PROJECT_NAME='PrimaryEngineTest'
BUILD_TYPE=Debug
POSTFIX=''
INTERNAL_RES_DIR_TARGET="$CMAKE_BUILD_DIR"
RES_DIR_TARGET="$CMAKE_BUILD_DIR"
RES_DIR_SOURCE="./resources"
PRIMARY_LIB_EXTENSION=""
PRIMARY_LIB_NAME="libPrimaryEngine-d";
PRIMARY_LIB_PATH="external/lib"

determine_lib_extension() {
	if [[ $(uname -s) == "Linux" ]]; then
		PRIMARY_LIB_EXTENSION='so'
		echo Linux
	else
		PRIMARY_LIB_EXTENSION='dll'
		echo Windows
	fi
}

copy_lib() {
	cp $PRIMARY_LIB_PATH/$PRIMARY_LIB_NAME.$PRIMARY_LIB_EXTENSION $RES_DIR_TARGET
}

copy_res_dir() {
	if [[ -d RES_DIR_TARGET ]]; then
		rm -rf $RES_DIR_TARGET
	fi
	cp $RES_DIR_SOURCE $RES_DIR_TARGET -r
}

configure() {
	echo -e "Configuring Cmake..."
	cmake -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -S . -B $CMAKE_BUILD_DIR
}

build() {
	echo -e "Building..."
	{ cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE --verbose && copy_res_dir; } \
	|| { echo "CMake hasn't been configured prior to building"; configure && cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE --verbose && copy_res_dir; } \
	|| { echo -e "${RED}Building failure${NOCOLOR}"; false; }
}

run() {
	echo -e "Running..."
	copy_lib
	$CMAKE_BUILD_DIR/$PROJECT_NAME$POSTFIX || echo -e "${RED}Working build wasn't found!${NOCOLOR}"
}

build_and_run() {
	{ build && run; } || echo -e "${RED}Build&Run failed${NOCOLOR}"
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

# while true
# do
# echo -e "\n \
# Build type -> ${GREEN}${BUILD_TYPE}${NOCOLOR}\n \
# (${RED}1${NOCOLOR}) configure cmake, \n \
# (${RED}2${NOCOLOR}) build, \n \
# (${RED}3${NOCOLOR}) build & run, \n \
# (${RED}4${NOCOLOR}) run, \n \
# (${RED}5${NOCOLOR}) clean all, \n \
# (${RED}b${NOCOLOR}) change build type, \n \
# (${GREEN}q${NOCOLOR}) exit\
# "

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
		run
		;;
	5)
		clean_all
		;;
	b)
		change_build_type
		;;
	*)
		exit
		;;
esac
# done
