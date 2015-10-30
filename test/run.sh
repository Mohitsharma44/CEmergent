#!/bin/bash

## This will be soon converted to CMakeLists or Makefile.

set -v #echo on

mkdir -p ./out

g++ -I/usr/include/python2.7 -I/usr/include -fPIC -c -Wall -m64 -I/opt/EVT/eSDK/include/ devinfo.cpp -o out/devinfo.o

g++ --shared -Wl,--export-dynamic ./out/devinfo.o -L/usr/lib -lboost_python -L/usr/lib/python2.7/config -lpython2.7 -L/opt/EVT/eSDK/lib/  -lEmergentCamera  -lEmergentGenICam  -lEmergentGigEVision -o ./out/devinfo.so
