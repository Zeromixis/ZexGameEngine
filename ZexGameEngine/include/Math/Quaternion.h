#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "External/boost/operators.hpp"

#include "Math/Vector.h"
#include "Math/MathConst.h"

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

        Quaternion (const Quaternion &that)
        {
            m_Quat = that.m_Quat;
        }

        Quaternion (Quaternion &&that)
        {
            m_Quat = std::move (that.m_Quat);
        }

        Quaternion (const F32 &x, const F32 &y, const F32 &z, const F32 &w)
            : m_Quat (x, y, z, w)
        {

        }

        template< typename U >
        explicit constexpr Quaternion (const Vector< U, 3 > &vec)
            : m_Quat (vec, 0.0f)
        {

        }

        template< typename U >
        explicit Quaternion (const Vector< U, 4 > &vec)
            : m_Quat (vec [0], vec [1], vec [2], 0.0f)
        {

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

        F32 & operator [] (const size_t &index)
        {
            return m_Quat [index];
        }

        const F32 & operator [] (const size_t &index) const
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

            m_Quat = Vector4f{x, y, z, w};

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

        template< typename U, typename R >
        Quaternion & FromAxisRadian (const Vector< U, 3 > &axis, const R &radian)
        {
            x () = axis.x () * std::sin (radian / 2.0);
            y () = axis.y () * std::sin (radian / 2.0);
            z () = axis.z () * std::sin (radian / 2.0);
            w () = std::cos (radian / 2.0);
            return *this;
        }

        template< typename U, typename R >
        Quaternion & FromAxisDegree (const Vector< U, 3 > &axis, const R &degree)
        {
            return FromAxisRadian (axis, MathFunc::DegreeToRadian (degree));
        }

        template< typename U, typename R >
        void ToAxisRadian (Vector< U, 3 > &axis, R &radian) const
        {
            Quaternion quat = *this;
            quat.Normalize ();
            radian = 2.0f * std::acos (w ());
            auto s = std::sqrt (1 - w () * w ());
            // If closed to zero////////
            if (s < 0.000001f)
            {
                axis = quat.m_Quat;
            }
            else
            {
                axis = quat.m_Quat / s;
            }
        }

        template< typename U, typename R >
        void ToAxisDegree (Vector< U, 3 > &axis, R &degree) const
        {
            R radian (0);
            ToAxisRadian (axis, radian);
            degree = MathFunc::RadianToDegree (radian);
        }

        Float44 ToMatrix44 () const
        {
            Float44 retMatrix;
            const auto xx = x () * x ();
            const auto xy = x () * y ();
            const auto xz = x () * z ();
            const auto xw = x () * w ();
            const auto yy = y () * y ();
            const auto yz = y () * z ();
            const auto yw = y () * w ();
            const auto zz = z () * z ();
            const auto zw = z () * w ();

            retMatrix [0] [0] = 1 - 2 * (yy + zz);
            retMatrix [0] [1] = 2 * (xy - zw);
            retMatrix [0] [2] = 2 * (xz + yw);

            retMatrix [1] [0] = 2 * (xy + zw);
            retMatrix [1] [1] = 1 - 2 * (xx + zz);
            retMatrix [1] [2] = 2 * (yz - xw);

            retMatrix [2] [0] = 2 * (xz - yw);
            retMatrix [2] [1] = 2 * (yz + xw);
            retMatrix [2] [2] = 1 - 2 * (xx + yy);

            return retMatrix;
        }

        template< typename U >
        Quaternion & FromEulerRadian (const Vector< U, 3 > &eulerRadian)
        {
            const auto c1 = std::cos (eulerRadian.y () / 2);
            const auto s1 = std::sin (eulerRadian.y () / 2);
            const auto c2 = std::cos (eulerRadian.x () / 2);
            const auto s2 = std::sin (eulerRadian.x () / 2);
            const auto c3 = std::cos (eulerRadian.z () / 2);
            const auto s3 = std::sin (eulerRadian.z () / 2);

            const auto c1c2 = c1 * c2;
            const auto s1s2 = s1 * s2;

            z () = c1c2 * s3 + s1s2 * c3;
            y () = s1 * c2 * c3 + c1 * s2 * s3;
            x () = c1 * s2 * c3 - s1 * c2 * s3;
            w () = c1c2 * c3 - s1s2 * s3;

            return *this;
        }

        template< typename U >
        Quaternion & FromEulerDegree (const Vector< U, 3 > &eulerDegree)
        {
            Vector< U, 3 > eulerRadian;
            for (int i = 0; i < 3; ++i)
            {
                eulerRadian [i] = MathFunc::DegreeToRadian (eulerDegree [i]);
            }
            return FromEulerRadian (eulerRadian);
        }

        Vector3f ToEulerRadian () const
        {
            Vector3f retVec;
            const auto sqx = x () * x ();
            const auto sqy = y () * y ();
            const auto sqz = z () * z ();
            const auto sqw = w () * w ();

            const auto unit = sqx + sqy + sqz + sqw;
            const auto test = w () * x () + y () * z ();
            if (test > 0.4999f * unit)
            {
                retVec.y () = 2.0f * std::atan2 (z (), w ());
                retVec.x () = MathConst::PI * 0.5f;
                retVec.z () = 0.0f;
            }
            else if (test < -0.4999f * unit)
            {
                retVec.y () = -2.0f * std::atan2 (z (), w ());
                retVec.x () = -MathConst::PI * 0.5f;
                retVec.z () = 0.0f;
            }
            else
            {
                retVec.y () = std::atan2 (2.0f * y () * w () - 2.0f * x () * z (), -sqx - sqy + sqz + sqw);
                retVec.x () = std::asin (2.0f * test / unit);
                retVec.z () = std::atan2 (2.0f * z () * w () - 2.0f * x () * y (), -sqx + sqy - sqz + sqw);
            }
            return retVec;
        }

        Vector3f ToEulerDegree () const
        {
            auto vec = ToEulerRadian ();
            for (auto &e : vec)
            {
                e = MathFunc::RadianToDegree (e);
            }
            return vec;
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
