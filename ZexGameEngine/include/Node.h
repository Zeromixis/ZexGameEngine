#ifndef _NODE_H_
#define _NODE_H_

#include "Math/Vector.h"

namespace ZGE
{
    class Node
    {
    public:
        virtual ~Node ();

    protected:
        Node*    m_Parent;
        Vector3f m_Position;
        Vector3f m_Rotation;
        Vector3f m_Scale;
    };
}


#endif