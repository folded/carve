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
#  include <carve_config.h>
#endif

#include <carve/carve.hpp>
#include <carve/triangulator.hpp>
#include <carve/geom.hpp>

TEST(Triangulate, Test2) {
  std::vector<carve::geom::vector<2> > poly;
  std::vector<carve::triangulate::tri_idx> result;

  poly.push_back(carve::geom::VECTOR(0.0614821249999999985824672,0.0192046249999999994739763));
  poly.push_back(carve::geom::VECTOR(0.0480376250000000004636291,0.0355867500000000003268497));
  poly.push_back(carve::geom::VECTOR(0.0316555000000000030802028,0.0490312499999999984456878));
  poly.push_back(carve::geom::VECTOR(0.0480376250000000004636291,0.0355867500000000003268497));

  carve::triangulate::triangulate(poly, result);
}

TEST(Triangulate, Test1) {
  std::vector<carve::geom::vector<2> > poly;
  std::vector<carve::triangulate::tri_idx> result;

  poly.push_back(carve::geom::VECTOR(-0.0197657499999999985984545,-0.00112325000000000618793905));
  poly.push_back(carve::geom::VECTOR(-0.0197657499999999985984545,-0.0562291249999999978581577));
  poly.push_back(carve::geom::VECTOR(0.000514374999999999966797393,-0.0562291249999999978581577));
  poly.push_back(carve::geom::VECTOR(0.000514374999999999966797393,-0.00112324999999999924904515));

  carve::triangulate::triangulate(poly, result);
}
