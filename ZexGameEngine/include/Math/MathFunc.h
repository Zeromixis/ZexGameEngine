#ifndef _MATH_MATHFUNC_H_
#define _MATH_MATHFUNC_H_

#include "ZGEDecl.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"


namespace ZGE
{
    struct MathFunc
    {
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

        template< typename T >
        static Vector< T, 3 > Cross ( const Vector< T, 3 >& lhs, const Vector< T, 3 >& rhs )
        {
            return Vector< T, 3 >
                (
                    lhs.y () * rhs.z () - lhs.z () * rhs.y (),
                    lhs.z () * rhs.x () - lhs.x () * rhs.z (),
                    lhs.x () * rhs.y () - lhs.y () * rhs.x ()
                    );
        }

        template< typename T, size_t N >
        static T Dot ( const Vector< T, N >& lhs, const Vector< T, N >& rhs )
        {
            T dot = T ( 0 );
            for ( size_t i = 0; i < N; ++i )
            {
                dot += lhs[ i ] * rhs[ i ];
            }
            return dot;
        }

        template < typename T >
        static Matrix44< T > OrthoLH ( const T& width, const T& height, const T& zNearPlane, const T& zFarPlane )
        {
            const T w_2 ( width / 2 );
            const T h_2 ( height / 2 );
            return OrthoOffCenterLH ( -w_2, w_2, -h_2, h_2, zNearPlane, zFarPlane );
        }

        template < typename T >
        static Matrix44< T > OrthoOffCenterLH ( const T& left, const T& right, const T& bottom, const T& top, const T& zNearPlane, const T& zFarPlane )
        {
            const T q ( T ( 1 ) / ( zFarPlane - zNearPlane ) );
            const T invWidth ( T ( 1 ) / ( right - left ) );
            const T invHeight ( T ( 1 ) / ( top - bottom ) );

            return Matrix44< T >
                (
                    invWidth + invWidth,            0,                              0,                  0,
                    0,                              invHeight + invHeight,          0,                  0,
                    0,                              0,                              q,                  0,
                    -( left + right ) * invWidth,   -( top + bottom ) * invHeight,  -zNearPlane * q,    1
                    );
        }

        template < typename T >
        static Matrix44< T > PerspectiveFovLH ( const T& fov, const T& aspect, const T& zNearPlane, const T& zFarPlane )
        {
            const T h ( T ( 1 ) / tan ( fov / T ( 2 ) ) );
            const T w ( h / aspect );
            const T q ( zFarPlane / ( zFarPlane - zNearPlane ) );
            return Matrix44< T >
                (
                    w, 0, 0,                0,
                    0, h, 0,                0,
                    0, 0, q,                1,
                    0, 0, -zNearPlane * q,  0
                    );
        }
    };
}

#endif // !_MATH_MATHFUNC_H_

