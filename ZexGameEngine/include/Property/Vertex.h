#ifndef _CORE_PROPERTY_VERTEX_H_
#define _CORE_PROPERTY_VERTEX_H_

#include "ZGEDecl.h"
#include "Math/Vector.h"
#include "Core/Asset/Property.h"

namespace ZGE
{
    struct Vertex
    {
        constexpr static U32 VERTEX_LINK_JOINT_MAX_NUM = 4;

        Vertex ()
        {
            Position = Vector4f ( 0.0f, 0.0f, 0.0f, 1.0f );
            Color   = Vector4f ( 0.0f, 0.0f, 0.0f, 1.0f );
            Normal  = Vector4f ( 0.0f, 0.0f, 0.0f, 1.0f );
            UV      = Vector2f ( 0.0f, 0.0f );

            JointIndexWeightPairList.reserve ( VERTEX_LINK_JOINT_MAX_NUM );

        }

        ~Vertex ()
        {
            
        }

        //PControlPoint VertexControlPoint;

        Vector4f Position;

        Vector4f Color;         // RGBA

        Vector4f Normal;

        Vector2f UV;

        std::vector< std::pair < U32, I32 > > JointIndexWeightPairList;
    };
}

#endif
