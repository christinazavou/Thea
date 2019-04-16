//============================================================================
//
// This file is part of the Browse3D project.
//
// This software is covered by the following BSD license, except for portions
// derived from other works which are covered by their respective licenses.
// For full licensing information including reproduction of these external
// licenses, see the file LICENSE.txt provided in the documentation.
//
// Copyright (C) 2011, Siddhartha Chaudhuri/Stanford University
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

#ifndef __Browse3D_MeshFwd_hpp__
#define __Browse3D_MeshFwd_hpp__

#include "Common.hpp"

namespace Thea {
namespace Graphics {

// Forward declarations
struct NullAttribute;

template <typename VA, typename EA, typename FA, template <typename T> class Alloc> class GeneralMeshVertex;
template <typename VA, typename EA, typename FA, template <typename T> class Alloc> class GeneralMeshFace;
template <typename MeshType> class MeshGroup;

} // namespace Graphics
} // namespace Thea

namespace Browse3D {

// Forward declarations
class VertexAttribute;
class FaceAttribute;
class Mesh;

/**< Handle to a mesh vertex. */
typedef Graphics::GeneralMeshVertex<VertexAttribute, Graphics::NullAttribute, FaceAttribute, std::allocator> MeshVertex;

/**< Handle to a mesh face. */
typedef Graphics::GeneralMeshFace<VertexAttribute, Graphics::NullAttribute, FaceAttribute, std::allocator> MeshFace;

typedef Graphics::MeshGroup<Mesh> MeshGroup;            ///< A hierarchical group of meshes.
typedef std::shared_ptr<Mesh> MeshPtr;                       ///< A shared pointer to a mesh.
typedef std::shared_ptr<Mesh const> MeshConstPtr;            ///< A shared pointer to an immutable mesh.
typedef std::shared_ptr<MeshGroup> MeshGroupPtr;             ///< A shared pointer to a group of meshes.
typedef std::shared_ptr<MeshGroup const> MeshGroupConstPtr;  ///< A shared pointer to an immutable group of meshes.

} // namespace Browse3D

#endif
