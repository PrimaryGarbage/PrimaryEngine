#!/bin/bash

source /usr/local/bin/color.sh

CMAKE_BUILD_DIR='../out'
PROJECT_NAME='PrimaryEngine'
BUILD_TYPE=Debug
POSTFIX='-d'
INTERNAL_RES_DIR_TARGET="$CMAKE_BUILD_DIR/bin/"
RES_DIR_TARGET="$CMAKE_BUILD_DIR/bin/"
RES_DIR_SOURCE="./resources"

copy_res_dir() {
	if [[ -d RES_DIR_TARGET ]]; then
		rm -rf $RES_DIR_TARGET
	fi
	cp $RES_DIR_SOURCE $RES_DIR_TARGET -r
}

configure() {
	echo -e "Configuring Cmake..."
	cmake -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE -S . -B $CMAKE_BUILD_DIR
}

build() {
	echo -e "Building..."
	{ cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE && copy_res_dir; } \
	|| { configure && cmake --build $CMAKE_BUILD_DIR --config $BUILD_TYPE && copy_res_dir; } \
	|| { echo -e "${RED}Building failure${NOCOLOR}"; false; }
}

run() {
	echo -e "Running..."
	$CMAKE_BUILD_DIR/bin/$PROJECT_NAME$POSTFIX || echo -e "${RED}Working build wasn't found!${NOCOLOR}"
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

while true
do
echo -e "\n \
Build type -> ${GREEN}${BUILD_TYPE}${NOCOLOR}\n \
(${RED}1${NOCOLOR}) configure cmake, \n \
(${RED}2${NOCOLOR}) build, \n \
(${RED}3${NOCOLOR}) build & run, \n \
(${RED}4${NOCOLOR}) run, \n \
(${RED}5${NOCOLOR}) clean all, \n \
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
done