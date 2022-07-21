/* include/carve/config.h.  Generated from config.h.in by configure.  */
#pragma once

#include <math.h>

#if defined(_MSC_VER)
#pragma warning(disable : 4201)
#endif

#include <math.h>

// These definitions are already provided by Visual Studio 2022
#if (defined _MSC_VER) && _MSC_VER >= 1929

#else

static inline double round(double value) {
  return (value >= 0) ? floor(value + 0.5) : ceil(value - 0.5);
}

#endif

typedef int ssize_t;
