#ifndef _MATH_QUATERNION_H_
#define _MATH_QUATERNION_H_

#include "ZGEDecl.h"

#include "External/boost/operators.hpp"

#include "Math/Vector.h"

namespace ZGE
{
    typedef Quaternion< F32 > QuaternionF;

    template < typename T >
    class Quaternion
        : boost::addable< Quaternion< T > >
        , boost::subtractable< Quaternion< T > >
        , boost::dividable2< Quaternion< T >, T >
        , boost::multipliable< Quaternion< T > >
        , boost::multipliable2< Quaternion< T >, T >
        , boost::equality_comparable< Quaternion< T > >
    {
         
    public:
        constexpr static unsigned int size = 4;

        typedef T                               value_type;
        typedef Vector< T, size >               VecArrayType;
        typedef typename VecArrayType::iterator          iterator;
        typedef typename VecArrayType::const_iterator    const_iterator;

        enum { ElemNum = 4 };

        Quaternion ()
        {

        }

        ~Quaternion ()
        {

        }

        Quaternion ( const Quaternion &that )
        {
            m_Quat = that.m_Quat;
        }

        Quaternion ( Quaternion &&that )
        {
            m_Quat = std::move ( that.m_Quat );
        }

        Quaternion ( const T& x, const T& y, const T& z, const T& w )
        {
            m_Quat[ 0 ] = x;
            m_Quat[ 1 ] = y;
            m_Quat[ 2 ] = z;
            m_Quat[ 3 ] = w;
        }

        T& x ()
        {
            return m_Quat[ 0 ];
        }

        const T& x () const
        {
            return m_Quat[ 0 ];
        }

        T& y ()
        {
            return m_Quat[ 1 ];
        }

        const T& y () const
        {
            return m_Quat[ 1 ];
        }

        T& z ()
        {
            return m_Quat[ 2 ];
        }

        const T& z () const
        {
            return m_Quat[ 2 ];
        }

        T& w ()
        {
            return m_Quat[ 3 ];
        }

        const T& w () const
        {
            return m_Quat[ 3 ];
        }

        iterator begin ()
        {
            return m_Quat.begin ();
        }

        iterator end ()
        {
            return m_Quat.end ();
        }

//         const_iterator begin ()
//         {
//             return m_Quat.begin ();
//         }
// 
//         const_iterator end ()
//         {
//             return m_Quat.end ();
//         }

        const_iterator cbegin ()
        {
            return m_Quat.cbegin ();
        }

        const_iterator cend ()
        {
            return m_Quat.cend ();
        }

        T& operator [] ( size_t index )
        {
            return m_Quat[ index ];
        }

        const T& operator [] ( size_t index ) const
        {
            return m_Quat[ index ];
        }

        Quaternion & operator = ( const Quaternion &that )
        {
            if ( this != &rhs )
            {
                m_Quat = that.m_Quat;
            }
            return *this;
        }

        Quaternion & operator = ( const Quaternion &&that )
        {
            if ( this != &rhs )
            {
                m_Quat = std::move ( that.m_Quat );
            }
            return *this;
        }

        const Quaternion & operator += ( const Quaternion &that )
        {
            m_Quat += that.m_Quat;
            return *this;
        }

        const Quaternion & operator -=( const Quaternion &that )
        {
            m_Quat -= that.m_Quat;
            return *this;
        }

        const Quaternion & operator /= ( const T &rhs )
        {
            m_Quat /= rhs;
            return *this;
        }

        const Quaternion & operator *= ( const Quaternion &that )
        {
            auto q1x = m_Quat[ 0 ],         q1y = m_Quat[ 1 ],          q1z = m_Quat[ 2 ],      q1w = m_Quat[ 3 ];
            auto q2x = that.m_Quat[ 0 ],    q2y = that.m_Quat[ 1 ],     q2z = that.m_Quat[ 2 ], q2w = that.m_Quat[ 3 ];

            auto x = q1x * q2w  + q1y * q2z - q1z * q2y + q1w * q2x;
            auto y = -q1x * q2z + q1y * q2w + q1z * q2x + q1w * q2y;
            auto z = q1x * q2y  - q1y * q2x + q1z * q2w + q1w * q2z;
            auto w = -q1x * q2x - q1y * q2y - q1z * q2z + q1w * q2w;

            m_Quat = Vector { x, y, z, w };

            return *this;
        }

        const Quaternion & operator *= ( const T &rhs )
        {
            m_Quat *= rhs;
            return *this;
        }
        
        const Quaternion operator + () const
        {
            return *this;
        }

        const Quaternion operator - () const
        {
            return Quaternion ( m_Quat.x (), m_Quat.y (), m_Quat.z (), m_Quat.w () );
        }

        bool operator == ( const Quaternion &that )
        {
            return m_Quat == that.m_Quat;
        }

        Quaternion & Normalize ()
        {
            m_Quat.Normalize ();
            return *this;
        }

        static const Quaternion & Identity ()
        {
            static const Quaternion value ( 0, 0, 0, 1 );
            return value;
        }

    private:
        Vector< T, ElemNum > m_Quat;
    };
}

#endif // !_MATH_QUATERNION_H_
