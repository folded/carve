// Copyright (c) 2006, Tobias Sargeant
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the
// distribution.  The names of its contributors may be used to endorse
// or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include <gloop/gloop.hpp>
#include <gloop/gloop-model.hpp>

#include <iostream>
#include <stdarg.h>
#include <stdio.h>

#ifdef WIN32

#if (defined _MSC_VER) && _MSC_VER >= 1929

#include <stdint.h>
#include <cstring>

#else

typedef char int8_t;
typedef short int16_t;
typedef long int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

#endif

#else 

#include <stdint.h>
#include <cstring>

#endif

namespace {

  const char *typestr(int t) {
    static const char *_typestr[] = { "char", "uchar", "short", "ushort", "int", "uint", "float", "double" };
    return _typestr[t];
  }

  int strtype(const std::string &s) {
    if (s == "uchar"  || s == "uint8"  ) return gloop::stream::U8;
    if (s == "ushort" || s == "uint16" ) return gloop::stream::U16;
    if (s == "uint"   || s == "uint32" ) return gloop::stream::U32;
    if (s == "int8"                    ) return gloop::stream::I8;
    if (s == "int16"                   ) return gloop::stream::I16;
    if (s == "int"    || s == "int32"  ) return gloop::stream::I32;
    if (s == "char"                    ) return gloop::stream::I8;
    if (s == "short"                   ) return gloop::stream::I16;
    if (s == "float"  || s == "float32") return gloop::stream::F32;
    if (s == "double" || s == "float64") return gloop::stream::F64;

    return -1;
  }

  inline bool is_big_endian() {
    long one= 1;
    return (*((char *)(&one))) == 0;
  }

  inline void _copy(char *a, const char *b, int size, bool swap) {
    if (swap) {
      for (b += size; size--; *a++ = *--b);
    } else {
      for (; size--; *a++ = *b++);
    }
  }

  template<typename T>
  bool _parse(std::istream &in, T &val);

  template<>
  bool _parse<int8_t>(std::istream &in, int8_t &val) {
    int32_t v; in >> v; if (v < -128 || v > 127) return false; val = (int8_t)v; return true;
  }

  template<>
  bool _parse<uint8_t>(std::istream &in, uint8_t &val) {
    uint32_t v; in >> v; if (v > 255) return false; val = (uint8_t)v; return true;
  }

  template<>
  bool _parse<int16_t>(std::istream &in, int16_t &val) {
    int32_t v; in >> v; if (v < -32768 || v > 32767) return false; val = (int16_t)v; return true;
  }

  template<>
  bool _parse<uint16_t>(std::istream &in, uint16_t &val) {
    uint32_t v; in >> v; if (v > 65535) return false; val = (uint16_t)v; return true;
  }

  template<>
  bool _parse<int32_t>(std::istream &in, int32_t &val) {
    in >> val; return true;
  }

  template<>
  bool _parse<uint32_t>(std::istream &in, uint32_t &val) {
    in >> val; return true;
  }

  template<>
  bool _parse<float>(std::istream &in, float &val) {
    in >> val; return true;
  }

  template<>
  bool _parse<double>(std::istream &in, double &val) {
    in >> val; return true;
  }

  template<typename T>
  bool _parse(std::istream &in, int type, T &out) {
    switch (type) {
    case gloop::stream::I8:  { int8_t   c; if (!_parse(in, c)) return false; out = T(c); return true; }
    case gloop::stream::I16: { int16_t  c; if (!_parse(in, c)) return false; out = T(c); return true; }
    case gloop::stream::I32: { int32_t  c; if (!_parse(in, c)) return false; out = T(c); return true; }
    case gloop::stream::U8:  { uint8_t  c; if (!_parse(in, c)) return false; out = T(c); return true; }
    case gloop::stream::U16: { uint16_t c; if (!_parse(in, c)) return false; out = T(c); return true; }
    case gloop::stream::U32: { uint32_t c; if (!_parse(in, c)) return false; out = T(c); return true; }
    case gloop::stream::F32: { float    c; if (!_parse(in, c)) return false; out = T(c); return true; }
    case gloop::stream::F64: { double   c; if (!_parse(in, c)) return false; out = T(c); return true; }
    default:         return false;
    }
  }

  template<typename U, typename T>
  inline void _read(const char *mem, bool byteswap, T &out) {
    U c;
    _copy((char *)&c, mem, sizeof(U), byteswap);
    out = T(c);
  }

  template<typename T>
  inline void _read(const char *mem, bool byteswap, int type, T &out) {
    switch (type) {
    case gloop::stream::I8:  _read<  int8_t>(mem, byteswap, out); return;
    case gloop::stream::U8:  _read< uint8_t>(mem, byteswap, out); return;
    case gloop::stream::I16: _read< int16_t>(mem, byteswap, out); return;
    case gloop::stream::U16: _read<uint16_t>(mem, byteswap, out); return;
    case gloop::stream::I32: _read< int32_t>(mem, byteswap, out); return;
    case gloop::stream::U32: _read<uint32_t>(mem, byteswap, out); return;
    case gloop::stream::F32: _read<   float>(mem, byteswap, out); return;
    case gloop::stream::F64: _read<  double>(mem, byteswap, out); return;
    }
  }

  template<typename U, typename T>
  inline void _write(char *mem, bool byteswap, const T &in) {
    U c = U(in);
    _copy(mem, (const char *)&c, sizeof(U), byteswap);
  }

  template<typename T>
  inline void _write(char *mem, bool byteswap, int type, const T &in) {
    switch (type) {
    case gloop::stream::I8:  _write<  int8_t>(mem, byteswap, in); return;
    case gloop::stream::U8:  _write< uint8_t>(mem, byteswap, in); return;
    case gloop::stream::I16: _write< int16_t>(mem, byteswap, in); return;
    case gloop::stream::U16: _write<uint16_t>(mem, byteswap, in); return;
    case gloop::stream::I32: _write< int32_t>(mem, byteswap, in); return;
    case gloop::stream::U32: _write<uint32_t>(mem, byteswap, in); return;
    case gloop::stream::F32: _write<   float>(mem, byteswap, in); return;
    case gloop::stream::F64: _write<  double>(mem, byteswap, in); return;
    }
  }

  template<typename T>
  inline bool prop_read(const char *mem, bool byteswap, gloop::stream::reader_base *rd) {
    T c;
    _copy((char *)&c, mem, sizeof(T), byteswap);
    rd->_val(c);
    return true;
  }

  bool prop_read(const char *mem, int type, bool byteswap, gloop::stream::reader_base *rd) {
    switch (type) {
    case gloop::stream::I8:  return prop_read<  int8_t>(mem, byteswap, rd);
    case gloop::stream::U8:  return prop_read< uint8_t>(mem, byteswap, rd);
    case gloop::stream::I16: return prop_read< int16_t>(mem, byteswap, rd);
    case gloop::stream::U16: return prop_read<uint16_t>(mem, byteswap, rd);
    case gloop::stream::I32: return prop_read< int32_t>(mem, byteswap, rd);
    case gloop::stream::U32: return prop_read<uint32_t>(mem, byteswap, rd);
    case gloop::stream::F32: return prop_read<   float>(mem, byteswap, rd);
    case gloop::stream::F64: return prop_read<  double>(mem, byteswap, rd);
    default:                 return false;
    }
  }

  template<typename T>
  inline bool prop_read(std::istream &in, gloop::stream::reader_base *rd) {
    T c;
    if (!_parse(in, c)) return false;
    rd->_val(c);
    return true;
  }

  bool prop_read(std::istream &in, int type, gloop::stream::reader_base *rd) {
    switch (type) {
    case gloop::stream::I8:  return prop_read<  int8_t>(in, rd);
    case gloop::stream::U8:  return prop_read< uint8_t>(in, rd);
    case gloop::stream::I16: return prop_read< int16_t>(in, rd);
    case gloop::stream::U16: return prop_read<uint16_t>(in, rd);
    case gloop::stream::I32: return prop_read< int32_t>(in, rd);
    case gloop::stream::U32: return prop_read<uint32_t>(in, rd);
    case gloop::stream::F32: return prop_read<   float>(in, rd);
    case gloop::stream::F64: return prop_read<  double>(in, rd);
    default:
      return false;
    }
  }

  template<typename T>
  inline bool prop_write(char *mem, bool byteswap, gloop::stream::writer_base *wt) {
    T c;
    wt->_val(c);
    _copy(mem, (const char *)&c, sizeof(c), byteswap);
    return true;
  }

  bool prop_write(char *mem, int type, bool byteswap, gloop::stream::writer_base *wt) {
    switch (type) {
    case gloop::stream::I8:  return prop_write<  int8_t>(mem, byteswap, wt);
    case gloop::stream::U8:  return prop_write< uint8_t>(mem, byteswap, wt);
    case gloop::stream::I16: return prop_write< int16_t>(mem, byteswap, wt);
    case gloop::stream::U16: return prop_write<uint16_t>(mem, byteswap, wt);
    case gloop::stream::I32: return prop_write< int32_t>(mem, byteswap, wt);
    case gloop::stream::U32: return prop_write<uint32_t>(mem, byteswap, wt);
    case gloop::stream::F32: return prop_write<   float>(mem, byteswap, wt);
    case gloop::stream::F64: return prop_write<  double>(mem, byteswap, wt);
    default:
      return false;
    }
  }

  bool prop_write(std::ostream &out, int type, gloop::stream::writer_base *wt) {
    switch (type) {
    case gloop::stream::I8:  { int8_t   c; wt->_val(c); out << (int32_t)c;  return out.good(); }
    case gloop::stream::U8:  { uint8_t  c; wt->_val(c); out << (uint32_t)c; return out.good(); }
    case gloop::stream::I16: { int16_t  c; wt->_val(c); out << (int32_t)c;  return out.good(); }
    case gloop::stream::U16: { uint16_t c; wt->_val(c); out << (uint32_t)c; return out.good(); }
    case gloop::stream::I32: { int32_t  c; wt->_val(c); out << (int32_t)c;  return out.good(); }
    case gloop::stream::U32: { uint32_t c; wt->_val(c); out << (uint32_t)c; return out.good(); }
    case gloop::stream::F32: { float    c; wt->_val(c); out << (float)c;    return out.good(); }
    case gloop::stream::F64: { double   c; wt->_val(c); out << (double)c;   return out.good(); }
    default: return false;
    }
  }

  bool _skip(std::istream &in, int type) {
    switch (type) {
    case gloop::stream::I8:
    case gloop::stream::I16:
    case gloop::stream::I32: {
      int32_t v;
      in >> v;
      return !in.fail();
    }
    case gloop::stream::U8:
    case gloop::stream::U16:
    case gloop::stream::U32: {
      uint32_t v;
      in >> v;
      return !in.fail();
    }
    case gloop::stream::F32:
    case gloop::stream::F64: {
      double v;
      in >> v;
      return !in.fail();
    }
    default:
      return false;
    }
  }

  struct prop_info {
    std::string name;
    int count_type;
    int type;
    bool is_list;
    gloop::stream::reader_base *rd;
    gloop::stream::writer_base *wt;

    prop_info(const std::string &s) : name(), count_type(0), type(0), is_list(false), rd(NULL), wt(NULL) {
      std::istringstream in(s);
      std::string tok;

      name = "";
      count_type = -1;
      type = -1;
      is_list = false;

      in >> tok;
      if (tok == "property") {
        in >> tok;
        if (tok == "list") {
          std::string s_count_type, s_type, s_name;
          in >> s_count_type >> s_type >> s_name;
          if (!in.fail()) {
            name = s_name;
            count_type = strtype(s_count_type);
            type = strtype(s_type);
            is_list = true;
          }
        } else {
          std::string s_type, s_name;
          s_type = tok;
          in >> s_name;
          if (!in.fail()) {
            name = s_name;
            if (name == "vertex_index") name = "vertex_indices";
            type = strtype(s_type);
          }
        }
      }
    }

    bool writeBinary(std::ostream &out, bool byteswap) const {
      if (wt == NULL) {
        return false;
      }
      char buf[8];
      size_t sz = gloop::stream::type_size(type);
      size_t len = 1;
      
      wt->begin();
      if (is_list) {
        size_t cts = gloop::stream::type_size(count_type);
        len = wt->length();
        _write(buf, byteswap, count_type, len);
        out.write(buf, cts);
        if (out.fail()) {
          wt->fail();
          return false;
        }
      }
      for (size_t i = 0; i < len; ++i) {
        wt->next();
        prop_write(buf, type, byteswap, wt);
        out.write(buf, sz);
        if (out.fail()) {
          wt->fail();
          return false;
        }
      }
      wt->end();
      return true;
    }

    bool writeAscii(std::ostream &out) const {
      if (wt == NULL) {
        return false;
      }
      size_t len = 1;
      bool first = true;

      wt->begin();
      if (is_list) {
        len = wt->length();
        out << len; first = false;
      }
      for (size_t i = 0; i < len; ++i) {
        wt->next();
        if (first) { first = false; } else { out << " "; }
        prop_write(out, type, wt);
      }
      wt->end();
      return true;
    }

    bool readBinary(std::istream &in, bool byteswap) const {
      size_t len = 1;
      size_t sz = gloop::stream::type_size(type);
      
      char buf[8];
      if (is_list) {
        size_t cts = gloop::stream::type_size(count_type);
        in.read(buf, cts); if (in.gcount() != (std::streamsize)cts) return false;
        _read(buf, byteswap, count_type, len);
      }
      if (rd == NULL) {
        in.seekg(len * sz, std::ios_base::cur);
      } else {
        rd->begin();
        rd->length(len);
        for (size_t i = 0; i < len; ++i) {
          rd->next();
          in.read(buf, sz);
          if (in.gcount() != (std::streamsize)sz) {
            rd->fail();
            return false;
          }
          prop_read(buf, type, byteswap, rd);
        }
        rd->end();
      }
      return true;
    }
    
    bool readAscii(std::istream &in) const {
      size_t len = 1;
      
      if (is_list) {
        if (!_parse(in, count_type, len)) return false;
      }
      if (rd != NULL) {
        rd->begin();
        rd->length(len);
        for (size_t i = 0; i < len; ++i) {
          rd->next();
          if (!prop_read(in, type, rd)) {
            rd->fail();
            return false;
          }
        }
        rd->end();
      } else {
        for (size_t i = 0; i < len; ++i) {
          if (!_skip(in, type)) {
            return false;
          }
        }
      }
      return true;
    }

    std::string header() const {
      std::ostringstream s;
      if (is_list) {
        s << "property list " << typestr(count_type) << " " << typestr(type) << " " << name;
      } else {
        s << "property " << typestr(type) << " " << name;
      }

      return s.str();
    }
  };



  struct elem_info {
    std::string name;
    int count;
    std::vector<prop_info> props;
    gloop::stream::reader_base *rd;
    gloop::stream::writer_base *wt;

    bool match(const char *elem_name, ...) const {
      if (elem_name != name) {
        return false;
      }
      bool ok = true;
      va_list args;
      va_start(args, elem_name);
      while (ok) {
        const char *prop = va_arg(args, const char *);
        if (!prop) break;
        ok = false;
        for (size_t i = 0; i < props.size(); ++i) {
          if (props[i].name == prop) {
            ok = true;
            break;
          }
        }
      }
      va_end(args);
      return ok;
    }

    elem_info(const std::string &s) : name(), count(0), props(), rd(NULL), wt(NULL) {
      std::istringstream in(s);
      std::string tok;

      name = "";
      count = -1;

      in >> tok;
      if (tok == "element") {
        std::string s_name;
        size_t s_count;
        in >> s_name;
        in >> s_count;
        if (!in.fail()) {
          name = s_name;
          count = s_count;
        }
      }
    }

    std::string header() const {
      std::ostringstream s;
      s << "element " << name << " " << count;
      return s.str();
    }
  };



  std::list<elem_info>::iterator matchPolyhedronElements(std::list<elem_info>::iterator b,
                                                         std::list<elem_info>::iterator e) {
    std::list<elem_info>::iterator i = b;
    if (i == e || !(*i).match("vertex", "x", "y", "z", NULL)) return b;
    ++i;
    int face_elem_count = 0;
    while (i != e) {
      if ((*i).match("face", "vertex_indices", NULL) ||
          (*i).match("tristrips", "vertex_indices", NULL)) {
        ++i;
        ++face_elem_count;
      } else {
        break;
      }
    }
    if (face_elem_count == 0) return b;
    return i;
  }



  std::list<elem_info>::iterator matchPolylineElements(std::list<elem_info>::iterator b,
                                                       std::list<elem_info>::iterator e) {
    std::list<elem_info>::iterator i = b;
    if (i == e || !(*i).match("vertex", "x", "y", "z", NULL)) return b;
    ++i;
    if (i == e || !(*i).match("polyline", "closed", "vertex_indices", NULL)) {
      return b;
    }
    return ++i;
  }



  std::list<elem_info>::iterator matchPointSetElements(std::list<elem_info>::iterator b,
                                                       std::list<elem_info>::iterator e) {
    std::list<elem_info>::iterator i = b;
    if (i == e || !(*i).match("vertex", "x", "y", "z", NULL)) return b;
    return ++i;
  }



  bool readBlock(std::istream &in,
                 bool binary,
                 bool byteswap,
                 gloop::stream::block_t *base,
                 std::list<elem_info>::iterator b,
                 std::list<elem_info>::iterator e) {
    bool result = true;
    if (base && base->rd != NULL) base->rd->begin();
    while (b != e) {
      gloop::stream::named_element_t *elem = NULL;
      if (base) elem = base->findElem((*b).name);

      if (elem != NULL && elem->rd != NULL) {
        (*b).rd = elem->rd.ptr();
        elem->rd->begin();
        elem->rd->length((*b).count);
      }

      for (size_t j = 0; j < (*b).props.size(); ++j) {
        prop_info &pi = (*b).props[j];
        gloop::stream::named_prop_t *prop = NULL;
        if (elem) prop = elem->findProp(pi.name);
        if (prop) pi.rd = prop->rd.ptr();
      }

      for (int i = 0; i < (*b).count; ++i) {
        if ((*b).rd) (*b).rd->next();
        for (size_t j = 0; j < (*b).props.size(); ++j) {
          prop_info &pi = (*b).props[j];
          bool ok;
          if (binary) {
            ok = pi.readBinary(in, byteswap);
          } else {
            ok = pi.readAscii(in);
          }
          if (!ok) {
            if ((*b).rd) (*b).rd->fail();
            result = false;
            goto done;
          }
        }
      }

      if ((*b).rd) (*b).rd->end();

      ++b;
    }

  done:;
    if (base && base->rd != NULL) {
      if (result) {
        base->rd->end();
      } else {
        base->rd->fail();
      }
    }

    return result;
  }

  bool readPolyhedron(std::istream &in,
                      bool binary,
                      bool byteswap,
                      gloop::ply::PlyReader &reader,
                      std::list<elem_info>::iterator b,
                      std::list<elem_info>::iterator e) {
    return readBlock(in,
                     binary,
                     byteswap,
                     reader.findBlock("polyhedron"),
                     b,
                     e);
  }



  bool readPolyline(std::istream &in,
                    bool binary,
                    bool byteswap,
                    gloop::ply::PlyReader &reader,
                    std::list<elem_info>::iterator b,
                    std::list<elem_info>::iterator e) {
    return readBlock(in,
                     binary,
                     byteswap,
                     reader.findBlock("polyline"),
                     b,
                     e);
  }



  bool readPointSet(std::istream &in,
                    bool binary,
                    bool byteswap,
                    gloop::ply::PlyReader &reader,
                    std::list<elem_info>::iterator b,
                    std::list<elem_info>::iterator e) {
    return readBlock(in,
                     binary,
                     byteswap,
                     reader.findBlock("pointset"),
                     b,
                     e);
  }

}

namespace gloop {
  namespace ply {

    bool PlyReader::read(std::istream &in) {
      bool binary = false;
      bool byteswap = false;

      std::string s;
      std::getline(in, s);
      if (str::rstrip(s) != "ply") {
        throw exception("bad PLY header");
      }

      std::getline(in, s);
      std::istringstream ins(s);
      std::string tok;
      ins >> tok;
      if (tok == "format") {
        std::string fmt, ver;
        ins >> fmt >> ver;
        if (!ins.fail()) {
          if (fmt == "ascii") {
            binary = false;
          } else if (fmt == "binary_big_endian") {
            binary = true;
            byteswap = !is_big_endian();
          } else if (fmt == "binary_little_endian") {
            binary = true;
            byteswap = is_big_endian();
          } else {
            throw exception(str::format() << "bad PLY format [" <<  fmt << "]");
          }
        }
      } else {
        throw exception(str::format() << "bad PLY header [" << s << "]");
      }

      std::list<elem_info> elements;

      for (;;) {
        std::getline(in, s);
        if (str::startswith(s, "end_header")) break;
        else if (str::startswith(s, "comment")) continue;
        else if (str::startswith(s, "element")) {
          elements.push_back(elem_info(s));
          if (elements.back().name == "") {
            throw exception(str::format() << "bad PLY header [" << s << "]");
          }
        } else if (str::startswith(s, "property")) {
          elements.back().props.push_back(prop_info(s));
          if (elements.back().props.back().name == "") {
            throw exception(str::format() << "bad PLY header [" << s << "]");
          }
        } else {
          throw exception(str::format() << "bad PLY header [" << s << "]");
        }
      }

      std::list<elem_info>::iterator i = elements.begin();
      std::list<elem_info>::iterator e = elements.end();
      std::list<elem_info>::iterator j;
      while (i != e) {
        j = matchPolyhedronElements(i, e);
        if (j != i) {
          readPolyhedron(in, binary, byteswap, *this, i, j);
          i = j;
          continue;
        }
        j = matchPolylineElements(i, e);
        if (j != i) {
          readPolyline(in, binary, byteswap, *this, i, j);
          i = j;
          continue;
        }
        j = matchPointSetElements(i, e);
        if (j != i) {
          readPointSet(in, binary, byteswap, *this, i, j);
          i = j;
          continue;
        }
        std::cerr << "warning: unhandled PLY element [" << (*i).name << "]" << std::endl;
        ++i;
      }

      return true;
    }

    bool PlyWriter::write(std::ostream &out) {
      out << "ply" << std::endl;
      if (!binary) {
        out << "format ascii 1.0" << std::endl;
      } else {
        bool be = is_big_endian();
        if (byteswap) be = !be;
        if (be) {
          out << "format binary_big_endian 1.0" << std::endl;
        } else {
          out << "format binary_little_endian 1.0" << std::endl;
        }
      }

      std::list<elem_info> elements;

      for (std::list<std::pair<std::string, stream::block_t> >::iterator i = blocks.begin(); i != blocks.end(); ++i) {
        stream::block_t &block = (*i).second;

        for (stream::named_element_list_t::iterator j = block.elems.begin(); j != block.elems.end(); ++j) {
          stream::named_element_t &elem = *j;
          if (elem.wt == NULL) continue;

          elements.push_back(elem_info(elem.name));
          elem_info &ei = elements.back();
          ei.name = elem.name;
          ei.wt = elem.wt.ptr();
          ei.count = elem.wt->length();
          out << ei.header() << std::endl;

          for (stream::named_prop_list_t::iterator k = elem.props.begin(); k != elem.props.end(); ++k) {
            stream::named_prop_t &prop = *k;
            if (prop.wt == NULL) continue;

            ei.props.push_back(prop_info(prop.name));
            prop_info &pi = elements.back().props.back();
            pi.name = prop.name;
            pi.wt = prop.wt.ptr();
            pi.type = prop.wt->dataType();
            pi.is_list = prop.wt->isList();
            if (pi.is_list) {
              pi.count_type = stream::smallest_type((uint32_t)prop.wt->maxLength());
            }
            out << pi.header() << std::endl;
          }
        }
      }

      out << "end_header" << std::endl;

      for (std::list<elem_info>::iterator b = elements.begin(); b != elements.end(); ++b) {
        (*b).wt->begin();
        for (int i = 0; i < (*b).count; ++i) {
          (*b).wt->next();
          if (binary) {
            for (size_t j = 0; j < (*b).props.size(); ++j) {
              prop_info &pi = (*b).props[j];
              pi.writeBinary(out, byteswap);
            }
          } else {
            for (size_t j = 0; j < (*b).props.size(); ++j) {
              prop_info &pi = (*b).props[j];
              if (j) out << " ";
              pi.writeAscii(out);
            }
            out << std::endl;
          }
        }
        (*b).wt->end();
      }

      return true;
    }

  }
}
