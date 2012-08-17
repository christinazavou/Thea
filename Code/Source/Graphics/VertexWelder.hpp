//============================================================================
//
// This file is part of the Thea project.
//
// This software is covered by the following BSD license, except for portions
// derived from other works which are covered by their respective licenses.
// For full licensing information including reproduction of these external
// licenses, see the file LICENSE.txt provided in the documentation.
//
// Copyright (c) 2009, Stanford University
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holders nor the names of contributors
// to this software may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//============================================================================

#ifndef __Thea_Graphics_VertexWelder_hpp__
#define __Thea_Graphics_VertexWelder_hpp__

#include "../Common.hpp"
#include "../Noncopyable.hpp"
#include "../Vector3.hpp"

namespace Thea {
namespace Graphics {

// Forward declaration
class VertexWelderImpl;

/**
 * Maintains a set of vertices, with associated positions, without duplication. Two vertices are considered identical if their
 * positions are approximately the same.
 */
class THEA_API VertexWelder : private Noncopyable
{
  public:
    /** Constructor. Two vertices are considered identical if their positions are separated by at most the welding radius. */
    VertexWelder(Real weld_radius);

    /** Destructor. */
    ~VertexWelder();

    /** Add a vertex to the set, unless a coincident vertex already exists. */
    void addVertex(void * vertex, Vector3 const & position);

    /** If a vertex exists at the specified position, return it, else return null. */
    void * getVertex(Vector3 const & position) const;

  private:
    VertexWelderImpl * impl;

}; // class VertexWelder

} // namespace Graphics
} // namespace Thea

#endif
