#ifndef _PROPERTY_ANIMSTACK_H_
#define _PROPERTY_ANIMSTACK_H_

#include "ZGEDecl.h"

#include "Core/Asset/Property.h"
#include "AnimLayer.h"

namespace ZGE
{
    struct AnimStack
        : public Property
    {
    public:
        
        std::string Name;                            // The Animation Name

        std::vector< PAnimLayer > AnimLayerList;
    };

    typedef std::shared_ptr< AnimStack > PAnimStack;
}

#endif // !_PROPERTY_ANIMSTACK_H_

