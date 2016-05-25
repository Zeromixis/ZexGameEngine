#ifndef _PROPERTY_ANIMCURVENODE_H_
#define _PROPERTY_ANIMCURVENODE_H_

#include "ZGEDecl.h"

#include "Core/Asset/Property.h"

namespace ZGE
{
    struct AnimCurveNode
        : public Property
    {
        enum EInterpolationType
        {
            eConstant,
            eLinear,
            eCubic,
        };

        AnimCurveNode ()
        {
            Time = 0.0f;
            Value = 0.0f;
            InterpolationType = eLinear;
        }

        ~AnimCurveNode ()
        {

        }

        EInterpolationType InterpolationType;
        F32 Time;
        F32 Value;

    };

    typedef std::shared_ptr< AnimCurveNode > PAnimCurveNode;
}

#endif // !_PROPERTY_ANIMCURVENODE_H_