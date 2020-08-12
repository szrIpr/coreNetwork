#pragma once
#include <iostream>
#include <algorithm>
#include <initializer_list>

class Matrix {
private:
    int m_rows = 0;
    int m_cols = 0;
    float* m_data = nullptr;

public:

    Matrix(const int row, const int col) : m_rows(row), m_cols(col), m_data(new float[row*col]()) {}

    Matrix(const std::initializer_list<std::initializer_list<float>> list) : Matrix(list.size(), list.begin()->size())
    {
	for(int i = 0; i < m_rows; i++)
	    std::copy((list.begin()+i)->begin(), (list.begin()+i)->end(), m_data + (i * m_cols));
    }

    Matrix(const int row, const int col, const float val) : Matrix(row, col)
    {
        std::fill_n(m_data, m_rows*m_cols, val);
    }

    Matrix(const Matrix& other) : Matrix(other.m_rows, other.m_cols)
    {
        std::copy_n(other.m_data, m_rows*m_cols, m_data);
    }

    Matrix(Matrix&& other) : m_rows(other.m_rows), m_cols(other.m_cols), m_data(other.m_data)
    {
        other.m_data = nullptr;
        other.m_rows = 0;
        other.m_cols = 0;
    }

    Matrix& operator=(const Matrix& other)
    {
        if(this != &other)
        {
            if(m_rows*m_cols != other.m_rows*other.m_cols)
            {
                delete[] m_data;
                m_rows = other.m_rows;
                m_cols = other.m_cols;
                m_data = new float[m_rows*m_cols]();
            }
            else if(m_rows != other.m_rows || m_cols != other.m_cols)
            {
                m_rows = other.m_rows;
                m_cols = other.m_cols;
            }
            
            std::copy_n(other.m_data, m_rows*m_cols, m_data);
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other)
    {
        if(this != &other)
        {
            delete[] m_data;

            m_data = other.m_data;
            m_rows = other.m_rows;
            m_cols = other.m_cols;

            other.m_data = nullptr;
            other.m_rows = 0;
            other.m_cols = 0;
        }
        return *this;
    }

    template<typename func>
    void map(func f)
    {
        for(int i = 0; i < m_rows; i++)
            for(int j = 0; j < m_cols; j++)
                (*this)[i][j] = f((*this)[i][j]);
    }

    void operator*=(const Matrix& other)
    {
        if(rows() != other.rows() || cols() != other.cols()) throw "Matrices should be the same size.";

        for(int i = 0; i < rows(); i++)
            for(int j = 0; j < cols(); j++)
                (*this)[i][j] *= other[i][j];
    }

    void operator*=(float val)
    {
        for(int i = 0; i < rows(); i++)
            for(int j = 0; j < cols(); j++)
                (*this)[i][j] *= val;
    }

    void operator+=(const Matrix& other)
    {
        if(rows() != other.rows() || cols() != other.cols()) throw "Matrices should be the same size.";

        for(int i = 0; i < rows(); i++)
            for(int j = 0; j < cols(); j++)
                (*this)[i][j] += other[i][j];
    }

    void operator-=(const Matrix& other)
    {
        if(rows() != other.rows() || cols() != other.cols()) throw "Matrices should be the same size.";

        for(int i = 0; i < rows(); i++)
            for(int j = 0; j < cols(); j++)
                (*this)[i][j] -= other[i][j];
    }

    Matrix operator*(const Matrix& other) const
    {
        Matrix newMat(*this);

        newMat *= other;

        return newMat;
    }

    Matrix operator*(float val) const
    {
        Matrix newMat(*this);

        newMat *= val;

        return newMat;
    }

    Matrix operator+(const Matrix& other) const
    {
        Matrix newMat(*this);

        newMat += other;

        return newMat;
    }

    Matrix operator-(const Matrix& other) const
    {
        Matrix newMat(*this);

        newMat -= other;

        return newMat;
    }

    int rows() const { return m_rows; }
    int cols() const { return m_cols; }

    float* operator[](int i) { return &m_data[i*m_cols]; }
    const float* operator[](int i) const { return &m_data[i*m_cols]; }

    ~Matrix() { delete[] m_data; }
};

Matrix dot(const Matrix& a, const Matrix& b) {
    if(a.cols() != b.rows()) throw "The number of columns of the first matrix must equal the number of rows of the second matrix.";

    Matrix newMat(a.rows(), b.cols());

    for(int i = 0; i < newMat.rows(); i++)
        for(int j = 0; j < newMat.cols(); j++)
            for(int k = 0; k < a.cols(); k++)
                newMat[i][j] += a[i][k] * b[k][j];

    return newMat;
}

Matrix transpose(Matrix& mat) {
    Matrix newMat(mat.cols(), mat.rows());

    for(int i = 0; i < newMat.rows(); i++)
        for(int j = 0; j < newMat.cols(); j++)
            newMat[i][j] = mat[j][i];
            
    return newMat;
}

template<typename func>
Matrix map(const Matrix& mat, func f)
{
    Matrix newMat(mat);

    for(int i = 0; i < newMat.rows(); i++)
        for(int j = 0; j < newMat.cols(); j++)
            newMat[i][j] = f(mat[i][j]);

    return newMat;
}

void print(const Matrix& mat) {
    for(int i = 0; i < mat.rows(); i++) {
        for(int j = 0; j < mat.cols(); j++)
            std::cout << mat[i][j] << " ";
        std::cout << "\n";
    }
}
