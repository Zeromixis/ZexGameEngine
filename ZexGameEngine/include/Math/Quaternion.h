#ifndef _MATH_QUATERNION_H_
#define _MATH_QUATERNION_H_

#include "ZGEDecl.h"

#include "External/boost/operators.hpp"

#include "Math/Vector.h"

namespace ZGE
{
    class Quaternion
        : boost::addable< Quaternion >
        , boost::subtractable< Quaternion >
        , boost::dividable2< Quaternion, F32 >
        , boost::multipliable< Quaternion >
        , boost::multipliable2< Quaternion, F32 >
        , boost::equality_comparable< Quaternion >
    {

    public:
        constexpr static unsigned int size = 4;

        typedef F32                                     value_type;
        typedef Vector4f                                VecArrayType;
        typedef typename VecArrayType::iterator         iterator;
        typedef typename VecArrayType::const_iterator   const_iterator;

        Quaternion ()
        {

        }

        ~Quaternion ()
        {

        }

        Quaternion (const Quaternion &that)
        {
            m_Quat = that.m_Quat;
        }

        Quaternion (Quaternion &&that)
        {
            m_Quat = std::move (that.m_Quat);
        }

        Quaternion (const F32 &x, const F32 &y, const F32 &z, const F32 &w)
        {
            m_Quat [0] = x;
            m_Quat [1] = y;
            m_Quat [2] = z;
            m_Quat [3] = w;
        }

        template< typename U >
        explicit Quaternion (const Vector< U, 3 > &vec)
        {
            m_Quat [0] = vec [0];
            m_Quat [1] = vec [1];
            m_Quat [2] = vec [2];
            m_Quat [3] = 0;
        }

        template< typename U >
        explicit Quaternion (const Vector< U, 4 > &vec)
        {
            m_Quat [0] = vec [0];
            m_Quat [1] = vec [1];
            m_Quat [2] = vec [2];
            m_Quat [3] = 0;
        }

        F32 & x ()
        {
            return m_Quat [0];
        }

        const F32 & x () const
        {
            return m_Quat [0];
        }

        F32 & y ()
        {
            return m_Quat [1];
        }

        const F32 & y () const
        {
            return m_Quat [1];
        }

        F32 & z ()
        {
            return m_Quat [2];
        }

        const F32 & z () const
        {
            return m_Quat [2];
        }

        F32 & w ()
        {
            return m_Quat [3];
        }

        const F32 & w () const
        {
            return m_Quat [3];
        }

        iterator begin ()
        {
            return m_Quat.begin ();
        }

        iterator end ()
        {
            return m_Quat.end ();
        }

        const_iterator cbegin ()
        {
            return m_Quat.cbegin ();
        }

        const_iterator cend ()
        {
            return m_Quat.cend ();
        }

        F32 & operator [] (size_t index)
        {
            return m_Quat [index];
        }

        const F32 & operator [] (size_t index) const
        {
            return m_Quat [index];
        }

        Quaternion & operator = (const Quaternion &that)
        {
            if (this != &that)
            {
                m_Quat = that.m_Quat;
            }
            return *this;
        }

        Quaternion & operator = (const Quaternion &&that)
        {
            if (this != &that)
            {
                m_Quat = std::move (that.m_Quat);
            }
            return *this;
        }

        const Quaternion & operator += (const Quaternion &that)
        {
            m_Quat += that.m_Quat;
            return *this;
        }

        const Quaternion & operator -= (const Quaternion &that)
        {
            m_Quat -= that.m_Quat;
            return *this;
        }

        const Quaternion & operator /= (const F32 &rhs)
        {
            m_Quat /= rhs;
            return *this;
        }

        const Quaternion & operator *= (const Quaternion &that)
        {
            auto q1x = m_Quat [0], q1y = m_Quat [1], q1z = m_Quat [2], q1w = m_Quat [3];
            auto q2x = that.m_Quat [0], q2y = that.m_Quat [1], q2z = that.m_Quat [2], q2w = that.m_Quat [3];

            auto x = q1x * q2w + q1y * q2z - q1z * q2y + q1w * q2x;
            auto y = -q1x * q2z + q1y * q2w + q1z * q2x + q1w * q2y;
            auto z = q1x * q2y - q1y * q2x + q1z * q2w + q1w * q2z;
            auto w = -q1x * q2x - q1y * q2y - q1z * q2z + q1w * q2w;

            m_Quat = Vector4f {x, y, z, w};

            return *this;
        }

        const Quaternion & operator *= (const F32 &rhs)
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
            return Quaternion (m_Quat.x (), m_Quat.y (), m_Quat.z (), m_Quat.w ());
        }

        bool operator == (const Quaternion &that) const
        {
            return m_Quat == that.m_Quat;
        }

        Quaternion & Normalize ()
        {
            m_Quat.Normalize ();
            return *this;
        }

        const Quaternion GetConjugate () const
        {
            return Quaternion (-x (), -y (), -z (), w ());
        }

        static const Quaternion & Identity ()
        {
            static const Quaternion value (0, 0, 0, 1);
            return value;
        }

    private:
        Vector4f m_Quat;
    };

    template< typename T >
    static decltype (auto) operator * (const Vector< T, 4 > &vec, const Quaternion &quat)
    {
        Vector< T, 4 >::value_type t1 (1);
        Quaternion::value_type t2 (1);

        Vector< decltype (t1 * t2), 4 > retVec;
        Quaternion q = Quaternion (vec) * quat;

        retVec [0] = q [0];
        retVec [1] = q [1];
        retVec [2] = q [2];
        retVec [3] = vec [3];

        return retVec;
    }

    template< typename T >
    static decltype (auto) operator * (const Quaternion &quat, const Vector< T, 4 > &vec)
    {
        Vector< T, 4 >::value_type t1 (1);
        Quaternion::value_type t2 (1);

        Vector< decltype (t1 * t2), 4 > retVec;
        Quaternion q = quat * Quaternion (vec);

        retVec [0] = q [0];
        retVec [1] = q [1];
        retVec [2] = q [2];
        retVec [3] = vec [3];

        return retVec;
    }

    template< typename T >
    static decltype (auto) operator * (const Vector< T, 3 > &vec, const Quaternion &quat)
    {
        Vector< T, 3 >::value_type t1 (1);
        Quaternion::value_type t2 (1);

        Vector< decltype (t1 * t2), 3 > retVec;
        Quaternion q = Quaternion (vec) * quat;

        retVec [0] = q [0];
        retVec [1] = q [1];
        retVec [2] = q [2];

        return retVec;
    }

    template< typename T >
    static decltype (auto) operator * (const Quaternion &quat, const Vector< T, 3 > &vec)
    {
        Vector< T, 3 >::value_type t1 (1);
        Quaternion::value_type t2 (1);

        Vector< decltype (t1 * t2), 3 > retVec;
        Quaternion q = quat * Quaternion (vec);

        retVec [0] = q [0];
        retVec [1] = q [1];
        retVec [2] = q [2];

        return retVec;
    }
}

#endif // !_MATH_QUATERNION_H_
