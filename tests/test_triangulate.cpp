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

#include <carve/geom2d.hpp>
#include <carve/triangulator.hpp>

#include "coords.h"
#include "geom_draw.hpp"
#include "scene.hpp"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <iostream>

struct TestScene : public Scene {
  GLuint d_list;

  virtual bool key(unsigned char k, int x, int y) {
    return true;
  }

  virtual GLvoid draw() {
    glCallList(d_list);
  }

  TestScene(int argc, char **argv) : Scene(argc, argv) {
    d_list = glGenLists(1);
  }

  virtual ~TestScene() {
    glDeleteLists(d_list, 1);
  }
};

int main(int argc, char **argv) {
  TestScene *scene = new TestScene(argc, argv);

  std::vector<carve::geom2d::P2> poly;

  switch (3) {
  case 0: {
    poly.push_back(carve::geom::VECTOR(0,0));
    poly.push_back(carve::geom::VECTOR(1,0));
    poly.push_back(carve::geom::VECTOR(1,1));
    poly.push_back(carve::geom::VECTOR(0,1));
    break;
  }
  case 1: {
    poly.push_back(carve::geom::VECTOR(0,0));
    poly.push_back(carve::geom::VECTOR(1,0));
    poly.push_back(carve::geom::VECTOR(1,.2));
    poly.push_back(carve::geom::VECTOR(.2,.2));
    poly.push_back(carve::geom::VECTOR(.2,.8));
    poly.push_back(carve::geom::VECTOR(1,.8));
    poly.push_back(carve::geom::VECTOR(1,1));
    poly.push_back(carve::geom::VECTOR(0,1));
    break;
  }
  case 2: {
    size_t N = sizeof(map) / sizeof(map[0]);
    poly.reserve(N);
    for (size_t i = 0; i < N; ++i) {
      poly.push_back(carve::geom::VECTOR(map[i][0], map[i][1]));
    }
    break;
  }
  case 3: {
    size_t N = sizeof(floral) / sizeof(floral[0]);
    poly.reserve(N);
    for (size_t i = 0; i < N; ++i) {
      poly.push_back(carve::geom::VECTOR(floral[i][0], floral[i][1]));
    }
    break;
  }
  }


  std::vector<carve::triangulate::tri_idx> result;

  try {
    result.clear();
    carve::triangulate::triangulate(poly, result);
    carve::triangulate::improve(poly, result);
  } catch (carve::exception exc) {
    std::cerr << "FAIL: " << exc.str() << std::endl;
  }

  carve::geom::aabb<2> aabb;
  aabb.fit(poly.begin(), poly.end());
  double scale = 20.0 / std::max(aabb.extent.x, aabb.extent.y);

  glNewList(scene->d_list, GL_COMPILE);
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < poly.size(); ++i) {
    glColor4f(1, 1, 1, 1);
    glVertex3f((poly[i].x - aabb.pos.x) * scale, (poly[i].y - aabb.pos.y) * scale, 2.0);
  }
  glEnd();

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < result.size(); ++i) {
    glColor4f(.5 + .5 * sin(i / 1.1),
              .5 + .5 * sin(1.3 + i / 3.0),
              .5 + .5 * sin(.6 + i / 11.0), 1);
    unsigned i1 = result[i].a;
    unsigned i2 = result[i].b;
    unsigned i3 = result[i].c;
    glVertex3f((poly[i1].x - aabb.pos.x) * scale, (poly[i1].y - aabb.pos.y) * scale, 2.1);
    glVertex3f((poly[i2].x - aabb.pos.x) * scale, (poly[i2].y - aabb.pos.y) * scale, 2.1);
    glVertex3f((poly[i3].x - aabb.pos.x) * scale, (poly[i3].y - aabb.pos.y) * scale, 2.1);
  }
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glEndList();

  scene->run();

  delete scene;

  return 0;
}
