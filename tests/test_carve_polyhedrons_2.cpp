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

#include <carve/csg.hpp>
#include <carve/geom.hpp>
#include <carve/poly.hpp>

#include <iostream>
#include <vector>

int main() {
  // create a tetrahedron
  std::vector<carve::mesh::MeshSet<3>::vertex_t> tet_verts;
  std::vector<carve::mesh::MeshSet<3>::face_t*> tet_faces;
  std::vector<carve::mesh::MeshSet<3>::vertex_t*> corners;

  tet_verts.push_back(
      carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR(0.0, 0.0, 0.0)));
  tet_verts.push_back(
      carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR(1.0, 0.0, 0.0)));
  tet_verts.push_back(
      carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR(0.0, 1.0, 0.0)));
  tet_verts.push_back(
      carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR(0.0, 0.0, 1.0)));

  corners.push_back(&tet_verts[0]);
  corners.push_back(&tet_verts[2]);
  corners.push_back(&tet_verts[1]);
  tet_faces.push_back(
      new carve::mesh::MeshSet<3>::face_t(corners.begin(), corners.end()));

  corners.clear();
  corners.push_back(&tet_verts[0]);
  corners.push_back(&tet_verts[1]);
  corners.push_back(&tet_verts[3]);
  tet_faces.push_back(
      new carve::mesh::MeshSet<3>::face_t(corners.begin(), corners.end()));

  corners.clear();
  corners.push_back(&tet_verts[0]);
  corners.push_back(&tet_verts[3]);
  corners.push_back(&tet_verts[2]);
  tet_faces.push_back(
      new carve::mesh::MeshSet<3>::face_t(corners.begin(), corners.end()));

  corners.clear();
  corners.push_back(&tet_verts[1]);
  corners.push_back(&tet_verts[2]);
  corners.push_back(&tet_verts[3]);
  tet_faces.push_back(
      new carve::mesh::MeshSet<3>::face_t(corners.begin(), corners.end()));

  carve::mesh::MeshSet<3> tetrahedron(tet_faces);

  // create a triangle
  std::vector<carve::mesh::MeshSet<3>::vertex_t> tri_verts;
  std::vector<carve::mesh::MeshSet<3>::face_t*> tri_faces;

  // Vertices
  // crashes if last coordinate set to 1e-8, but ok for 1e-7
  tri_verts.push_back(
      carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR(-0.3, 0.0, 1e-8)));
  tri_verts.push_back(
      carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR(1.0, 0.0, 1.1e-8)));
  tri_verts.push_back(carve::mesh::MeshSet<3>::vertex_t(
      carve::geom::VECTOR(-0.3, 1.0, 1.1e-8)));

  // Face
  corners.clear();
  corners.push_back(&tri_verts[0]);
  corners.push_back(&tri_verts[2]);
  corners.push_back(&tri_verts[1]);
  tri_faces.push_back(
      new carve::mesh::MeshSet<3>::face_t(corners.begin(), corners.end()));

  //  corners.clear();
  //  corners.push_back(&tri_verts[0]);
  //  corners.push_back(&tri_verts[1]);
  //  corners.push_back(&tri_verts[2]);
  //  tri_faces.push_back(new carve::mesh::MeshSet<3>::face_t(corners));

  carve::mesh::MeshSet<3> triangle(tri_faces);

  // cut triangle with tetrahedron.
  carve::mesh::MeshSet<3>* is_poly = carve::csg::CSG().compute(
      &tetrahedron, &triangle, carve::csg::CSG::INTERSECTION);

  // std::cout << "Tetrahedron is ... \n" << tetrahedron;
  // std::cout << "Triangle is ... \n" << triangle;
  // std::cout << "Intersection is ... \n" << *is_poly;

  return 0;
}
