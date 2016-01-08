#ifndef _PROPERTY_ANIMLAYER_H_
#define _PROPERTY_ANIMLAYER_H_

#include "ZGEDecl.h"

#include "Core/Asset/Property.h"
#include "AnimCurves.h"

namespace ZGE
{
    struct AnimLayer
        : public Property
    {
    public:

        U32 Index;                                    // Layer Index
         
        std::string Name;                             // Layer Name

        std::vector< PAnimCurves > AnimCurvesList;

    };

    typedef std::shared_ptr< AnimLayer > PAnimLayer;
}

#endif //!_PROPERTY_ANIMLAYER_H_