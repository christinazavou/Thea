//============================================================================
//
// This file is part of the Browse3D project.
//
// This software is covered by the following BSD license, except for portions
// derived from other works which are covered by their respective licenses.
// For full licensing information including reproduction of these external
// licenses, see the file LICENSE.txt provided in the documentation.
//
// Copyright (c) 2011, Stanford University
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

#ifndef __Browse3D_Util_hpp__
#define __Browse3D_Util_hpp__

#include "Common.hpp"
#include "../../Ray3.hpp"

class QPoint;
class QPointF;

namespace Thea {

class Image;

namespace Graphics {

class RenderSystem;
class Camera;

} // namespace Graphics

} // namespace Thea

namespace Browse3D {

// Draw a sphere.
void drawSphere(Graphics::RenderSystem & render_system, Vector3 const & center, Real radius, int num_steps = 16);

// Draw a capsule (cylinder with edges capped by hemispheres).
void drawCapsule(Graphics::RenderSystem & render_system, Vector3 const & base_center, Vector3 const & top_center, Real radius,
                 int num_steps = 16);

// Draw a torus with the given center and primary axes given by the unit vectors u, v.
void drawTorus(Graphics::RenderSystem & render_system, Vector3 const & center, Vector3 const & u, Vector3 const & v,
               Real radius, Real width, int num_major_steps = 16, int num_minor_steps = 8, bool alternate_dark_light = false,
               Color4 const & color1 = Color4(1, 0, 0, 1), Color4 const & color2 = Color4(0, 1, 0, 1));

// Get the number of colors in the standard palette.
int numPaletteColors();

// Get the i-th color in the standard palette.
Color3 const & getPaletteColor(int i);

// Map a label to a color.
Color3 getLabelColor(QString const & label);

// Compute a picking ray, given a screen point and a camera.
Ray3 computePickRay(QPointF const & p, Graphics::Camera const & camera, int width, int height);

// Map mouse drags to transforms.
Vector3 dragToTranslation(QPoint const & start, QPoint const & end, int width, int height, Graphics::Camera const & camera,
                          Real object_distance);
Matrix3 dragToRotation(QPoint const & start, QPoint const & end, int width, int height, Graphics::Camera const & camera);
Matrix3 dragToRotationAroundAxis(QPoint const & start, Vector3 const & start_pick, QPoint const & end, Vector3 const & axis,
                                 Vector3 const & center, int width, int height, Graphics::Camera const & camera);
Matrix3 dragToJoystickRotation(QPoint const & start, QPoint const & end, Vector3 const & center, Real offset, int width,
                               int height, Graphics::Camera const & camera);
Real dragToScale(QPoint const & start, QPoint const & end, int width, int height, Graphics::Camera const & camera);

// Load an image from a file, with a subsequent call to fixChannelOrdering().
bool loadImage(Image & image, QString const & filename);

// Make sure image channels are in order RGB[A]. Currently works only for RGB_8U and RGBA_8U.
void fixChannelOrdering(Image & image);

// A wrapper for an iterator that, when dereferenced, returns a mutable pointer to the current object instead of a reference to
// it.
template <typename RefIterator>
struct RefToPtrIterator : public RefIterator
{
  RefToPtrIterator(RefIterator i) : RefIterator(i) {}

  typename RefIterator::value_type * operator*() const { return &(this->RefIterator::operator*()); }

}; // struct RefToPtrIterator

// Specialization when the iterator is a pointer to T.
template <typename T>
class RefToPtrIterator<T *> : public std::iterator<std::random_access_iterator_tag, T *, std::ptrdiff_t, T **, T *>
{
  public:
    explicit RefToPtrIterator(T * ii_ = NULL) : ii(ii_) {}
    RefToPtrIterator(RefToPtrIterator const & src) : ii(src.ii) {}

    T * operator*() const { return ii; }
    template <typename IntegerType> T * operator[](IntegerType n) { return &ii[n]; }

    RefToPtrIterator & operator++() { ++ii; return *this; }
    RefToPtrIterator & operator++(int) { RefToPtrIterator tmp(*this); operator++(); return *this; }
    RefToPtrIterator & operator--() { --ii; return *this; }
    RefToPtrIterator & operator--(int) { RefToPtrIterator tmp(*this); operator--(); return *this; }

    RefToPtrIterator & operator=(RefToPtrIterator const & src) { ii = src.ii; return *this; }
    template <typename IntegerType> RefToPtrIterator & operator+=(IntegerType n) { ii += n; return *this; }
    template <typename IntegerType> RefToPtrIterator & operator-=(IntegerType n) { ii -= n; return *this; }

    template <typename IntegerType> RefToPtrIterator operator+(IntegerType n) const { return RefToPtrIterator(ii + n); }
    template <typename IntegerType> RefToPtrIterator operator-(IntegerType n) const { return RefToPtrIterator(ii - n); }

    std::ptrdiff_t operator-(RefToPtrIterator const & rhs) const { return ii - rhs.ii; }

    bool operator==(RefToPtrIterator const & rhs) const { return ii == rhs.ii; }
    bool operator!=(RefToPtrIterator const & rhs) const { return ii != rhs.ii; }

    bool operator< (RefToPtrIterator const & rhs) const { return ii <  rhs.ii; }
    bool operator> (RefToPtrIterator const & rhs) const { return ii >  rhs.ii; }
    bool operator<=(RefToPtrIterator const & rhs) const { return ii <= rhs.ii; }
    bool operator>=(RefToPtrIterator const & rhs) const { return ii >= rhs.ii; }

  private:
    T * ii;

}; // class RefToPtrIterator<T, T*>

} // namespace Browse3D

#endif
