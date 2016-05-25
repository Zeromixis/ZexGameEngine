#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

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
