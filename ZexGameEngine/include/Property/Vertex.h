#ifndef _CORE_PROPERTY_VERTEX_H_
#define _CORE_PROPERTY_VERTEX_H_

#include "Math/Vector.h"

namespace ZGE
{
    struct Vertex
    {
        Vector3f Position;

        Vector4f Color;         // RGBA

        Vector3f Normal;

        Vector2f UVPosition;
    };
}

#endif
