#ifndef _PROPERTY_ANIMCURVE_H_
#define _PROPERTY_ANIMCURVE_H_

#include "ZGEDecl.h"

#include "Core/Asset/Property.h"
#include "AnimCurveNode.h"

namespace ZGE
{
    struct AnimCurve
        : public Property
    {
        enum EType
        {
            eTRANSLATION_X,
            eTRANSLATION_Y,
            eTRANSLATION_Z,
            eROTATION_X,
            eROTATION_Y,
            eROTATION_Z,
            eSCALING_X,
            eSCALING_Y,
            eSCALING_Z,
        };

        EType Type;

        std::vector< PAnimCurveNode > AnimCurveNodeList;
    };

    typedef std::shared_ptr< AnimCurve > PAnimCurve;
}

#endif // !_PROPERTY_ANIMCURVE_H_

