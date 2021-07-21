@echo off
echo building the carve library for MSVC...
REM by Carsten Arnholm, December 2020
REM 
REM Requirements
REM   1) Must be executed from "MSVC Developer Command Prompt"
REM   2) cmake must be installed and defined in system PATH
REM 
REM Output is found in "build" folder 
REM    only carve static library is built
REM
REM Using the library
REM    include paths:  
REM       carve/include
REM       
REM    lib path:
REM       carve/build/lib/Release     (for release builds)
REM       carve/build/lib/Debug       (for debug builds)
REM
cmake -B build ^
     -DBUILD_TESTING:BOOL="0" ^
     -DCARVE_USE_EXACT_PREDICATES:BOOL="1" ^
     -DBUILD_SHARED_LIBS:BOOL="0"  ^
     -DCARVE_GTEST_TESTS:BOOL="0" ^
     -DCARVE_WITH_GUI:BOOL="0" ^
     -DBUILD_SHARED_LIBS:BOOL="0"
REM
cd build
msbuild carve.sln -target:carve /p:Platform="x64" /p:Configuration=Release /m
msbuild carve.sln -target:carve /p:Platform="x64" /p:Configuration=Debug /m
dir .\lib\Debug,.\lib\release
echo Carve build script ended