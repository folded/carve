
This fork of carve contains fixes to allow static compilation for
* Windows MSVC2013 Express compiler x64
* Linux Ubuntu GNU g++ 7.3.0 with boost x64

## Basic build instructions

* Use CMake Gui on both platforms, disable dev warning (Options menu)
* Enable only CARVE_USE_EXACT_PREDICATES
* Set "Where to build" ../carve/cmake_build
* Windows MSVC2013: Build using carve/cmake_build/carve.sln
   * Static libs under cmake_build/lib/Release and cmake_build/lib/Debug
* Linux: Build with makefile under carve/cmake_build
   * Static lib under cmake_build/lib


