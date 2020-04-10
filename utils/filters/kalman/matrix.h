/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef MATRIX_H
#define MATRIX_H


#include <iostream>

#include <assert.h>

class Matrix {

private:

    unsigned _lines;

    unsigned _columns;

    float **_matrix;

    void allocate();

    void deallocate();

    void initialize();

public:

    Matrix(unsigned lines=0, unsigned columns=0);

    Matrix(const Matrix &M);

    ~Matrix();

    // Basics

    void setSize(unsigned lines, unsigned columns);

    unsigned lines() const { return _lines; }

    unsigned columns() const { return _columns; }

    float get(unsigned i, unsigned j) const;

    void set(unsigned i, unsigned j, float value);

    // Matrix functions

    Matrix transposed() const;

    // Matrix generation

    static Matrix identity(unsigned size);

    static Matrix diag(unsigned size, float diagValue);

    // Auxiliary functions

    void copyFrom(const Matrix &M);

    void print();

    // Operators

    void operator=(const Matrix &M);

    Matrix operator+(const Matrix &M) const;

    Matrix operator+(float k) const;

    Matrix operator-(const Matrix &M) const;

    Matrix operator-(float k) const;

    Matrix operator*(const Matrix &M) const;

    Matrix operator*(float k) const;

    void operator+=(const Matrix &M);

    void operator+=(float k);

    void operator-=(const Matrix &M);

    void operator-=(float k);

    void operator*=(float k);

};
#endif // MATRIX_H
