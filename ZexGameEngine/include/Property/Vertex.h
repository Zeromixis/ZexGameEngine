#ifndef _CORE_PROPERTY_VERTEX_H_
#define _CORE_PROPERTY_VERTEX_H_

#include "ZGEDecl.h"
#include "Math/Vector.h"
#include "Core/Asset/Property.h"

namespace ZGE
{
    struct ControlPoint
        : public Property
    {
        constexpr static U32 VERTEX_LINK_JOINT_MAX_NUM = 4;

        ControlPoint ()
        {
            Position = Vector4f ( 0.0f, 0.0f, 0.0f, 1.0f );

            JointIndexWeightPairList.reserve ( VERTEX_LINK_JOINT_MAX_NUM );
        }

        Vector4f Position;

        std::vector< std::pair < U32, I32 > > JointIndexWeightPairList;
    };

    using PControlPoint = PropertyHandle< ControlPoint >/* PControlPoint*/;

    struct Vertex
        : public Property
    {
        constexpr static U32 VERTEX_LINK_JOINT_MAX_NUM = 4;

        Vertex ()
        {
            Color   = Vector4f ( 0.0f, 0.0f, 0.0f, 1.0f );
            Normal  = Vector4f ( 0.0f, 0.0f, 0.0f, 1.0f );
            UV      = Vector2f ( 0.0f, 0.0f );
        }

        ~Vertex ()
        {
            
        }

        PControlPoint VertexControlPoint;

        Vector4f Color;         // RGBA

        Vector4f Normal;

        Vector2f UV;
    };

    typedef PropertyHandle< Vertex > PVertex;
}

#endif
