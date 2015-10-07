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
#include <carve/csg.hpp>
#include <carve/input.hpp>

#include <map>

static carve::mesh::MeshSet<3> *makeCube(const carve::math::Matrix &transform) {
  carve::input::PolyhedronData data;

  data.addVertex(transform * carve::geom::VECTOR(+1.0, +1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, +1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, -1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, -1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, +1.0, -1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, +1.0, -1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, -1.0, -1.0));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, -1.0, -1.0));

  data.addFace(0, 1, 2, 3);
  data.addFace(7, 6, 5, 4);
  data.addFace(0, 4, 5, 1);
  data.addFace(1, 5, 6, 2);
  data.addFace(2, 6, 7, 3);
  data.addFace(3, 7, 4, 0);

  return new carve::mesh::MeshSet<3>(data.points, data.getFaceCount(), data.faceIndices);
}

struct ResultFaceHook : public carve::csg::CSG::Hook {
  std::map<const carve::mesh::MeshSet<3> *, int> &counter;

  ResultFaceHook(std::map<const carve::mesh::MeshSet<3> *, int> &_counter) : counter(_counter) {
  }

  virtual void resultFace(const carve::mesh::MeshSet<3>::face_t *output_face,
      const carve::mesh::MeshSet<3>::face_t *source_face,
      bool flipped) {
    const carve::mesh::MeshSet<3> *source_poly = static_cast<const carve::mesh::MeshSet<3> *>(source_face->mesh->meshset);
    counter[source_poly]++;
  }
};

TEST(HookTest, ResultFace) {
  std::map<const carve::mesh::MeshSet<3> *, int> counter;
  carve::csg::CSG csg;

  carve::mesh::MeshSet<3> *a = makeCube(carve::math::Matrix::SCALE(+5, +5, .5));
  carve::mesh::MeshSet<3> *b = makeCube(carve::math::Matrix::ROT(.5, +1, +1, +1));

  csg.hooks.registerHook(new ResultFaceHook(counter), carve::csg::CSG::Hooks::RESULT_FACE_BIT);
  csg.compute(a, b, carve::csg::CSG::UNION, NULL, carve::csg::CSG::CLASSIFY_EDGE);

  ASSERT_EQ(counter.size(), 2);
  ASSERT_TRUE(counter.find(a) != counter.end());
  ASSERT_TRUE(counter.find(b) != counter.end());

  ASSERT_EQ(counter[a], 6);
  ASSERT_EQ(counter[b], 10);
}
