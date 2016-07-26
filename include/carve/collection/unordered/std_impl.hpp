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


#pragma once

#include <unordered_map>
#include <unordered_set>

#undef UNORDERED_COLLECTIONS_SUPPORT_RESIZE

namespace std {

  template <typename A, typename B>
  struct hash<std::pair<A, B> > : public std::unary_function<std::pair<A, B>, size_t> {
    size_t operator()(const std::pair<A, B> &v) const {
      std::size_t seed = 0;

      seed ^= hash<A>()(v.first);
      seed ^= hash<B>()(v.second) + (seed<<6) + (seed>>2);

      return seed;
    }
  };

}
