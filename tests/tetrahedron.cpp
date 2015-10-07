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
#  include <carve_config.h>
#endif

#include <carve/vector.hpp>
#include <iostream>

double triangularPrismVolume(const carve::geom3d::Vector &a,
                             const carve::geom3d::Vector &b,
                             const carve::geom3d::Vector &c,
                             double z) {
  double v1 = carve::geom3d::tetrahedronVolume(carve::geom::VECTOR(a.x, a.y, z),
                                               carve::geom::VECTOR(c.x, c.y, z),
                                               carve::geom::VECTOR(b.x, b.y, z),
                                               a);
  double v2 = carve::geom3d::tetrahedronVolume(carve::geom::VECTOR(a.x, a.y, z),
                                               carve::geom::VECTOR(c.x, c.y, z),
                                               b,
                                               a);
  double v3 = carve::geom3d::tetrahedronVolume(carve::geom::VECTOR(c.x, c.y, z),
                                               c,
                                               b,
                                               a);

  std::cerr << "[components:" << v1 << "," << v2 << "," << v3 << "]" << std::endl;
  return v1 + v2 + v3;
}
                             
int main(int argc, char **argv) {
  std::cerr << "result: "
            << triangularPrismVolume(carve::geom::VECTOR(1,0,1),
                                     carve::geom::VECTOR(0,1,1),
                                     carve::geom::VECTOR(0,0,3),
                                     0)
            << std::endl;
  std::cerr << "result: "
            << triangularPrismVolume(carve::geom::VECTOR(11,10,1),
                                     carve::geom::VECTOR(10,11,1),
                                     carve::geom::VECTOR(10,10,3),
                                     0)
            << std::endl;
}
