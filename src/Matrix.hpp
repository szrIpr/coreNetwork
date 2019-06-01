#pragma once
#include <iostream>
#include <algorithm>
#include <initializer_list>

class Matrix {
private:
    float* m_data = nullptr;
    int m_rows = 0;
    int m_cols = 0;

public:

    Matrix(const int row = 0, const int col = 0) : m_rows(row), m_cols(col), m_data(new float[row*col]()) {}

    Matrix(const std::initializer_list<std::initializer_list<const float>> list) : Matrix(list.size(), list.begin()->size())
    {
        std::copy_n(list.begin()->begin(), m_rows*m_cols, m_data);
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

    void operator*=(Matrix& other)
    {
        if(this->rows() != other.rows() || this->cols() != other.cols()) throw "Matrices should be the same size.";

        for(int i = 0; i < this->rows(); i++)
            for(int j = 0; j < this->cols(); j++)
                (*this)[i][j] *= other[i][j];
    }
    void operator/=(Matrix& other)
    {
        if(this->rows() != other.rows() || this->cols() != other.cols()) throw "Matrices should be the same size.";

        for(int i = 0; i < this->rows(); i++)
            for(int j = 0; j < this->cols(); j++)
                (*this)[i][j] /= other[i][j];
    }
    void operator+=(Matrix& other)
    {
        if(this->rows() != other.rows() || this->cols() != other.cols()) throw "Matrices should be the same size.";

        for(int i = 0; i < this->rows(); i++)
            for(int j = 0; j < this->cols(); j++)
                (*this)[i][j] += other[i][j];
    }
    void operator-=(Matrix& other)
    {
        if(this->rows() != other.rows() || this->cols() != other.cols()) throw "Matrices should be the same size.";

        for(int i = 0; i < this->rows(); i++)
            for(int j = 0; j < this->cols(); j++)
                (*this)[i][j] -= other[i][j];
    }

    Matrix operator*(Matrix& other)
    {
        if(this -> cols() != other.rows()) throw "The number of columns of the first matrix must equal the number of rows of the second matrix.";

        Matrix newMat(this->rows(), other.cols());

        for(int i = 0; i < newMat.rows(); i++)
            for(int j = 0; j < newMat.cols(); j++)
                for(int k = 0; k < this->cols(); k++)
                    newMat[i][j] += (*this)[i][k] * other[k][j];

        return newMat;
    }

    Matrix operator+(Matrix& other)
    {
        if(this->rows() != other.rows() || this->cols() != other.cols()) throw "Matrices should be the same size.";

        Matrix newMat(this->rows(), this->cols());

        for(int i = 0; i < newMat.rows(); i++)
            for(int j = 0; j < newMat.cols(); j++)
                newMat[i][j] += (*this)[i][j] + other[i][j];

        return newMat;
    }
    Matrix operator-(Matrix& other)
    {
        if(this->rows() != other.rows() || this->cols() != other.cols()) throw "Matrices should be the same size.";

        Matrix newMat(this->rows(), this->cols());

        for(int i = 0; i < newMat.rows(); i++)
            for(int j = 0; j < newMat.cols(); j++)
                newMat[i][j] += (*this)[i][j] - other[i][j];

        return newMat;
    }

    int rows() { return m_rows; }
    int cols() { return m_cols; }

    float* operator[](const int i) { return &m_data[i*m_cols]; }

    ~Matrix() { delete[] m_data; }
};

Matrix transpose(Matrix& mat) {
    Matrix newMat(mat.cols(), mat.rows());

    for(int i = 0; i < newMat.rows(); i++)
        for(int j = 0; j < newMat.cols(); j++)
            newMat[i][j] = mat[j][i];
            
    return newMat;
}

void print(Matrix& mat) {
    for(int i = 0; i < mat.rows(); i++) {
        std::cout << "\n";
        for(int j = 0; j < mat.cols(); j++)
            std::cout << mat[i][j] << " ";
    }
}