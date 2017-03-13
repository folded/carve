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

#include <carve/aabb.hpp>
#include <carve/geom3d.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  carve::geom3d::AABB aabb(carve::geom::VECTOR(0, 0, 0),
                           carve::geom::VECTOR(1, 1, 1));

  std::ifstream in("aabb.test");
  while (in.good()) {
    double x1, y1, z1;
    double x2, y2, z2;
    char ray_intersects[10];
    char lineseg_intersects[10];
    bool ri;
    bool li;
    std::string line;

    std::getline(in, line);
    sscanf(line.c_str(), "<%lf,%lf,%lf>\t<%lf,%lf,%lf>\t%s\t%s", &x1, &y1, &z1,
           &x2, &y2, &z2, ray_intersects, lineseg_intersects);

    carve::geom3d::Vector v1 = carve::geom::VECTOR(x1, y1, z1);
    carve::geom3d::Vector v2 = carve::geom::VECTOR(x2, y2, z2);

    carve::geom3d::Ray r(v2 - v1, v1);
    carve::geom3d::LineSegment l(v1, v2);

    ri = !std::strcmp(ray_intersects, "True");
    li = !std::strcmp(lineseg_intersects, "True");

    bool ri_t = aabb.intersects(r);
    bool li_t = aabb.intersectsLineSegment(l.v1, l.v2);

    if (li != li_t || ri != ri_t) std::cout << line << std::endl;

    if (ri != ri_t) {
      std::cout << "RAY: " << ri << " " << ri_t << std::endl;
    }
    if (li != li_t) {
      std::cout << "LINE: " << li << " " << li_t << std::endl;
      std::cout << "LINE MIDPOINT = " << l.midpoint.asStr() << std::endl;
      aabb.intersectsLineSegment(l.v1, l.v2);
    }
    if (li != li_t || ri != ri_t) std::cout << std::endl;
  }
}
