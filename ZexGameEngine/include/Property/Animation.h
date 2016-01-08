#ifndef _PROPERTY_ANIMATION_H_
#define _PROPERTY_ANIMATION_H_

#include "ZGEDecl.h"

#include "Core/Asset/Property.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"

namespace ZGE
{
    struct AnimKeyFrame
    {
        constexpr static U32 FRAME_RATE = 60;

        U32 Frame;

        // This data has transformed to Global.
        Vector4f Translation;

        QuaternionF Rotation;

        Vector4f Scaling;
    };

    typedef PropertyHandle< AnimKeyFrame > PAnimKeyFrame;

    struct AnimData
        : public Property
    {
        std::string Name;                       // Animation Name

        std::vector< PAnimKeyFrame > KeyFrameList;
    };

    typedef PropertyHandle< AnimData > PAnimData;

    struct AnimNode
        : public Property
    {
        std::string Name;                       // Node Name

        std::vector< PAnimData > AnimDataList;  // All Animation Data
    };

    typedef PropertyHandle< AnimNode > PAnimNode;
}

#endif // !_PROPERTY_ANIMATION_H_

