#ifndef _PROPERTY_ANIMCURVES_H_
#define _PROPERTY_ANIMCURVES_H_

#include "ZGEDecl.h"

#include "Core/Asset/Property.h"
#include "AnimCurve.h"

namespace ZGE
{
    struct AnimCurves
        : public Property
    {
        std::string JointName;

        U32 JointIndex;

        std::vector< PAnimCurve > AniCurveNodeList;
    };

    typedef std::shared_ptr< AnimCurves > PAnimCurves;
}

#endif // !_PROPERTY_ANIMCURVES_H_

