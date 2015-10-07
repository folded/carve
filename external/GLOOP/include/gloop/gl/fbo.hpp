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

#include <vector>
#include <string>

namespace gloop {
  class FrameBuffer {
    static std::vector<GLuint> s_bound_fbo;
    
    GLuint fbo;
    unsigned width;
    unsigned height;
    
    std::pair<GLenum, Surface::Ptr> depth;
    std::pair<GLenum, Surface::Ptr> stencil;
    std::vector<std::pair<GLenum, Surface::Ptr> > colour;
    
    bool _attach(GLenum target, const Surface::Ptr &surf, GLenum attachment, int mipmap_level);
    
  public:
      FrameBuffer(unsigned _width, unsigned _height);
    
    void add(GLenum target, const Surface::Ptr &surf);
    void add(const Surface::Ptr &surf);
    void init();
    void pushBind();
    void bind();
    static void popBind();
    void attach(int mipmap_level = 0);
  };
}
