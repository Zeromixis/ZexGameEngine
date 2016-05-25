#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "Core/Asset/Property.h"
#include "Property/Vertex.h"
#include "Property/Joint.h"
#include "Property/AnimStack.h"
#include "Math/Matrix.h"

namespace ZGE
{
    struct Mesh
        : public Property
    {
        std::vector< Vertex > VertexList;

        std::vector< U32 > IndexList;

        std::vector< Joint > JointList;

        std::vector< PAnimStack > AnimStackList;

        Float44 GeometryTransformMatrix;

        Float44 Mesh2WorldMatrix;
    };

    typedef PropertyHandle< Mesh > PMesh;
}

