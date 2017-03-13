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

#include <gtest/gtest.h>

#if defined(HAVE_CONFIG_H)
#include <carve_config.h>
#endif

#include <carve/carve.hpp>
#include <carve/triangle_intersection.hpp>

#include <fstream>

typedef carve::geom::vector<2> vec2;
typedef carve::geom::vector<2> vec3;

std::istream& operator>>(std::istream& in, vec2& vec) {
  char c = 0;
  in >> vec.x;
  if (in >> c && c != ',') in.setstate(std::ios_base::failbit);
  in >> vec.y;
  return in;
}

std::ostream& operator<<(std::ostream& out, const vec2& vec) {
  out << vec.x << "," << vec.y;
  return out;
}

TEST(TriangleIntersectionTest, Test3D) {
  vec3 tri_a[3], tri_b[3];
  tri_a[0] = carve::geom::VECTOR(0, 1, 0);
  tri_a[1] = carve::geom::VECTOR(1, 0.5, 0);
  tri_a[2] = carve::geom::VECTOR(0.5, 0.75, 1);

  tri_b[0] = carve::geom::VECTOR(2, 1, 1);
  tri_b[1] = carve::geom::VECTOR(1, 0.5, 1);
  tri_b[2] = carve::geom::VECTOR(1.5, 0.75, 0);
}

TEST(TriangleIntersectionTest, Test2D) {
  std::ifstream in("intersection_2d.txt");

  while (in.good()) {
    char c;
    vec2 t1[3], t2[3];

    in >> c >> t1[0] >> t1[1] >> t1[2] >> t2[0] >> t2[1] >> t2[2];
    if (!in.eof()) {
      ASSERT_EQ(
          carve::geom::triangle_intersection(t1, t2),
          (c == 't') ? carve::geom::TR_INT_INT : carve::geom::TR_INT_NONE);
    }
  }
}
