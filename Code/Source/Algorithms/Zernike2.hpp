//============================================================================
//
// This file is part of the Thea project.
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

#ifndef __Thea_Algorithms_Zernike2_hpp__
#define __Thea_Algorithms_Zernike2_hpp__

// Currently Visual Studio 2010 fails to compile Boost multi_array in Debug mode: https://svn.boost.org/trac/boost/ticket/4874
#if defined(_MSC_VER) && _MSC_VER >= 1600
#  define THEA_NO_ZERNIKE
#endif

#ifndef THEA_NO_ZERNIKE

#include "../Common.hpp"
#include "../AbstractAddressableMatrix.hpp"
#include "../MatVec.hpp"
#include <boost/multi_array.hpp>
#include <complex>

namespace Thea {
namespace Algorithms {

/**
 * Compute Zernike moments of a 2D distribution, represented as a matrix of density values. The density values may be
 * multidimensional, i.e. the matrix elements may be vectors or colors.
 *
 * This class adapts code for the LightField Descriptor from Ding-Yun Chen et al.,
 * http://3d.csie.ntu.edu.tw/~dynamic/3DRetrieval/index.html .
 */
class THEA_API Zernike2
{
  public:
    /** The matrix type storing N-dimensional moments. Each column is a moment. */
    template <int N, typename ScalarT> using MomentMatrix = Matrix< N, Eigen::Dynamic, std::complex<ScalarT> >;

    /** %Options for generating Zernike moments. */
    struct THEA_API Options
    {
      int angular_steps;  ///< Number of angular steps.
      int radial_steps;   ///< Number of radial steps.
      int lut_radius;     ///< Radius of Zernike basis function for lookup table.

      /** Constructor. */
      Options(int angular_steps_ = 12, int radial_steps_ = 3, int lut_radius_ = 50)
      : angular_steps(angular_steps_), radial_steps(radial_steps_), lut_radius(lut_radius_)
      {}

      /** Get the set of default options. */
      static Options const & defaults() { static Options const def; return def; }

    }; // struct Options

    /** Constructor. */
    Zernike2(Options const & opts_ = Options::defaults());

    /** Get the number of moments generated by a call to compute(). */
    intx numMoments() const { return opts.angular_steps * opts.radial_steps; }

    /**
     * Compute Zernike moments of a 2D distribution, represented as a matrix of single- or multi-dimensional density values
     * (such as reals, vectors or colors). If the density values have more than one dimension, they should allow array
     * addressing (<code>operator[](intx i)</code>). The template parameter N, inferred from the last argument, must be the same
     * as the number of dimensions of the input.
     *
     * @param distrib The distribution represented as an addressable matrix of density values.
     * @param center_x The x-coordinate (column) of the center of the non-zero region of the distribution, in matrix
     *   coordinates.
     * @param center_y The y-coordinate (row) of the center of the non-zero region of the distribution, in matrix coordinates.
     * @param radius The radius of the non-zero region of the distribution, measured from the center, in matrix coordinates. All
     *   zero elements can be ignored when specifying this number.
     * @param moments Used to return the Zernike moments, specified in "angle-major, radius-minor" order. Each moment is a
     *   column of the matrix.
     *
     * @return The number of pixels that have non-zero values and were used to compute the moments.
     */
    template <int N, typename T, typename ScalarT>
    intx compute(AbstractAddressableMatrix<T> const & distrib, double center_x, double center_y, double radius,
                 MomentMatrix<N, ScalarT> & moments) const;

  private:
    /** Generate lookup table for moments. */
    void generateBasisLUT() const;

    /**
     * Check if an input element (type T) is zero. This is the generic implementation for multi-channel inputs. The
     * single-channel case is separately specialized.
    */
    template <int N> struct IsZero
    {
      template <typename T> static bool check(T const & t)
      {
        for (int i = 0; i < N; ++i)
          if (t[i] != 0) return false;

        return true;
      }
    };

    /**
     * Add a scaled increment to a moment. This is the generic implementation for multi-channel inputs. The single-channel case
     * is separately specialized.
     */
    template <int N, typename ScalarT> struct Accum
    {
      template <typename T>
      static void add(T const & t, std::complex<double> const & x, typename MomentMatrix<N, ScalarT>::ColXpr acc)
      {
        for (intx i = 0; i < acc.size(); ++i)
        {
          acc[i].real(acc[i].real() + static_cast<ScalarT>(x.real() * t[i]));
          acc[i].imag(acc[i].imag() - static_cast<ScalarT>(x.imag() * t[i]));
        }
      }
    };

    typedef boost::multi_array< std::complex<double>, 4 > LUT;  ///< Lookup table class.

    Options opts;                ///< Set of options.
    mutable LUT lut;             ///< Coefficient lookup table.
    mutable bool lut_generated;  ///< Has the LUT been generated?

}; // class Zernike2

// Specializations for single-channel input.
template <>
struct Zernike2::IsZero<1>
{
  template <typename T> static bool check(T const & t)
  {
    return t != 0;
  }
};

template <typename ScalarT>
struct Zernike2::Accum<1, ScalarT>
{
  template <typename T>
  static void add(T const & t, std::complex<double> const & x, typename MomentMatrix<1, ScalarT>::ColXpr & acc)
  {
    acc[0].real(acc[0].real() + static_cast<ScalarT>(x.real() * t));
    acc[0].imag(acc[0].imag() - static_cast<ScalarT>(x.imag() * t));
  }
};

template <int N, typename T, typename ScalarT>
intx
Zernike2::compute(AbstractAddressableMatrix<T> const & distrib, double center_x, double center_y, double radius,
                  Zernike2::MomentMatrix<N, ScalarT> & moments) const
{
  alwaysAssertM(radius > 0, "Zernike2: Radius must be greater than zero");

  this->generateBasisLUT();

  moments.resize(Eigen::NoChange, numMoments());
  moments.setZero();

  intx ncols = distrib.cols();
  intx nrows = distrib.rows();

  // Don't go outside the specified radius
  intx min_x = std::max(0L,        (intx)std::ceil (center_x - radius));
  intx max_x = std::min(ncols - 1, (intx)std::floor(center_x + radius));

  intx min_y = std::max(0L,        (intx)std::ceil (center_y - radius));
  intx max_y = std::min(nrows - 1, (intx)std::floor(center_y + radius));

  double r_radius = opts.lut_radius / radius;

  std::complex<double> x1, x2, x3;
  double dx, dy, tx, ty;
  intx ix, iy;
  intx count = 0;
  for (intx y = min_y; y <= max_y; ++y)
  {
    for (intx x = min_x; x <= max_x; ++x)
    {
      T const & density = distrib.at(y, x);
      if (!IsZero<N>::check(density))
      {
        dx = x - center_x;
        dy = y - center_y;
        tx = dx * r_radius + opts.lut_radius;
        ty = dy * r_radius + opts.lut_radius;
        ix = (intx)tx;
        iy = (intx)ty;
        dx = tx - ix;
        dy = ty - iy;

        // Summation of basis function
        for (intx p = 0; p < opts.angular_steps; ++p)
        {
          for (intx r = 0; r < opts.radial_steps; ++r)
          {
            x1 = lut[p][r][ix][iy    ] + (lut[p][r][ix + 1][iy    ] - lut[p][r][ix][iy    ]) * dx;
            x2 = lut[p][r][ix][iy + 1] + (lut[p][r][ix + 1][iy + 1] - lut[p][r][ix][iy + 1]) * dx;
            x3 = x1 + (x2 - x1) * dy;

            Accum<N, ScalarT>::add(density, x3, moments.col(p * opts.radial_steps + r));
          }
        }

        count++;
      }
    }
  }

  if (count > 0)
    moments /= (ScalarT)count;

  return count;
}

} // namespace Algorithms
} // namespace Thea

#endif // !defined(THEA_NO_ZERNIKE)

#endif
