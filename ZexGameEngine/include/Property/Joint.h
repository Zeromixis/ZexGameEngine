#ifndef _PROPERTY_JOINT_H_
#define _PROPERTY_JOINT_H_

#include "ZGEDecl.h"

#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace ZGE
{
    struct Joint
    {
        Joint ()
        {
            Joint2WorldBindPoseMatrix = Float44::Identity ();
        }

        std::string Name;

        Float44 Joint2WorldBindPoseMatrix;
    };
}

#endif // !_PROPERTY_JOINT_H_