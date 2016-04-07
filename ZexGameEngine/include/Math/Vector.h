#ifndef _MATH_VECTOR_H_
#define _MATH_VECTOR_H_

#include "ZGEDecl.h"

#include "External/boost/operators.hpp"

#include "Math/MathFunc.h"

namespace ZGE
{
    typedef Vector< I32, 2 > Vector2i;
    typedef Vector< F32, 2 > Vector2f;
    typedef Vector< U32, 2 > Vector2u;
    typedef Vector< I32, 3 > Vector3i;
    typedef Vector< F32, 3 > Vector3f;
    typedef Vector< U32, 3 > Vector3u;
    typedef Vector< I32, 4 > Vector4i;
    typedef Vector< F32, 4 > Vector4f;
    typedef Vector< U32, 4 > Vector4u;

    template < typename T, U32 N >
    class Vector
        :
        boost::addable< Vector< T, N >,
        boost::addable2< Vector< T, N >, T,
        boost::subtractable< Vector< T, N >,
        boost::subtractable2< Vector< T, N >, T,
        boost::multipliable< Vector< T, N >,
        boost::multipliable2< Vector< T, N >, T,
        boost::dividable< Vector< T, N >,
        boost::dividable2< Vector< T, N >, T,
        boost::equality_comparable< Vector< T, N > > > > > > > > > >
        , public Object
    {
        ZGE_STATIC_ASSART ( N > 0 );
        friend class VectorHelper;
    public:
        typedef T                                       value_type;
        typedef std::array< T, N >                      VecArrayType;
        typedef typename VecArrayType::iterator         iterator;
        typedef typename VecArrayType::const_iterator   const_iterator;
        //typedef N                                       size;
        static constexpr U32 size = N;

        enum { ElemNum = N };
        enum { ElemSize = sizeof ( T ) };

        Vector ()
        {
            m_VecArray.fill ( T ( 0 ) );
        }

        template < typename xArg >
        explicit Vector ( const xArg &x )
            : Vector ()
        {
            m_VecArray[ 0 ] = x;
        }

        template < typename Arg, typename... Args >
        explicit Vector ( const Arg &x, const Args&... args )
            : Vector ()
        {
            m_VecArray[ 0 ] = x;
            VectorHelper::RecursiveAssignValue ( *this, 1, args... );
        }

        template < typename T1, size_t N, typename... Args >
        explicit Vector ( const Vector< T1, N > &that, const Args&... args )
        {
            auto minAssignElemNum = std::min ( this->ElemNum, that.ElemNum );
            int pos = 0;
            for ( ; pos < minAssignElemNum; ++pos )
            {
                m_VecArray[ pos ] = that.m_VecArray[ pos ];
            }
            if ( pos != this->ElemNum )
            {
                VectorHelper::RecursiveAssignVector ( *this, minAssignElemNum, args... )
            }
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
            if ( rhs.ElemNum >= this->ElemNum )
            {
                for ( size_t i = 0; i < this->ElemNum; ++i )
                {
                    m_VecArray[ i ] = rhs[ i ];
                }
            }
            else
            {
                for ( size_t i = 0; i < rhs.ElemNum; ++i )
                {
                    m_VecArray[ i ] = rhs[ i ];
                }
            }
        }

        iterator begin ()
        {
            return m_VecArray.begin ();
        }

        iterator end ()
        {
            return m_VecArray.end ();
        }

        const_iterator cbegin ()
        {
            return m_VecArray.cbegin ();
        }

        const_iterator cend ()
        {
            return m_VecArray.cend ();
        }

        T& x ()
        {
            ZGE_STATIC_ASSART ( ElemNum >= 1 );
            return m_VecArray[ 0 ];
        }

        
        const T& x () const
        {
            ZGE_STATIC_ASSART ( ElemNum >= 1 );
            return m_VecArray[ 0 ];
        }
        

        T& y ()
        {
            ZGE_STATIC_ASSART ( ElemNum >= 2 );
            return m_VecArray[ 1 ];
        }

        const T& y () const
        {
            ZGE_STATIC_ASSART ( ElemNum >= 2 );
            return m_VecArray[ 1 ];
        }

        T& z ()
        {
            ZGE_STATIC_ASSART ( ElemNum >= 3 );
            return m_VecArray[ 2 ];
        }

        const T& z () const
        {
            ZGE_STATIC_ASSART ( ElemNum >= 3 );
            return m_VecArray[ 2 ];
        }

        T& w ()
        {
            ZGE_STATIC_ASSART ( ElemNum >= 4 );
            return m_VecArray[ 3 ];
        }

        const T& w () const
        {
            ZGE_STATIC_ASSART ( ElemNum >= 4 );
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
            assert ( index < ElemNum );
            return m_VecArray[ index ];
        }

        const T& operator [] ( const size_t& index ) const
        {
            assert ( index < ElemNum );
            return m_VecArray[ index ];
        }

        T& operator - ()
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] = -m_VecArray[ i ];
            }
        }

        bool operator == ( const Vector& rhs ) const
        {
            if ( this->ElemNum != rhs.ElemNum )
                return false;

            for ( int i = 0; i < ElemNum; ++i )
            {
                if ( m_VecArray[ i ] != rhs.m_VecArray[ i ] )
                    return false;
            }
            return true;
        }
        
        const Vector& operator += ( const Vector& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] += rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator += ( const T& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] += rhs;
            }
            return *this;
        }

        const Vector& operator -= ( const Vector& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] -= rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator -= ( const T& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] -= rhs;
            }
            return *this;
        }

        const Vector& operator *= ( const Vector& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] *= rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator *= ( const T& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
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
                retrunVector[ i ] = MathFunc::Dot ( *this, rhs.ColVector ( i ) );
            }
            return retrunVector;
        }
        

        const Vector& operator /= ( const Vector& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] /= rhs.m_VecArray[ i ];
            }
            return *this;
        }

        const Vector& operator /= ( const T& rhs )
        {
            for ( int i = 0; i < ElemNum; ++i )
            {
                m_VecArray[ i ] /= rhs;
            }
            return *this;
        }

        T Dot ( const Vector &rhs )
        {
            T dot = T ( 0 );
            for ( size_t i = 0; i < N; ++i )
            {
                dot += ( *this )[ i ] * rhs[ i ];
            }
            return dot;
        }

        Vector & Cross ( const Vector &rhs )
        {
            ZGE_STATIC_ASSART ( N == 3 );


            return *this;
        }

        F32 Length2 ()
        {
            F32 length2 = 0.0f;
            for ( size_t i = 0; i < N; ++i )
            {
                length2 += m_VecArray[ i ] * m_VecArray[ i ];
            }
            return length2;
        }

        F32 Length ()
        {
            return std::sqrtf ( Length2 () );
        }

        Vector & Normalize ()
        {
            F32 length = Length ();
            if ( length != 0.0f )
            {
                for ( size_t i = 0; i < N; ++i )
                {
                    m_VecArray[ i ] /= length;
                }
            }
            return *this;
        }

    protected:
        std::array< T, N > m_VecArray;
    };
    
    class VectorHelper
    {
    public:
        template< typename T, size_t N, typename U, typename... Args >
        static void RecursiveAssignValue ( Vector< T, N >& vector, size_t position, const U& val, const Args&... restVal )
        {
            if ( position < Vector< T, N >::ElemNum )
            {
                vector.m_VecArray[ position ] = val;
                return RecursiveAssignValue ( vector, position + 1, restVal... );
            }
            else
            {
                return;
            }
        }

        template< typename T, size_t N, typename U >
        static void RecursiveAssignValue ( Vector< T, N >& vector, size_t position, const U& val )
        {
            if ( position < Vector< T, N >::ElemNum )
            {
                vector.m_VecArray[ position ] = val;
                ++position;
                while ( position < Vector< T, N >::ElemNum )
                {
                    vector.m_VecArray[ position ] = T ( 0 );
                    ++position;
                }
            }
        }

        template < typename T, size_t NT, typename U, size_t NU, typename... Args >
        static void RecursiveAssignVector ( Vector< T, NT > &vector, size_t position, const Vector< U, NU > &rhs, const Args &... restVal )
        {
            auto minAssignElemNum = std::min ( vector.ElemNum - position, rhs.ElemNum );
            for ( int i = position; i < minAssignElemNum; ++i )
            {
                vector.m_VecArray[ i ] = rhs[ i - position ];
            }
            if ( position + minAssignElemNum < vector.ElemNum )
            {
                return RecursiveAssignVector ( vector, position + minAssignElemNum, restVal... );
            }
            else
            {
                return;
            }
        }

        template < typename T, size_t NT, typename U, size_t NU >
        static void RecursiveAssignVector ( Vector< T, NT > &vector, size_t position, const Vector< U, NU > &rhs )
        {
            auto minAssignElemNum = std::min ( vector.ElemNum - position, rhs.ElemNum );
            for ( int i = position; i < minAssignElemNum; ++i )
            {
                vector.m_VecArray[ i ] = rhs[ i - position ];
            }
            if ( position + minAssignElemNum < vector.ElemNum )
            {
                for ( int i = position + minAssignElemNum; i < vector.ElemNum; ++i )
                {
                    vector.m_VecArray[ i ] = T ( 0 );
                }
            }
            return;
        }
    };
}

#endif // !_MATH_VECTOR_H_