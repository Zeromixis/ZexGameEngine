#ifndef _UI_UIRECT_H_
#define _UI_UIRECT_H_


#include "UIObject.h"
#include "Node.h"

namespace ZGE
{
    class UIRect : public UIObject, public Node2D
    {
    public:

    private:
        virtual Node2D::ElementType & Rotation () override;
        virtual Node2D::ElementType & Scale () override;

    };

}


#endif // _UI_UIRECT_H_