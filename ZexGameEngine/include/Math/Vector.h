#ifndef _MATH_VECTOR_H_
#define _MATH_VECTOR_H_

#include <array>
#include "DataDef.h"
#include "boost/operators.hpp"
#include "boost/static_assert.hpp"
#include <assert.h>
#include <cmath>
#include <functional>

namespace ZGE
{
    template < typename T, size_t row, size_t col > class Matrix;

    template< typename T, size_t N >
    class Vector :
        boost::addable< Vector< T, N > ,
        boost::addable2< Vector< T, N >, T,
        boost::subtractable< Vector< T, N >,
        boost::subtractable2< Vector< T, N >, T,
        boost::multipliable< Vector< T, N > ,
        boost::multipliable2< Vector< T, N >, T,
        boost::dividable< Vector< T, N >,
        boost::dividable2< Vector< T, N >, T,
        boost::equality_comparable< Vector< T, N > > > > > > > > > >
    {
        BOOST_STATIC_ASSERT ( N != 0 );
    public:
        typedef T value_type;

        enum { elemNum = N };

        Vector ()
        {
			
        }

        ~Vector ()
        {

        }

        Vector ( const Vector& rhs )
        {
            if ( this != &rhs )
            {
                std::copy ( rhs.m_VecArray.begin (), rhs.m_VecArray.end (), m_VecArray.begin () );
            }
        }

        Vector ( Vector&& rhs )
        {
            if ( this != &rhs )
            {
                m_VecArray = std::move ( rhs.m_VecArray );
            }
            
        }

        template < typename U, size_t M >
        Vector ( const Vector< U, M >& rhs )
        {
            if ( rhs.elemNum >= this->elemNum )
            {
                for ( size_t i = 0; i < this->elemNum; ++i )
                {
                    m_VecArray[ i ] = rhs[ i ];
                }
            }
            else
            {
                for ( size_t i = 0; i < rhs.elemNum; ++i )
                {
                    m_VecArray[ i ] = rhs[ i ];
                }
            }
        }

        T& x ()
        {
            //static_assert( elemNum >= 1 );
            return m_VecArray[ 0 ];
        }

        
        const T& x () const
        {
            //static_assert( elemNum >= 1 );
            return m_VecArray[ 0 ];
        }
        

        T& y ()
        {
            //static_assert ( elemNum >= 2 );
            return m_VecArray[ 1 ];
        }

        const T& y () const
        {
            //static_assert ( elemNum >= 2 );
            return m_VecArray[ 1 ];
        }

        T& z ()
        {
            //static_assert ( elemNum >= 3 );
            return m_VecArray[ 2 ];
        }

        const T& z () const
        {
            //static_assert ( elemNum >= 3 );
            return m_VecArray[ 2 ];
        }

        T& w ()
        {
            //static_assert ( elemNum >= 4 );
            return m_VecArray[ 3 ];
        }

        const T& w () const
        {
            //static_assert ( elemNum >= 4 );
            return m_VecArray[ 3 ];
        }

        Vector& operator = ( const Vector& rhs )
        {
            if ( this != &rhs )
            {
                std::copy ( rhs.m_VecArray.begin (), rhs.m_VecArray.end (), m_VecArray.begin () );
            }
            return *this;
        }

        template < typename U, size_t M >
        Vector& operator = ( const Vector< U, M >& rhs )
        {
            BOOST_STATIC_ASSERT ( M >= N );
            for ( size_t i = 0; i < N; ++i )
            {
                m_VecArray[ i ] = rhs[ i ];
            }
            return *this;
        }

        T& operator [] ( const size_t& index )
        {
            assert ( index < elemNum );
            return m_VecArray[ index ];
        }

        const T& operator [] ( const size_t& index ) const
        {
            assert ( index < elemNum );
            return m_VecArray[ index ];
        }

        T& operator - ()
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] = -m_VecArray[ i ];
            }
        }

        bool operator == ( const Vector& rhs )
        {
            if ( this->elemNum != rhs.elemNum )
                return false;

            for ( int i = 0; i < elemNum; ++i )
            {
                if ( m_VecArray[ i ] != rhs.m_VecArray[ i ] )
                    return false;
            }
            return true;
        }
        
        const Vector& operator += ( const Vector& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] += rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator += ( const T& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] += rhs;
            }
            return *this;
        }

        const Vector& operator -= ( const Vector& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] -= rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator -= ( const T& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] -= rhs;
            }
            return *this;
        }

        const Vector& operator *= ( const Vector& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] *= rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator *= ( const T& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] *= rhs;
            }
            return *this;
        }

        
        template < size_t MatrixCol >
        const Vector< T, MatrixCol > operator * ( const Matrix< T, N, MatrixCol >& rhs )
        {
            Vector< T, MatrixCol> retrunVector;
            for ( size_t i = 0; i < MatrixCol; ++i )
            {
                retrunVector[ i ] = Dot ( *this, rhs.ColVector ( i ) );
            }
            return retrunVector;
        }
        

        const Vector& operator /= ( const Vector& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] /= rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator /= ( const T& rhs )
        {
            for ( int i = 0; i < elemNum; ++i )
            {
                m_VecArray[ i ] /= rhs;
            }
            return *this;
        }

    protected:
        std::array< T, N > m_VecArray;
    };

    /*
    template < typename T, size_t N, size_t MatrixCol >
    static Vector< T, MatrixCol > operator * ( const Vector< T, N >& lhs, const Matrix< T, N, MatrixCol >&rhs )
    {
        Vector< T, MatrixCol> retrunVector;
        for ( size_t i = 0; i < MatrixCol; ++i )
        {
            retrunVector[ i ] = Dot ( lhs, rhs.ColVector ( i ) );
        }
        return retrunVector;
    }
    */
    
    template< typename T >
    class Vector3 : public Vector< T, 3 >
    {
    public:

        Vector3 ()
        {
            Vector3 ( T ( 0 ), T ( 0 ), T ( 0 ) );
        }

        Vector3 ( T x, T y, T z )
        {
            m_VecArray[ 0 ] = x;
            m_VecArray[ 1 ] = y;
            m_VecArray[ 2 ] = z;
        }

        Vector3 ( const Vector3& rhs )
        {
            m_VecArray[ 0 ] = rhs.x ();
            m_VecArray[ 1 ] = rhs.y ();
            m_VecArray[ 2 ] = rhs.z ();
        }

        Vector3 ( const Vector< T, 3 >& rhs )
        {
            m_VecArray[ 0 ] = rhs.x ();
            m_VecArray[ 1 ] = rhs.y ();
            m_VecArray[ 2 ] = rhs.z ();
        }
    };

    template< typename T >
    class Vector4 : public Vector< T, 4 >
    {
    public:
        Vector4 ( )
        {
            Vector4 ( T ( 0 ), T ( 0 ), T ( 0 ), T ( 0 ) );
        }

        Vector4 ( T x, T y, T z, T w )
        {
            m_VecArray[ 0 ] = x;
            m_VecArray[ 1 ] = y;
            m_VecArray[ 2 ] = z;
            m_VecArray[ 3 ] = w;
        }

        Vector4 ( const Vector4& rhs )
        {
            m_VecArray[ 0 ] = rhs.x ();
            m_VecArray[ 1 ] = rhs.y ();
            m_VecArray[ 2 ] = rhs.z ();
            m_VecArray[ 3 ] = rhs.w ();
        }

        Vector4 ( const Vector< T, 4 >& rhs )
        {
            m_VecArray[ 0 ] = rhs.x ();
            m_VecArray[ 1 ] = rhs.y ();
            m_VecArray[ 2 ] = rhs.z ();
            m_VecArray[ 3 ] = rhs.w ();
        }
    };

    template< typename T, size_t N >
    static F32 Length ( const Vector< T, N >& vec )
    {
        F32 length = 0.0f;
        for ( size_t i = 0; i < N; ++i )
        {
            length += vec[ i ] * vec[ i ];
        }
        return std::sqrtf ( length );
    }

    template< typename T, size_t N >
    static Vector< T, N > Normalize ( const Vector< T, N >& vec )
    {
        Vector< T, N > result;
        F32 length = Length ( vec );
        if ( length != 0.0f )
        {
            for ( size_t i = 0; i < N; ++i )
            {
                result[ i ] = vec[ i ] / length;
            }
        }
        return result;
    }

    template< typename T, size_t N >
    static Vector< T, N > Cross ( const Vector< T, N >& lhs, const Vector< T, N >& rhs )
    {
        return Vector3f
        (
            lhs.y () * rhs.z () - lhs.z () * rhs.y (),
            lhs.z () * rhs.x () - lhs.x () * rhs.z (),
            lhs.x () * rhs.y () - lhs.y () * rhs.x ()
        );
    }

    template< typename T, size_t N >
    static T Dot ( const Vector< T, N >& lhs, const Vector< T, N >& rhs )
    {
        T dot = T( 0 );
        for ( size_t i = 0; i < N; ++i )
        {
            dot += lhs[ i ] * rhs[ i ];
        }
        return dot;
    }

    class VectorHelper
    {

    };

    typedef Vector3< I32 > Vector3I;
    typedef Vector3< F32 > Vector3f;
    typedef Vector4< I32 > Vector4I;
    typedef Vector4< F32 > Vector4f;
}

#endif