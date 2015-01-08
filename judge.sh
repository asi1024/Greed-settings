#!/bin/bash

echo ""
echo ""

ls *.cpp > ls.log || exit 1
FILENAME=`head -n 1 ls.log`
rm ls.log

if test $# -eq 1
then
    FILENAME=$1.cpp
fi

echo "-- $FILENAME --"

g++-4.9 -std=c++11 $FILENAME -Wall -O2 -o a.out 2> compile.log || (head -n 10 compile.log; exit 1)
head -n 10 compile.log
./a.out
rm ./a.out
