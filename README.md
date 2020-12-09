
This fork of carve contains fixes to allow static compilation for
* Windows MSVC2013 Express compiler x64
* Linux Ubuntu GNU g++ 7.3.0 with boost x64

## Basic build instructions

* Use CMake Gui on both platforms, disable dev warning (Options menu)
* Enable only CARVE_USE_EXACT_PREDICATES
* Set "Where to build" ../carve/build
* Windows MSVC2013: Build using carve/build/carve.sln
   * Static libs under build/lib/Release and build/lib/Debug
* Linux: Build with makefile under carve/build
   * Static lib under build/lib

## Linux command line build with cmake

$ cd carve
$ cmake -B build -DBUILD_TESTING:BOOL="0" -DCARVE_USE_EXACT_PREDICATES:BOOL="1" -DCARVE_GTEST_TESTS:BOOL="0" -DCARVE_WITH_GUI:BOOL="0" -DBUILD_SHARED_LIBS:BOOL="0"
$ cd build
$ make