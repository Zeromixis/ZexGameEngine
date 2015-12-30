#ifndef _CORE_PROPERTY_MESH_H_
#define _CORE_PROPERTY_MESH_H_

#include <vector>
#include "Core/Asset/Property.h"
#include "Property/Vertex.h"

namespace ZGE
{
    class Mesh
        : public Property
    {
    public:
        Mesh ()
        {

        }

        std::vector< Vertex >& GetVerticesRef ()
        {
            return m_Vertices;
        }

        std::vector< U32 >& GetVertexIndicesRef ()
        {
            return m_VectexIndices;
        }
            
    protected:
        std::vector< Vertex > m_Vertices;

        std::vector< U32 > m_VectexIndices;
    };
}


#endif
