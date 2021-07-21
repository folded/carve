#!/bin/bash
echo building the carve library for Linux...
# by Carsten Arnholm, December 2020
# 
# Requirements
#   1) cmake must be installed and defined in system PATH
# 
# Output is found in "build" folder 
#    only carve static library is built
#
# Using the library:
#    include paths:  
#       carve/include
#       carve/build/include
#       
#    lib path:
#       carve/build/lib
#
cmake -B build \
     -DBUILD_TESTING:BOOL="0" \
     -DCARVE_USE_EXACT_PREDICATES:BOOL="1" \
     -DBUILD_SHARED_LIBS:BOOL="0"  \
     -DCARVE_GTEST_TESTS:BOOL="0" \
     -DCARVE_WITH_GUI:BOOL="0" \
     -DBUILD_SHARED_LIBS:BOOL="0"
#
cd build
make carve
ls -l ./lib/*.a
echo Carve build script ended