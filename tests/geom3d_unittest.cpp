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
#include <carve/geom.hpp>
#include <carve/geom2d.hpp>
#include <carve/geom3d.hpp>
#include <carve/matrix.hpp>

using namespace carve::geom;
using namespace carve::geom3d;

#include <random>

std::mt19937 rng;
std::normal_distribution<double> norm;

Vector randomUnitVector() {
  Vector vec = VECTOR(norm(rng), norm(rng), norm(rng));
  vec.normalize();
  return vec;
}

Vector randomPerpendicularVector(const Vector &p) {
  Vector t;
  do {
    t = randomUnitVector();
  } while (fabs(carve::geom::dot(p, t)) > 1 - 1e-5);
  return cross(t, p).normalized();
}

carve::math::Matrix3 randomRotation() {
  Vector vx = randomUnitVector();
  Vector vy = randomPerpendicularVector(vx);
  Vector vz = cross(vx, vy).normalized();

  carve::math::Matrix3 m;
  m._11 = vx.x; m._12 = vx.y; m._13 = vx.z;
  m._21 = vy.x; m._22 = vy.y; m._23 = vy.z;
  m._31 = vz.x; m._32 = vz.y; m._33 = vz.z;
  return m;
}

// static inline double antiClockwiseAngle(const Vector &from, const Vector &to, const Vector &orient)

void checkOrderInvariance(const Vector &dir,
                          const Vector &base,
                          const Vector &a,
                          const Vector &b) {
  ASSERT_EQ(compareAngles(dir, base, a, b), -compareAngles(dir, base, b, a));
}

void checkReflectionInvariance(const Vector &dir,
                               const Vector &base,
                               const Vector &a,
                               const Vector &b) {
  ASSERT_EQ(compareAngles(dir, base, a, b), -compareAngles(-dir, base, a, b));
}

void checkRotationInvariance(const Vector &dir,
                             const Vector &base,
                             const Vector &a,
                             const Vector &b,
                             size_t n) {
  for (size_t i = 0; i < n; ++i) {
    carve::math::Matrix3 rot = randomRotation();
    ASSERT_EQ(compareAngles(dir, base, a, b),
              compareAngles(rot * dir, rot * base, rot * a, rot * b));

  }
}

void checkInvariance(const Vector &dir,
                     const Vector &base,
                     const Vector &a,
                     const Vector &b) {
  checkOrderInvariance(dir, base, a, b);
  checkReflectionInvariance(dir, base, a, b);
  checkRotationInvariance(dir, base, a, b, 1000);

}

int sign(double d) {
  return (d > 0) ? +1 : (d == 0) ? 0 : -1;
}

TEST(GeomTest, compareAnglesFailure0) {
  using namespace carve::geom3d;
  Vector v_dir  = VECTOR( 0, 0, 1);
  Vector v_base = VECTOR(-1, 0, 0);
  Vector v_a = VECTOR( 0.305679278186268776895673, -0.952134538228459503805823, 0);
  Vector v_b = VECTOR(-0.305679278186268721384522,  0.95213453822845939278352,  0);
  ASSERT_EQ(compareAngles(v_dir, v_base, v_a, v_b), -1);
}

TEST(GeomTest, compareAnglesFailure1) {
  using namespace carve::geom3d;
  Vector v_dir  = VECTOR(-4.44089209850062616169453e-16,0,1);
  Vector v_base = VECTOR(-0.952134538228459281761218,-0.305679278186269054451429,-1.40944224917610240079275e-16);
  Vector v_a    = VECTOR(0.952134538228459281761218,0.305679278186268998940278,-1.06444930801409522585195e-16);
  Vector v_b    = VECTOR(-0.952134538228459281761218,-0.305679278186269054451429,-1.40944224917610240079275e-16);
  ASSERT_EQ(compareAngles(v_dir, v_base, v_a, v_b), +1);
}

TEST(GeomTest, Geom3D) {
  using namespace carve::geom3d;

  ASSERT_EQ(0.0, dot(VECTOR(1,1,1), VECTOR(-2,0,2)));

  // 1 2 3  =>  +(12-15), -(6-12), +(5-8)
  // 4 5 6
  ASSERT_EQ(VECTOR(-3, +6, -3), cross(VECTOR(1,2,3), VECTOR(4,5,6)));

  // should return 0 - a == b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(+1,0,0), VECTOR(+1,0,0)), 0);
  // should return 0 - a == b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(-1,0,0), VECTOR(-1,0,0)), 0);
  // should return 0 - a == b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(+1,+1,0), VECTOR(+1,+1,0)), 0);
  // should return +1 - a > b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(-1,+1,0), VECTOR(+1,+1,0)), +1);
  // should return -1 - a < b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(+1,+1,0), VECTOR(-1,+1,0)), -1);
  // should return +1 - a > b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(-1,0,0), VECTOR(+1,0,0)), +1);
  // should return -1 - a < b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(+1,0,0), VECTOR(-1,0,0)), -1);
  // +1 a > b
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(-1,-1,0), VECTOR(-1,+1,0)), +1);
  // -1 a < b
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(-1,-1,0), VECTOR(+1,-1,0)), -1);
  // should return +1 - a > b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(-1,0,0), VECTOR(0,1,0)), +1);
  // should return -1 - a < b.
  ASSERT_EQ(compareAngles(VECTOR(0,0,1), VECTOR(1,0,0), VECTOR(0,1,0), VECTOR(-1,0,0)), -1);

  for (size_t i = 0; i < 100; ++i) {
    Vector dir = randomUnitVector();
    dir = VECTOR(0,0,1);
    Vector base = randomPerpendicularVector(dir);
    Vector a = randomPerpendicularVector(dir);
    Vector b = randomPerpendicularVector(dir);
    double da = antiClockwiseAngle(base, a, dir);
    double db = antiClockwiseAngle(base, b, dir);
    // std::cerr << "dir=" << dir << " base=" << base << " a=" << a << " b=" << b << std::endl;
    // std::cerr << compareAngles(dir, base, a, b) << " " << sign(da - db) << " " << da << " " << db << std::endl;
    checkInvariance(dir, base, a, b);
  }
}

