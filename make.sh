
#!/bin/bash

CC=gcc
CCFLAGS="-Wall -Wextra -Wconversion -Wcast-qual -Wshadow -Wpedantic -fsanitize=undefined,address -Wformat-signedness -Wformat=2 -Wlogical-op -Wwrite-strings -Wduplicated-branches -Wduplicated-cond -O3"

SRC_DIR1=./
SRC_DIR2=./dependencies/Implementation
BINS_DIR=./bin
OBJ_DIR=$BINS_DIR/obj
FINAL_BIN=$BINS_DIR/zed

mode=$1

case $mode in
	"")
		SRC_FILES=$(find $SRC_DIR1 $SRC_DIR2 -name '*.c')
		if [[ ! $SRC_FILES =~ ^[[:blank:]]*$ ]]; then
			for path in $SRC_FILES
			do
				file=$(basename $path | sed 's/\.c$//').o
				$CC $CCFLAGS -ggdb3 -c $path -o $OBJ_DIR/$file
				echo $OBJ_DIR/$file produced
			done
		else
			echo Seems like source files were deleted, exiting...
			exit 1
		fi

		OBJ_FILES=$(find $OBJ_DIR -name '*.o')

		if [[ ! $OBJ_FILES =~ ^[[:blank:]]*$ ]]; then
			$CC $CCFLAGS -ggdb3 $OBJ_FILES -o $FINAL_BIN
			echo $FINAL_BIN is your final binary
		else
			echo Seems like object files were deleted, exiting...
			exit 1
		fi
		;;

	"clean")
		rm $OBJ_DIR/*
		;;

	*)
		echo Unknown mode, exiting...
		exit 1
esac
