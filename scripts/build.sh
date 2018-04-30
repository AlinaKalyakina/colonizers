#!/bin/bash
SRC=./src/
INC=./include/
EXE=./exe/
mkdir -p $EXE
cd $EXE
for VAR in engine errors gamefield test_positioning interface test_start
do
g++-7 --coverage -Wall -O3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -c -std=gnu++1z ../$SRC/$VAR.cpp -o ./$VAR.o -I ../$INC
done
g++-7 --coverage -Wall -std=gnu++1z engine.o errors.o gamefield.o test_positioning.o interface.o -o test_positioning.out -I ../$INC
g++-7 --coverage -Wall -std=gnu++1z engine.o errors.o gamefield.o test_start.o interface.o -o test_start.out -I ../$INC
