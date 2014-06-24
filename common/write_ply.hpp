// Begin License:
// Copyright (C) 2006-2014 Tobias Sargeant (tobias.sargeant@gmail.com).
// All rights reserved.
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// This file may be used under the terms of either the GNU General
// Public License version 2 or 3 (at your option) as published by the
// Free Software Foundation and appearing in the files LICENSE.GPL2
// and LICENSE.GPL3 included in the packaging of this file.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// End:


#pragma once

#include <carve/carve.hpp>

#include <carve/poly.hpp>
#include <carve/polyline.hpp>
#include <carve/pointset.hpp>

#include <ostream>
#include <fstream>



void writePLY(std::ostream &out, const carve::mesh::MeshSet<3> *poly, bool ascii = false);
void writePLY(const std::string &out_file, const carve::mesh::MeshSet<3> *poly, bool ascii = false);

void writePLY(std::ostream &out, const carve::poly::Polyhedron *poly, bool ascii = false);
void writePLY(const std::string &out_file, const carve::poly::Polyhedron *poly, bool ascii = false);

void writePLY(std::ostream &out, const carve::line::PolylineSet *lines, bool ascii = false);
void writePLY(const std::string &out_file, const carve::line::PolylineSet *lines, bool ascii = false);

void writePLY(std::ostream &out, const carve::point::PointSet *points, bool ascii = false);
void writePLY(const std::string &out_file, const carve::point::PointSet *points, bool ascii = false);



void writeOBJ(std::ostream &out, const carve::mesh::MeshSet<3> *poly);
void writeOBJ(const std::string &out_file, const carve::mesh::MeshSet<3> *poly);

void writeOBJ(std::ostream &out, const carve::poly::Polyhedron *poly);
void writeOBJ(const std::string &out_file, const carve::poly::Polyhedron *poly);

void writeOBJ(std::ostream &out, const carve::line::PolylineSet *lines);
void writeOBJ(const std::string &out_file, const carve::line::PolylineSet *lines);



void writeVTK(std::ostream &out, const carve::mesh::MeshSet<3> *poly);
void writeVTK(const std::string &out_file, const carve::mesh::MeshSet<3> *poly);

void writeVTK(std::ostream &out, const carve::poly::Polyhedron *poly);
void writeVTK(const std::string &out_file, const carve::poly::Polyhedron *poly);

void writeVTK(std::ostream &out, const carve::line::PolylineSet *lines);
void writeVTK(const std::string &out_file, const carve::line::PolylineSet *lines);
