#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/MathConst.h"


namespace ZGE
{
    struct MathFunc
    {
        template< typename T, size_t N >
        static F32 Length (const Vector< T, N >& vec)
        {
            F32 length = 0.0f;
            for (size_t i = 0; i < N; ++i)
            {
                length += vec [i] * vec [i];
            }
            return std::sqrtf (length);
        }

        template< typename T, size_t N >
        static Vector< T, N > Normalize (const Vector< T, N >& vec)
        {
            Vector< T, N > result;
            F32 length = Length (vec);
            if (length != 0.0f)
            {
                for (size_t i = 0; i < N; ++i)
                {
                    result [i] = vec [i] / length;
                }
            }
            return result;
        }

        template< typename T >
        static Vector< T, 3 > Cross (const Vector< T, 3 >& lhs, const Vector< T, 3 >& rhs)
        {
            return Vector< T, 3 >
                (
                    lhs.y () * rhs.z () - lhs.z () * rhs.y (),
                    lhs.z () * rhs.x () - lhs.x () * rhs.z (),
                    lhs.x () * rhs.y () - lhs.y () * rhs.x ()
                    );
        }

        template< typename T, size_t N >
        static T Dot (const Vector< T, N >& lhs, const Vector< T, N >& rhs)
        {
            T dot = T (0);
            for (size_t i = 0; i < N; ++i)
            {
                dot += lhs [i] * rhs [i];
            }
            return dot;
        }

        template < typename T >
        static Matrix44< T > OrthoLH (const T& width, const T& height, const T& zNearPlane, const T& zFarPlane)
        {
            const T w_2 (width / 2);
            const T h_2 (height / 2);
            return OrthoOffCenterLH (-w_2, w_2, -h_2, h_2, zNearPlane, zFarPlane);
        }

        template < typename T >
        static Matrix44< T > OrthoOffCenterLH (const T& left, const T& right, const T& bottom, const T& top, const T& zNearPlane, const T& zFarPlane)
        {
            const T q (T (1) / (zFarPlane - zNearPlane));
            const T invWidth (T (1) / (right - left));
            const T invHeight (T (1) / (top - bottom));

            return Matrix44< T >
                (
                    invWidth + invWidth, 0, 0, 0,
                    0, invHeight + invHeight, 0, 0,
                    0, 0, q, 0,
                    -(left + right) * invWidth, -(top + bottom) * invHeight, -zNearPlane * q, 1
                    );
        }

        template< typename T >
        static Matrix44< T > PerspectiveFovLH (const T& fov, const T& aspect, const T& zNearPlane, const T& zFarPlane)
        {
            const T h (T (1) / tan (fov / T (2)));
            const T w (h / aspect);
            const T q (zFarPlane / (zFarPlane - zNearPlane));
            return Matrix44< T >
                (
                    w, 0, 0, 0,
                    0, h, 0, 0,
                    0, 0, q, 1,
                    0, 0, -zNearPlane * q, 0
                    );
        }

        template< typename T >
        static Matrix44< T > CreateScaleMatrix (const T &scaleX, const T &scaleY, const T &scaleZ)
        {
            Matrix44< T > mat
            (
                scaleX, 0, 0, 0,
                0, scaleY, 0, 0,
                0, 0, scaleZ, 0,
                0, 0, 0, 1
            );
            return mat;
        }

        template< typename T >
        static Matrix44< T > CreateScaleMatrix (const T &scaleXYZ)
        {
            return CreateScaleMatrix (scaleXYZ, scaleXYZ, scaleXYZ);
        }

        template < typename T >
        static T MatrixDeterminant (const Matrix44< T > &rhs)
        {
            T const _3142_3241 (rhs (2, 0) * rhs (3, 1) - rhs (2, 1) * rhs (3, 0));
            T const _3143_3341 (rhs (2, 0) * rhs (3, 2) - rhs (2, 2) * rhs (3, 0));
            T const _3144_3441 (rhs (2, 0) * rhs (3, 3) - rhs (2, 3) * rhs (3, 0));
            T const _3243_3342 (rhs (2, 1) * rhs (3, 2) - rhs (2, 2) * rhs (3, 1));
            T const _3244_3442 (rhs (2, 1) * rhs (3, 3) - rhs (2, 3) * rhs (3, 1));
            T const _3344_3443 (rhs (2, 2) * rhs (3, 3) - rhs (2, 3) * rhs (3, 2));

            return rhs (0, 0) * (rhs (1, 1) * _3344_3443 - rhs (1, 2) * _3244_3442 + rhs (1, 3) * _3243_3342)
                - rhs (0, 1) * (rhs (1, 0) * _3344_3443 - rhs (1, 2) * _3144_3441 + rhs (1, 3) * _3143_3341)
                + rhs (0, 2) * (rhs (1, 0) * _3244_3442 - rhs (1, 1) * _3144_3441 + rhs (1, 3) * _3142_3241)
                - rhs (0, 3) * (rhs (1, 0) * _3243_3342 - rhs (1, 1) * _3143_3341 + rhs (1, 2) * _3142_3241);
        }

        template < typename T >
        static Matrix44< T > MatrixInverse (const Matrix44< T > &rhs)
        {
            T const _2132_2231 (rhs (1, 0) * rhs (2, 1) - rhs (1, 1) * rhs (2, 0));
            T const _2133_2331 (rhs (1, 0) * rhs (2, 2) - rhs (1, 2) * rhs (2, 0));
            T const _2134_2431 (rhs (1, 0) * rhs (2, 3) - rhs (1, 3) * rhs (2, 0));
            T const _2142_2241 (rhs (1, 0) * rhs (3, 1) - rhs (1, 1) * rhs (3, 0));
            T const _2143_2341 (rhs (1, 0) * rhs (3, 2) - rhs (1, 2) * rhs (3, 0));
            T const _2144_2441 (rhs (1, 0) * rhs (3, 3) - rhs (1, 3) * rhs (3, 0));
            T const _2233_2332 (rhs (1, 1) * rhs (2, 2) - rhs (1, 2) * rhs (2, 1));
            T const _2234_2432 (rhs (1, 1) * rhs (2, 3) - rhs (1, 3) * rhs (2, 1));
            T const _2243_2342 (rhs (1, 1) * rhs (3, 2) - rhs (1, 2) * rhs (3, 1));
            T const _2244_2442 (rhs (1, 1) * rhs (3, 3) - rhs (1, 3) * rhs (3, 1));
            T const _2334_2433 (rhs (1, 2) * rhs (2, 3) - rhs (1, 3) * rhs (2, 2));
            T const _2344_2443 (rhs (1, 2) * rhs (3, 3) - rhs (1, 3) * rhs (3, 2));
            T const _3142_3241 (rhs (2, 0) * rhs (3, 1) - rhs (2, 1) * rhs (3, 0));
            T const _3143_3341 (rhs (2, 0) * rhs (3, 2) - rhs (2, 2) * rhs (3, 0));
            T const _3144_3441 (rhs (2, 0) * rhs (3, 3) - rhs (2, 3) * rhs (3, 0));
            T const _3243_3342 (rhs (2, 1) * rhs (3, 2) - rhs (2, 2) * rhs (3, 1));
            T const _3244_3442 (rhs (2, 1) * rhs (3, 3) - rhs (2, 3) * rhs (3, 1));
            T const _3344_3443 (rhs (2, 2) * rhs (3, 3) - rhs (2, 3) * rhs (3, 2));


            T const det (MatrixDeterminant (rhs));
            if (det != 0)
            {
                T invDet (T (1) / det);

                return Float44
                (
                    +invDet * (rhs (1, 1) * _3344_3443 - rhs (1, 2) * _3244_3442 + rhs (1, 3) * _3243_3342),
                    -invDet * (rhs (0, 1) * _3344_3443 - rhs (0, 2) * _3244_3442 + rhs (0, 3) * _3243_3342),
                    +invDet * (rhs (0, 1) * _2344_2443 - rhs (0, 2) * _2244_2442 + rhs (0, 3) * _2243_2342),
                    -invDet * (rhs (0, 1) * _2334_2433 - rhs (0, 2) * _2234_2432 + rhs (0, 3) * _2233_2332),

                    -invDet * (rhs (1, 0) * _3344_3443 - rhs (1, 2) * _3144_3441 + rhs (1, 3) * _3143_3341),
                    +invDet * (rhs (0, 0) * _3344_3443 - rhs (0, 2) * _3144_3441 + rhs (0, 3) * _3143_3341),
                    -invDet * (rhs (0, 0) * _2344_2443 - rhs (0, 2) * _2144_2441 + rhs (0, 3) * _2143_2341),
                    +invDet * (rhs (0, 0) * _2334_2433 - rhs (0, 2) * _2134_2431 + rhs (0, 3) * _2133_2331),

                    +invDet * (rhs (1, 0) * _3244_3442 - rhs (1, 1) * _3144_3441 + rhs (1, 3) * _3142_3241),
                    -invDet * (rhs (0, 0) * _3244_3442 - rhs (0, 1) * _3144_3441 + rhs (0, 3) * _3142_3241),
                    +invDet * (rhs (0, 0) * _2244_2442 - rhs (0, 1) * _2144_2441 + rhs (0, 3) * _2142_2241),
                    -invDet * (rhs (0, 0) * _2234_2432 - rhs (0, 1) * _2134_2431 + rhs (0, 3) * _2132_2231),

                    -invDet * (rhs (1, 0) * _3243_3342 - rhs (1, 1) * _3143_3341 + rhs (1, 2) * _3142_3241),
                    +invDet * (rhs (0, 0) * _3243_3342 - rhs (0, 1) * _3143_3341 + rhs (0, 2) * _3142_3241),
                    -invDet * (rhs (0, 0) * _2243_2342 - rhs (0, 1) * _2143_2341 + rhs (0, 2) * _2142_2241),
                    +invDet * (rhs (0, 0) * _2233_2332 - rhs (0, 1) * _2133_2331 + rhs (0, 2) * _2132_2231)
                );
            }
            else
            {
                return rhs;
            }
        }

        template< typename T >
        static decltype(auto) DegreeToRadian (const T &degree)
        {
            return degree * MathConst::PI / T (180);
        }

        template< typename T >
        static decltype(auto) RadianToDegree (const T &radian)
        {
            return radian / MathConst::PI * T (180);
        }

        template< typename U, typename R >
        static Quaternion FromAxisDegree (const Vector< U, 3 > &axis, const R &degree)
        {
            Quaternion quat;
            return quat.FromAxisDegree (axis, degree);
        }

        template< typename U, typename R >
        static Quaternion FromAxisRadian (const Vector< U, 3 > &axis, const R &radian)
        {
            Quaternion quat;
            return quat.FromAxisRadian (axis, radian);
        }

        template< typename U >
        static Quaternion FromEulerDegree (const Vector< U, 3 > &eulerDegree)
        {
            Quaternion quat;
            return quat.FromEulerDegree (eulerDegree);
        }

        template< typename U >
        static Quaternion FromEulerRadian (const Vector< U, 3 > &eulerRadian)
        {
            Quaternion quat;
            return quat.FromEulerRadian (eulerRadian);
        }
    };
}

