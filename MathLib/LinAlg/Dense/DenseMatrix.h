/**
 * \file
 * \author Thomas Fischer
 * \date   2011-05-24
 * \brief  Definition of the DenseMatrix class.
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <new>
#include <exception>
#include <stdexcept>
#include <iostream>

// MathLib/LinAlg
#include "../MatrixBase.h"

namespace MathLib {

/**
 * Matrix represents a dense matrix for a numeric data type.
 */
template <class T> class DenseMatrix : public MatrixBase
{
public:
   DenseMatrix (std::size_t rows, std::size_t cols);
   DenseMatrix (std::size_t rows, std::size_t cols, const T& val);
   DenseMatrix (const DenseMatrix &src);

   ~DenseMatrix ();

   /**
    * \f$ y = \alpha \cdot A x + \beta y\f$
    */
   void axpy ( T alpha, const T* x, T beta, T* y) const;

   /**
    * DenseMatrix vector multiplication
    * @param x
    * @return
    */
   T* operator* (const T *x) const;
   /**
    * DenseMatrix matrix addition.
    * @param mat
    * @return
    */
   DenseMatrix<T>* operator+ (const DenseMatrix<T>& mat) const throw (std::range_error);
   /**
    * DenseMatrix matrix subtraction
    * @param mat
    * @return
    */
   DenseMatrix<T>* operator- (const DenseMatrix<T>& mat) const throw (std::range_error);

   /**
    * DenseMatrix matrix multiplication \f$ C = A \cdot B\f$
    * @param mat the matrix \f$ B \f$
    * @return the matrix \f$ C \f$
    */
   DenseMatrix<T>* operator* (const DenseMatrix<T>& mat) const throw (std::range_error);

   /**
    * matrix transpose
    * @return the transpose of the matrix
    */
   DenseMatrix<T>* transpose() const; // HB & ZC

   DenseMatrix<T>* getSubMatrix (std::size_t b_row, std::size_t b_col, std::size_t e_row, std::size_t e_col) const throw (std::range_error);

   /**
    * overwrites values of the matrix with the given sub matrix
    * @param b_row the first row
    * @param b_col the first column
    * @param sub_mat the sub matrix
    */
   void setSubMatrix (std::size_t b_row, std::size_t b_col, const DenseMatrix<T>& sub_mat) throw (std::range_error);

   inline T & operator() (std::size_t row, std::size_t col) throw (std::range_error);
   inline T const& operator() (std::size_t row, std::size_t col) const throw (std::range_error);

   /**
    * writes the matrix entries into the output stream
    * @param out the output stream
    */
   void write (std::ostream& out) const;

private:
   // zero based addressing, but Fortran storage layout
   //inline std::size_t address(std::size_t i, std::size_t j) const { return j*rows+i; };
   // zero based addressing, C storage layout
   inline std::size_t address(std::size_t i, std::size_t j) const { return i*_n_cols+j; };

   T *_data;
};

template<class T> DenseMatrix<T>::DenseMatrix (std::size_t rows, std::size_t cols)
      : MatrixBase(rows, cols), _data (new T[_n_rows*_n_cols])
{}

template<class T> DenseMatrix<T>::DenseMatrix (std::size_t rows, std::size_t cols, T const& initial_value)
		: MatrixBase(rows, cols), _data (new T[_n_rows*_n_cols])
{
	const std::size_t n(_n_rows*_n_cols);
	for (std::size_t k(0); k<n; k++)
		_data[k] = initial_value;
}

template<class T> DenseMatrix<T>::DenseMatrix (const DenseMatrix& src) :
	MatrixBase(src.getNRows (), src.getNCols ()), _data (new T[_n_rows * _n_cols])
{
   for (std::size_t i = 0; i < _n_rows; i++)
      for (std::size_t j = 0; j < _n_cols; j++)
         _data[address(i,j)] = src (i, j);
}

template <class T> DenseMatrix<T>::~DenseMatrix ()
{
   delete [] _data;
}

template<class T> void DenseMatrix<T>::axpy ( T alpha, const T* x, T beta, T* y) const
{
   for (std::size_t i(0); i<_n_rows; i++) {
      y[i] += beta * y[i];
      for (std::size_t j(0); j<_n_cols; j++) {
         y[i] += alpha * _data[address(i,j)] * x[j];
      }
   }
}

template<class T> T* DenseMatrix<T>::operator* (const T *x) const
{
	T *y (new T[_n_rows]);
	for (std::size_t i(0); i < _n_rows; i++) {
		y[i] = 0.0;
		for (std::size_t j(0); j < _n_cols; j++) {
			y[i] += _data[address(i, j)] * x[j];
		}
	}

	return y;
}

// HS initial implementation
template<class T> DenseMatrix<T>* DenseMatrix<T>::operator+ (const DenseMatrix<T>& mat) const throw (std::range_error)
{
	// make sure the two matrices have the same dimension.
	if (_n_rows != mat.getNRows() || _n_cols != mat.getNCols())
		throw std::range_error("DenseMatrix::operator+, illegal matrix size!");

	DenseMatrix<T>* y(new DenseMatrix<T> (_n_rows, _n_cols));
	for (std::size_t i = 0; i < _n_rows; i++) {
		for (std::size_t j = 0; j < _n_cols; j++) {
			(*y)(i, j) = _data[address(i, j)] + mat(i, j);
		}
	}

	return y;
}

// HS initial implementation
template<class T> DenseMatrix<T>* DenseMatrix<T>::operator- (const DenseMatrix<T>& mat) const throw (std::range_error)
{
	// make sure the two matrices have the same dimension.
	if (_n_rows != mat.getNRows() || _n_cols != mat.getNCols())
		throw std::range_error("DenseMatrix::operator-, illegal matrix size!");

	DenseMatrix<T>* y(new DenseMatrix<T> (_n_rows, _n_cols));
	for (std::size_t i = 0; i < _n_rows; i++) {
		for (std::size_t j = 0; j < _n_cols; j++) {
			(*y)(i, j) = _data[address(i, j)] - mat(i, j);
		}
	}

	return y;
}

// HS initial implementation
template<class T> DenseMatrix<T>* DenseMatrix<T>::operator* (const DenseMatrix<T>& mat) const throw (std::range_error)
{
	// make sure the two matrices have the same dimension.
	if (_n_cols != mat.getNRows())
		throw std::range_error(
				"DenseMatrix::operator*, number of rows and cols should be the same!");

	std::size_t y_cols(mat.getNCols());
	DenseMatrix<T>* y(new DenseMatrix<T> (_n_rows, y_cols, T(0)));

	for (std::size_t i = 0; i < _n_rows; i++) {
		for (std::size_t j = 0; j < y_cols; j++) {
			for (std::size_t k = 0; k < _n_cols; k++)
				(*y)(i, j) += _data[address(i, k)] * mat(k, j);
		}
	}

	return y;
}

// HS initial implementation
template<class T> DenseMatrix<T>* DenseMatrix<T>::transpose() const
{
	DenseMatrix<T>* y(new DenseMatrix<T> (_n_cols, _n_rows));

	for (std::size_t i = 0; i < _n_rows; i++) {
		for (std::size_t j = 0; j < _n_cols; j++) {
//			y->_data[y->address(j, i)] = _data[address(i, j)];
			(*y)(j,i) = _data[address(i, j)];
		}
	}
	return y;
}

template<class T> DenseMatrix<T>* DenseMatrix<T>::getSubMatrix(
		std::size_t b_row, std::size_t b_col,
		std::size_t e_row, std::size_t e_col) const throw (std::range_error)
{
	if (b_row >= e_row | b_col >= e_col)
		throw std::range_error ("DenseMatrix::getSubMatrix() illegal sub matrix");
	if (e_row > _n_rows | e_col > _n_cols)
		throw std::range_error ("DenseMatrix::getSubMatrix() illegal sub matrix");

	DenseMatrix<T>* y(new DenseMatrix<T> (e_row-b_row, e_col-b_col));
	for (std::size_t i=b_row; i<e_row; i++) {
		for (std::size_t j=b_col; j<e_col; j++) {
			(*y)(i-b_row, j-b_col) = _data[address(i, j)];
		}
	}
	return y;
}

template<class T> void DenseMatrix<T>::setSubMatrix(
		std::size_t b_row, std::size_t b_col, const DenseMatrix<T>& sub_mat) throw (std::range_error)
{
	if (b_row + sub_mat.getNRows() > _n_rows | b_col + sub_mat.getNCols() > _n_cols)
		throw std::range_error ("DenseMatrix::setSubMatrix() sub matrix to big");

	for (std::size_t i=0; i<sub_mat.getNRows(); i++) {
		for (std::size_t j=0; j<sub_mat.getNCols(); j++) {
			_data[address(i+b_row, j+b_col)] = sub_mat(i,j);
		}
	}
}

template<class T> T& DenseMatrix<T>::operator() (std::size_t row, std::size_t col)
	throw (std::range_error)
{
   if ( (row >= _n_rows) | ( col >= _n_cols) )
	  throw std::range_error ("DenseMatrix: op() const range error");
   return _data [address(row,col)];
}


template<class T> T const& DenseMatrix<T>::operator() (std::size_t row, std::size_t col) const
	throw (std::range_error)
{
   if ( (row >= _n_rows) | ( col >= _n_cols) )
      throw std::range_error ("DenseMatrix: op() const range error");
   return _data [address(row,col)];
}

template <class T> void DenseMatrix<T>::write (std::ostream &out) const
{
	for (std::size_t i = 0; i < _n_rows; i++) {
		for (std::size_t j = 0; j < _n_cols; j++) {
			out << _data[address(i, j)] << "\t";
		}
		out << "\n";
	}
}

template <class T> T sqrFrobNrm (const DenseMatrix<T> &mat)
{
	T nrm ((T)(0));
	std::size_t i,j;
	for (j=0; j<mat.getNCols(); j++)
		for (i=0; i<mat.getNRows(); i++)
			nrm += mat(i,j) * mat(i,j);

	return nrm;
}

/** overload the output operator for class DenseMatrix */
template <class T>
std::ostream& operator<< (std::ostream &os, const DenseMatrix<T> &mat)
{
	mat.write (os);
	return os;
}

} // end namespace MathLib

#endif