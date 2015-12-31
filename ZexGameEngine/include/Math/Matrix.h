#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "ZGEDecl.h"
#include "Math/Vector.h"

namespace ZGE
{
    typedef Matrix44< F32 > Float44;

    template < typename T, size_t row, size_t col >
    class Matrix
        : public Object
    {
    public:
        enum { Row = row, Col = col };
        enum { elemNum = Row * Col };

        typedef T value_type;

        Matrix ()
        {
            
        }

        ~Matrix ()
        {

        }

        Matrix ( const Matrix& rhs )
        {
            if ( this != &rhs )
            {
                m_Mat = rhs.m_Mat;
            }
        }

        Matrix& operator = ( const Matrix& rhs )
        {
            if ( this != &rhs )
            {
                m_Mat = rhs.m_Mat;
            }
            return *this;
        }

        T& operator () ( size_t row, size_t col )
        {
            return m_Mat[ row ][ col ];
        }

        const T& operator () ( size_t row, size_t col ) const
        {
            return m_Mat[ row ][ col ];
        }

        T& operator [] ( size_t index )
        {
            return *( &m_Mat[ 0 ][ 0 ] + index );
        }

        const T& operator [] ( size_t index ) const
        {
            return *( &m_Mat[ 0 ][ 0 ] + index );
        }

        // Get Row Vector
        const Vector< T, Col >& RowVector ( int row ) const
        {
            assert ( row < Row );
            return m_Mat[ row ];
        }

        // Get Col Vector
        const Vector< T, Row > ColVector ( int col ) const
        {
            Vector< T, Row > colVector;
            for ( int i = 0; i < Row; ++i )
            {
                colVector[ i ] = m_Mat[ i ][ col ];
            }
            return colVector;
        }

        bool operator == ( const Matrix& rhs )
        {
            return m_Mat == rhs.m_Mat;
        }

        /*
        template< typename U, int rhsRow, int rhsCol, typename resultType = decltype( T * U ) >
        auto operator * ( const Matrix< U, rhsRow, rhsCol >& rhs ) -> Matrix< resultType, Row, rhsCol >
        {
            BOOST_STATIC_ASSERT ( this->Col == rhs.Row );
            Matrix< decltype( T * U ), Row, rhsCol > resultMatrix;
            for ( int i = 0; i < Row; ++i )
            {
                for ( int j = 0; j < rhsCol; ++j )
                {
                    auto lhsRowVector = this->RowVector ( i );
                    auto rhsColVector = rhs.ColVector ( j );
                    auto resultVector = lhsRowVector * rhsColVector;
                    T result ( 0 );
                    for ( int m = 0; m < resultVector.elemNum; ++m )
                    {
                        result += resultVector[ m ];
                    }
                    resultMatrix ( i, j ) = result;
                }
            }
            return resultMatrix;
        }
        */

        template< size_t rhsRow, size_t rhsCol >
        Matrix< T, Row, rhsCol > operator * ( const Matrix< T, rhsRow, rhsCol >& rhs )
        {
            ZGE_STATIC_ASSART ( this->Col == rhs.Row );
            Matrix< T, Row, rhsCol > resultMatrix;
            for ( int i = 0; i < Row; ++i )
            {
                for ( int j = 0; j < rhsCol; ++j )
                {
                    auto lhsRowVector = this->RowVector ( i );
                    auto rhsColVector = rhs.ColVector ( j );
                    resultMatrix ( i, j ) = Dot ( lhsRowVector, rhsColVector );
                }
            }
            return resultMatrix;
        }

        void Transpose ()
        {
            ZGE_STATIC_ASSART ( Row == Col );
            for ( auto i = 0; i < Row; ++i )
            {
                for ( auto j = i; j < Col; ++j )
                {
                    std::swap ( ( *this ) ( i, j ), ( *this ) ( j, i ) );
                }
            }
        }

    protected:
        Vector< Vector< T, Col >, Row > m_Mat;
    };

    template < typename T >
    class Matrix44 
        : public Matrix< T, 4, 4 >
    {
    public:
        Matrix44 () 
            : Matrix ()
        {

        }

        Matrix44
        (
            const T& e00, const T& e01, const T& e02, const T& e03,
            const T& e10, const T& e11, const T& e12, const T& e13,
            const T& e20, const T& e21, const T& e22, const T& e23,
            const T& e30, const T& e31, const T& e32, const T& e33
        )
        : Matrix ()
        {
            m_Mat[ 0 ][ 0 ] = e00; m_Mat[ 0 ][ 1 ] = e01; m_Mat[ 0 ][ 2 ] = e02; m_Mat[ 0 ][ 3 ] = e03;
            m_Mat[ 1 ][ 0 ] = e10; m_Mat[ 1 ][ 1 ] = e11; m_Mat[ 1 ][ 2 ] = e12; m_Mat[ 1 ][ 3 ] = e13;
            m_Mat[ 2 ][ 0 ] = e20; m_Mat[ 2 ][ 1 ] = e21; m_Mat[ 2 ][ 2 ] = e22; m_Mat[ 2 ][ 3 ] = e23;
            m_Mat[ 3 ][ 0 ] = e30; m_Mat[ 3 ][ 1 ] = e31; m_Mat[ 3 ][ 2 ] = e32; m_Mat[ 3 ][ 3 ] = e33;
        }

        Matrix44 ( const Matrix< T, 4, 4 >& rhs )
        {
            for ( size_t i = 0; i < 4 * 4; ++i )
            {
                *( &m_Mat[ 0 ][ 0 ] + i ) = rhs[ i ];
            }
        }

        void operator *= ( const Matrix44 &rhs )
        {
            for ( int i = 0; i < Row; ++i )
            {
                for ( int j = 0; j < Col; ++j )
                {
                    auto lhsRowVector = this->RowVector ( i );
                    auto rhsColVector = rhs.ColVector ( j );
                    ( *this ) ( i, j ) = Dot ( lhsRowVector, rhsColVector );
                }
            }
        }

        static const Matrix44& Zero ()
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

        static const Matrix44& Identity ()
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
    };

    template < typename T >
    bool Equal ( const T& lhs, const T& rhs )
    {
        return ( lhs == rhs );
    }

    template < typename T >
	T Determinant ( const Matrix44< T >& rhs )
    {
        T const _3142_3241 ( rhs ( 2, 0 ) * rhs ( 3, 1 ) - rhs ( 2, 1 ) * rhs ( 3, 0 ) );
        T const _3143_3341 ( rhs ( 2, 0 ) * rhs ( 3, 2 ) - rhs ( 2, 2 ) * rhs ( 3, 0 ) );
        T const _3144_3441 ( rhs ( 2, 0 ) * rhs ( 3, 3 ) - rhs ( 2, 3 ) * rhs ( 3, 0 ) );
        T const _3243_3342 ( rhs ( 2, 1 ) * rhs ( 3, 2 ) - rhs ( 2, 2 ) * rhs ( 3, 1 ) );
        T const _3244_3442 ( rhs ( 2, 1 ) * rhs ( 3, 3 ) - rhs ( 2, 3 ) * rhs ( 3, 1 ) );
        T const _3344_3443 ( rhs ( 2, 2 ) * rhs ( 3, 3 ) - rhs ( 2, 3 ) * rhs ( 3, 2 ) );

        return rhs ( 0, 0 ) * ( rhs ( 1, 1 ) * _3344_3443 - rhs ( 1, 2 ) * _3244_3442 + rhs ( 1, 3 ) * _3243_3342 )
            - rhs ( 0, 1 ) * ( rhs ( 1, 0 ) * _3344_3443 - rhs ( 1, 2 ) * _3144_3441 + rhs ( 1, 3 ) * _3143_3341 )
            + rhs ( 0, 2 ) * ( rhs ( 1, 0 ) * _3244_3442 - rhs ( 1, 1 ) * _3144_3441 + rhs ( 1, 3 ) * _3142_3241 )
            - rhs ( 0, 3 ) * ( rhs ( 1, 0 ) * _3243_3342 - rhs ( 1, 1 ) * _3143_3341 + rhs ( 1, 2 ) * _3142_3241 );
    }

    template < typename T >
	Matrix44< T > Inverse ( const Matrix44< T >& rhs )
    {
        T const _2132_2231 ( rhs ( 1, 0 ) * rhs ( 2, 1 ) - rhs ( 1, 1 ) * rhs ( 2, 0 ) );
        T const _2133_2331 ( rhs ( 1, 0 ) * rhs ( 2, 2 ) - rhs ( 1, 2 ) * rhs ( 2, 0 ) );
        T const _2134_2431 ( rhs ( 1, 0 ) * rhs ( 2, 3 ) - rhs ( 1, 3 ) * rhs ( 2, 0 ) );
        T const _2142_2241 ( rhs ( 1, 0 ) * rhs ( 3, 1 ) - rhs ( 1, 1 ) * rhs ( 3, 0 ) );
        T const _2143_2341 ( rhs ( 1, 0 ) * rhs ( 3, 2 ) - rhs ( 1, 2 ) * rhs ( 3, 0 ) );
        T const _2144_2441 ( rhs ( 1, 0 ) * rhs ( 3, 3 ) - rhs ( 1, 3 ) * rhs ( 3, 0 ) );
        T const _2233_2332 ( rhs ( 1, 1 ) * rhs ( 2, 2 ) - rhs ( 1, 2 ) * rhs ( 2, 1 ) );
        T const _2234_2432 ( rhs ( 1, 1 ) * rhs ( 2, 3 ) - rhs ( 1, 3 ) * rhs ( 2, 1 ) );
        T const _2243_2342 ( rhs ( 1, 1 ) * rhs ( 3, 2 ) - rhs ( 1, 2 ) * rhs ( 3, 1 ) );
        T const _2244_2442 ( rhs ( 1, 1 ) * rhs ( 3, 3 ) - rhs ( 1, 3 ) * rhs ( 3, 1 ) );
        T const _2334_2433 ( rhs ( 1, 2 ) * rhs ( 2, 3 ) - rhs ( 1, 3 ) * rhs ( 2, 2 ) );
        T const _2344_2443 ( rhs ( 1, 2 ) * rhs ( 3, 3 ) - rhs ( 1, 3 ) * rhs ( 3, 2 ) );
        T const _3142_3241 ( rhs ( 2, 0 ) * rhs ( 3, 1 ) - rhs ( 2, 1 ) * rhs ( 3, 0 ) );
        T const _3143_3341 ( rhs ( 2, 0 ) * rhs ( 3, 2 ) - rhs ( 2, 2 ) * rhs ( 3, 0 ) );
        T const _3144_3441 ( rhs ( 2, 0 ) * rhs ( 3, 3 ) - rhs ( 2, 3 ) * rhs ( 3, 0 ) );
        T const _3243_3342 ( rhs ( 2, 1 ) * rhs ( 3, 2 ) - rhs ( 2, 2 ) * rhs ( 3, 1 ) );
        T const _3244_3442 ( rhs ( 2, 1 ) * rhs ( 3, 3 ) - rhs ( 2, 3 ) * rhs ( 3, 1 ) );
        T const _3344_3443 ( rhs ( 2, 2 ) * rhs ( 3, 3 ) - rhs ( 2, 3 ) * rhs ( 3, 2 ) );


        T const det ( Determinant ( rhs ) );
        if ( !Equal< T > ( det, 0 ) )
        {
            T invDet ( T ( 1 ) / det );

            return Float44 
            (
                +invDet * ( rhs ( 1, 1 ) * _3344_3443 - rhs ( 1, 2 ) * _3244_3442 + rhs ( 1, 3 ) * _3243_3342 ),
                -invDet * ( rhs ( 0, 1 ) * _3344_3443 - rhs ( 0, 2 ) * _3244_3442 + rhs ( 0, 3 ) * _3243_3342 ),
                +invDet * ( rhs ( 0, 1 ) * _2344_2443 - rhs ( 0, 2 ) * _2244_2442 + rhs ( 0, 3 ) * _2243_2342 ),
                -invDet * ( rhs ( 0, 1 ) * _2334_2433 - rhs ( 0, 2 ) * _2234_2432 + rhs ( 0, 3 ) * _2233_2332 ),

                -invDet * ( rhs ( 1, 0 ) * _3344_3443 - rhs ( 1, 2 ) * _3144_3441 + rhs ( 1, 3 ) * _3143_3341 ),
                +invDet * ( rhs ( 0, 0 ) * _3344_3443 - rhs ( 0, 2 ) * _3144_3441 + rhs ( 0, 3 ) * _3143_3341 ),
                -invDet * ( rhs ( 0, 0 ) * _2344_2443 - rhs ( 0, 2 ) * _2144_2441 + rhs ( 0, 3 ) * _2143_2341 ),
                +invDet * ( rhs ( 0, 0 ) * _2334_2433 - rhs ( 0, 2 ) * _2134_2431 + rhs ( 0, 3 ) * _2133_2331 ),

                +invDet * ( rhs ( 1, 0 ) * _3244_3442 - rhs ( 1, 1 ) * _3144_3441 + rhs ( 1, 3 ) * _3142_3241 ),
                -invDet * ( rhs ( 0, 0 ) * _3244_3442 - rhs ( 0, 1 ) * _3144_3441 + rhs ( 0, 3 ) * _3142_3241 ),
                +invDet * ( rhs ( 0, 0 ) * _2244_2442 - rhs ( 0, 1 ) * _2144_2441 + rhs ( 0, 3 ) * _2142_2241 ),
                -invDet * ( rhs ( 0, 0 ) * _2234_2432 - rhs ( 0, 1 ) * _2134_2431 + rhs ( 0, 3 ) * _2132_2231 ),

                -invDet * ( rhs ( 1, 0 ) * _3243_3342 - rhs ( 1, 1 ) * _3143_3341 + rhs ( 1, 2 ) * _3142_3241 ),
                +invDet * ( rhs ( 0, 0 ) * _3243_3342 - rhs ( 0, 1 ) * _3143_3341 + rhs ( 0, 2 ) * _3142_3241 ),
                -invDet * ( rhs ( 0, 0 ) * _2243_2342 - rhs ( 0, 1 ) * _2143_2341 + rhs ( 0, 2 ) * _2142_2241 ),
                +invDet * ( rhs ( 0, 0 ) * _2233_2332 - rhs ( 0, 1 ) * _2133_2331 + rhs ( 0, 2 ) * _2132_2231 ) 
             );
        }
        else
        {
            return rhs;
        }
    }

	template< typename T, size_t Row, size_t Col >
	Matrix< T, Col, Row >& Transpose ( const Matrix< T, Row, Col >& rhs )
	{
		Matrix< T, Col, Row > ret;
		for ( size_t i = 0; i < ret.Row; ++i )
		{
			for ( size_t j = 0; j < ret.Col; ++j )
			{
				ret ( i, j ) = rhs ( j, i );
			}
		}
		return ret;
	}

	template < typename T >
	Matrix44< T > OrthoLH ( const T& width, const T& height, const T& zNearPlane, const T& zFarPlane )
	{
		const T w_2 ( width / 2 );
		const T h_2 ( height / 2 );
		return OrthoOffCenterLH ( -w_2, w_2, -h_2, h_2, zNearPlane, zFarPlane );
	}

	template < typename T >
	Matrix44< T > OrthoOffCenterLH ( const T& left, const T& right, const T& bottom, const T& top, const T& zNearPlane, const T& zFarPlane )
	{
		const T q ( T ( 1 ) / ( zFarPlane - zNearPlane ) );
		const T invWidth ( T ( 1 ) / ( right - left ) );
		const T invHeight ( T ( 1 ) / ( top - bottom ) );

		return Matrix44< T > 
			(
				invWidth + invWidth,			0,								0,					0,
				0,								invHeight + invHeight,			0,					0,
				0,								0,								q,					0,
				-( left + right ) * invWidth,	-( top + bottom ) * invHeight,	-zNearPlane * q,	1
			);
	}

	template < typename T >
	Matrix44< T > PerspectiveFovLH ( const T& fov, const T& aspect, const T& zNearPlane, const T& zFarPlane )
	{
		const T h ( T ( 1 ) / tan ( fov / T ( 2 ) ) );
		const T w ( h / aspect );
		const T q ( zFarPlane / ( zFarPlane - zNearPlane ) );
		return Matrix44< T >
			(
				w,	0,	0,					0,
				0,	h,	0,					0,
				0,	0,	q,					1,
				0,	0,	-zNearPlane * q,	0
			);
	}

} 


#endif