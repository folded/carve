// Copyright 2006-2015 Tobias Sargeant (tobias.sargeant@gmail.com).
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#if defined(HAVE_CONFIG_H)
#include <carve_config.h>
#endif

#include <carve/rescale.hpp>

#include "mersenne_twister.h"

int main(int argc, char** argv) {
  MTRand rand;
  double sx, sy, sz;
  double minx, maxx, miny, maxy, minz, maxz;

  sx = rand.rand(1e5) + 1;
  sy = rand.rand(1e5) + 1;
  sz = rand.rand(1e5) + 1;

  minx = rand.rand(1e10) - sx / 2;
  maxx = minx + sx;
  miny = rand.rand(1e10) - sy / 2;
  maxy = miny + sy;
  minz = rand.rand(1e10) - sz / 2;
  maxz = minz + sz;

  if (minx > maxx) std::swap(minx, maxx);
  if (miny > maxy) std::swap(miny, maxy);
  if (minz > maxz) std::swap(minz, maxz);

  carve::rescale::rescale r(minx, miny, minz, maxx, maxy, maxz);
  carve::rescale::fwd fwd(r);
  carve::rescale::rev rev(r);

  std::cout << "x: [" << minx << "," << maxx << "]" << std::endl;
  std::cout << "y: [" << miny << "," << maxy << "]" << std::endl;
  std::cout << "z: [" << minz << "," << maxz << "]" << std::endl;
  std::cout << std::endl;
  std::cout << "r.dx=" << r.dx << " r.dy=" << r.dy << " r.dz=" << r.dz
            << " r.scale=" << r.scale << std::endl;
  std::cout << std::endl;

  for (int i = 0; i < 10000; i++) {
    carve::geom3d::Vector in, temp, out;
    in.x = rand.rand(maxx - minx) + minx;
    in.y = rand.rand(maxy - miny) + miny;
    in.z = rand.rand(maxz - minz) + minz;
    temp = fwd(in);
    out = rev(temp);
    std::cout << in << " -> " << temp << " -> " << out << std::endl;
    CARVE_ASSERT(fabs(temp.x) < 1.0 && fabs(temp.y) < 1.0 &&
                 fabs(temp.z) < 1.0);
    CARVE_ASSERT(out.x == in.x && out.y == in.y && out.z == in.z);
  }
}
