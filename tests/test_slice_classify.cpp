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

#include <carve/csg.hpp>
#include <carve/csg_triangulator.hpp>

#include "geom_draw.hpp"
#include "geometry.hpp"
#include "scene.hpp"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <fstream>
#include <set>
#include <string>
#include <utility>

#include <time.h>

struct TestScene : public Scene {
  GLuint draw_list_base;
  std::vector<bool> draw_flags;

  bool key(unsigned char k, int x, int y) override {
    const char* t;
    static const char* l = "1234567890!@#$%^&*()";
    t = strchr(l, k);
    if (t != NULL) {
      int layer = t - l;
      if (layer < draw_flags.size()) {
        draw_flags[layer] = !draw_flags[layer];
      }
    }
    return true;
  }

  GLvoid draw() override {
    for (int i = 0; i < draw_flags.size(); ++i) {
      if (draw_flags[i]) {
        glCallList(draw_list_base + i);
      }
    }
  }

  TestScene(int argc, char** argv, int n_dlist) : Scene(argc, argv) {
    draw_list_base = glGenLists(n_dlist);

    draw_flags.resize(n_dlist, false);
  }

  ~TestScene() override { glDeleteLists(draw_list_base, draw_flags.size()); }
};

#define POINTS 60

int main(int argc, char** argv) {
  carve::mesh::MeshSet<3>* a =
      makeCube(carve::math::Matrix::ROT(1.0, 1.0, 1.0, 1.0));

  std::vector<carve::mesh::MeshSet<3>::vertex_t> shape;

  for (int i = 0; i < POINTS; ++i) {
    double r = 2.0 + .4 * sin(i * 3 * M_TWOPI / POINTS) +
               .8 * sin(i * 5 * M_TWOPI / POINTS);
    shape.push_back(carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR(
        r * cos(i * M_TWOPI / POINTS), r * sin(i * M_TWOPI / POINTS), 0.0)));
  }
  std::vector<carve::mesh::MeshSet<3>::vertex_t*> face_verts;
  for (int i = 0; i < POINTS; ++i) {
    face_verts.push_back(&shape[i]);
  }
  std::vector<carve::mesh::MeshSet<3>::face_t*> faces;
  faces.push_back(new carve::mesh::MeshSet<3>::face_t(face_verts.begin(),
                                                      face_verts.end()));

  carve::mesh::MeshSet<3>* b = new carve::mesh::MeshSet<3>(faces);

  std::list<std::pair<carve::csg::FaceClass, carve::mesh::MeshSet<3>*> >
      b_sliced;

  carve::csg::CSG csg;

  csg.hooks.registerHook(new carve::csg::CarveTriangulator,
                         carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
  csg.sliceAndClassify(a, b, b_sliced);

  TestScene* scene = new TestScene(argc, argv, 6);

  glNewList(scene->draw_list_base + 0, GL_COMPILE);

  drawMeshSet(a, .4, .6, .8, 1.0);
  glEndList();

  glNewList(scene->draw_list_base + 1, GL_COMPILE);
  drawMeshSet(b, .8, .6, .4, 1.0);
  glEndList();

  glNewList(scene->draw_list_base + 2, GL_COMPILE);
  {
    int n = 0;
    for (std::list<std::pair<carve::csg::FaceClass,
                             carve::mesh::MeshSet<3>*> >::iterator i =
             b_sliced.begin();
         i != b_sliced.end(); ++i) {
      float r, g, b;
      switch ((*i).first) {
        case carve::csg::FACE_IN:
          r = 0.0;
          g = 0.0;
          b = 1.0;
          break;
        case carve::csg::FACE_OUT:
          r = 1.0;
          g = 0.0;
          b = 0.0;
          break;
        case carve::csg::FACE_ON_ORIENT_OUT:
          r = 1.0;
          g = 1.0;
          b = 0.0;
          break;
        case carve::csg::FACE_ON_ORIENT_IN:
          r = 0.0;
          g = 1.0;
          b = 1.0;
          break;
      }
      drawMeshSet((*i).second, r, g, b, 1.0);
      ++n;
    }
  }
  glEndList();

  glNewList(scene->draw_list_base + 3, GL_COMPILE);
  {
    int n = 0;
    for (std::list<std::pair<carve::csg::FaceClass,
                             carve::mesh::MeshSet<3>*> >::iterator i =
             b_sliced.begin();
         i != b_sliced.end(); ++i) {
      float r, g, b;
      switch ((*i).first) {
        case carve::csg::FACE_IN:
          r = 0.3;
          g = 0.3;
          b = 0.7;
          break;
        case carve::csg::FACE_OUT:
          r = 0.7;
          g = 0.3;
          b = 0.3;
          break;
        case carve::csg::FACE_ON_ORIENT_OUT:
          r = 0.7;
          g = 0.7;
          b = 0.3;
          break;
        case carve::csg::FACE_ON_ORIENT_IN:
          r = 0.3;
          g = 0.7;
          b = 0.7;
          break;
      }
      drawMeshSetWireframe((*i).second, -1, false, false);
      ++n;
    }
  }
  glEndList();

  scene->run();

  delete scene;

  return 0;
}
