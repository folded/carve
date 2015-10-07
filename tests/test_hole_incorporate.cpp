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

#include <fstream>
#include <sstream>

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

#if defined(__GNUC__)
#define __stdcall
#endif

#if defined(GLU_TESS_CALLBACK_VARARGS)
  typedef GLvoid (_stdcall *GLUTessCallback)(...);
#else
    typedef void (__stdcall *GLUTessCallback)();
#endif

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

  typedef std::vector<carve::geom2d::P2> loop_t;
  std::vector<loop_t> poly;

  std::ifstream in(argv[1]);
  while (in.good()) {
    std::string s;
    std::getline(in, s);
    if (s == "BEGIN") {
      poly.push_back(loop_t());
    } else {
      std::istringstream in_s(s);
      double x,y;
      in_s >> x >> y;
      poly.back().push_back(carve::geom::VECTOR(x, y));
    }
  }

  std::vector<std::pair<size_t, size_t> > result;
  std::vector<carve::geom2d::P2> merged;
  std::vector<carve::triangulate::tri_idx> triangulated;

  try {
    result = carve::triangulate::incorporateHolesIntoPolygon(poly);
    merged.reserve(result.size());
    for (size_t i = 0; i < result.size(); ++i) {
      merged.push_back(poly[result[i].first][result[i].second]);
    }
    carve::triangulate::triangulate(merged, triangulated);
    carve::triangulate::improve(merged, triangulated);

  } catch (carve::exception exc) {
    std::cerr << "FAIL: " << exc.str() << std::endl;
    return -1;
  }

  carve::geom::aabb<2> aabb;
  aabb.fit(merged.begin(), merged.end());
  double scale = 20.0 / std::max(aabb.extent.x, aabb.extent.y);

  glNewList(scene->d_list, GL_COMPILE);

  glDisable(GL_LIGHTING);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(0.2, 0.3, 0.4, 1.0);

  glBegin(GL_TRIANGLES);
  for (size_t i = 0; i != triangulated.size(); ++i) {
    double x, y;
    x = (merged[triangulated[i].a].x - aabb.pos.x) * scale;
    y = (merged[triangulated[i].a].y - aabb.pos.y) * scale;
    glVertex3f(x, y, 0.0);
    x = (merged[triangulated[i].b].x - aabb.pos.x) * scale;
    y = (merged[triangulated[i].b].y - aabb.pos.y) * scale;
    glVertex3f(x, y, 0.0);
    x = (merged[triangulated[i].c].x - aabb.pos.x) * scale;
    y = (merged[triangulated[i].c].y - aabb.pos.y) * scale;
    glVertex3f(x, y, 0.0);
  }
  glEnd();

  glColor4f(0.0, 0.0, 0.0, 0.1);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  for (size_t i = 0; i != triangulated.size(); ++i) {
    double x, y;
    x = (merged[triangulated[i].a].x - aabb.pos.x) * scale;
    y = (merged[triangulated[i].a].y - aabb.pos.y) * scale;
    glVertex3f(x, y, 0.0);
    x = (merged[triangulated[i].b].x - aabb.pos.x) * scale;
    y = (merged[triangulated[i].b].y - aabb.pos.y) * scale;
    glVertex3f(x, y, 0.0);
    x = (merged[triangulated[i].c].x - aabb.pos.x) * scale;
    y = (merged[triangulated[i].c].y - aabb.pos.y) * scale;
    glVertex3f(x, y, 0.0);
  }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  glColor4f(1, 1, 1, 1);
  glBegin(GL_LINE_LOOP);
  for (size_t i = 0; i < merged.size(); ++i) {
    glVertex3f((merged[i].x - aabb.pos.x) * scale, (merged[i].y - aabb.pos.y) * scale, 2.0);
  }
  glEnd();

  glEndList();

  scene->run();

  delete scene;

  return 0;
}
