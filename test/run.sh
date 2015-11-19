#!/bin/bash

## This will be soon converted to CMakeLists or Makefile.

#set -v #echo on

#rm -rf ./out

#mkdir -p ./out

#g++ -I/usr/include/python2.7 -I/usr/include -fPIC -c -Wall -m64 -I/opt/EVT/eSDK/include/ CEmergent.cpp -o out/CEmergent.o

#g++ --shared -Wl,--export-dynamic ./out/CEmergent.o -L/usr/lib -lboost_python -L/usr/lib/python2.7/config -lpython2.7 -L/opt/EVT/eSDK/lib/  -lEmergentCamera  -lEmergentGenICam  -lEmergentGigEVision -o ./out/CEmergent.so


make clean

make

make copyfiles

cd ./out

python emergent.py

xdg-open test.tif
