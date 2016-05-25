#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "Math/Vector.h"

namespace ZGE
{
    template< typename T >
    class Matrix44
    {
    public:
        typedef T value_type;

        static constexpr unsigned int ROW = 4;
        static constexpr unsigned int COL = 4;
        static constexpr unsigned int ElemNum = ROW * COL;

        Matrix44 ()
        {
            Matrix44 (Identity ());
        }

        Matrix44
        (
            const T &e00, const T &e01, const T &e02, const T &e03,
            const T &e10, const T &e11, const T &e12, const T &e13,
            const T &e20, const T &e21, const T &e22, const T &e23,
            const T &e30, const T &e31, const T &e32, const T &e33
        )
        {
            m_Mat [0] [0] = e00; m_Mat [0] [1] = e01; m_Mat [0] [2] = e02; m_Mat [0] [3] = e03;
            m_Mat [1] [0] = e10; m_Mat [1] [1] = e11; m_Mat [1] [2] = e12; m_Mat [1] [3] = e13;
            m_Mat [2] [0] = e20; m_Mat [2] [1] = e21; m_Mat [2] [2] = e22; m_Mat [2] [3] = e23;
            m_Mat [3] [0] = e30; m_Mat [3] [1] = e31; m_Mat [3] [2] = e32; m_Mat [3] [3] = e33;
        }

        template< typename U >
        Matrix44 (const Matrix44< U > &rhs)
        {
            if (this != &rhs)
            {
                m_Mat = rhs.m_Mat;
            }
        }

        template< typename U >
        Matrix44 & operator = (const Matrix44< U > &rhs)
        {
            if (this != &rhs)
            {
                m_Mat = rhs.m_Mat;
            }
            return *this;
        }

        T & operator () (size_t row, size_t col)
        {
            return m_Mat [row] [col];
        }

        const T & operator () (size_t row, size_t col) const
        {
            return m_Mat [row] [col];
        }

        Matrix44ColProxy< T > operator [] (size_t index)
        {
            return Matrix44ColProxy<T> (&(m_Mat [index]));
        }

        const Matrix44ColProxy< T > operator [] (size_t index) const
        {
            return Matrix44ColProxy< T > (&(m_Mat [index]));
        }

        bool operator == (const Matrix44& rhs)
        {
            return m_Mat == rhs.m_Mat;
        }

        // Get Row Vector
        const Vector< T, COL > & RowVector (const int &row) const
        {
            assert (row < ROW);
            return m_Mat [row];
        }

        // Get Col Vector
        const Vector< T, ROW > ColVector (const int &col) const
        {
            Vector< T, ROW > colVector;
            for (int i = 0; i < ROW; ++i)
            {
                colVector [i] = m_Mat [i] [col];
            }
            return colVector;
        }

        void Transpose ()
        {
            std::swap ((*this) [0] [1], (*this) [1] [0]);
            std::swap ((*this) [0] [2], (*this) [2] [0]);
            std::swap ((*this) [0] [3], (*this) [3] [0]);
            std::swap ((*this) [2] [1], (*this) [1] [2]);
            std::swap ((*this) [3] [1], (*this) [1] [3]);
            std::swap ((*this) [3] [2], (*this) [2] [3]);
        }

        static const Matrix44 & Zero ()
        {
            static const Matrix44 m
            (
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
            );
            return m;
        }

        static const Matrix44 & Identity ()
        {
            static const Matrix44 m
            (
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
            return m;
        }
    private:
        Vector< Vector< T, COL >, ROW > m_Mat;
    };

    template<typename T>
    class Matrix44ColProxy
    {
        friend class Matrix44< T >;
        friend class Vector< T, 4 >;
        typedef T value_type;
    public:
        ~Matrix44ColProxy ()
        {

        }

        Matrix44ColProxy (const Matrix44ColProxy &) = delete;
        void operator = (const Matrix44ColProxy &)  = delete;

        value_type & operator [] (size_t index)
        {
            return const_cast<value_type &>((*m_ColVec) [index]);
        }

        const value_type & operator [] (size_t index) const
        {
            return (*m_ColVec) [index];
        }

        operator const Vector<T, 4> & ()
        {
            return *m_ColVec;
        }

    private:
        Matrix44ColProxy (const Vector< T, 4 > *colVec)
            : m_ColVec (colVec)
        {

        }

        Matrix44ColProxy (Matrix44ColProxy &&rhs)
        {
            m_ColVec = rhs.m_ColVec;
        }

        void operator = (Matrix44ColProxy &&rhs)
        {
            m_ColVec = rhs.m_ColVec;
        }

        const Vector< T, 4 > *m_ColVec;

    };

    template< typename TVec, typename TMat >
    static Vector< TVec, 4 > operator * (const Vector<TVec, 4> &lhs, const Matrix44<TMat> &rhs)
    {
        Vector< TVec, 4 > returnVec;
        returnVec [0] = MathFunc::Dot (lhs, rhs.ColVector (0));
        returnVec [1] = MathFunc::Dot (lhs, rhs.ColVector (1));
        returnVec [2] = MathFunc::Dot (lhs, rhs.ColVector (2));
        returnVec [3] = MathFunc::Dot (lhs, rhs.ColVector (3));
        return returnVec;
    }

    template< typename T >
    static Matrix44< T > operator * (const Matrix44< T > &lhs, const Matrix44< T > &rhs)
    {
        Matrix44< T > resultMatrix;
        for (int i = 0; i < Matrix44< T >::ROW; ++i)
        {
            for (int j = 0; j < Matrix44< T >::COL; ++j)
            {
                auto lhsRowVector = lhs.RowVector (i);
                auto rhsColVector = rhs.ColVector (j);
                resultMatrix (i, j) = MathFunc::Dot (lhsRowVector, rhsColVector);
            }
        }
        return resultMatrix;
    }
}