#ifndef _UI_UIRECT_H_
#define _UI_UIRECT_H_


#include "UIObject.h"
#include "Node.h"

namespace ZGE
{
    class UIRect : public UIObject, public Node2D
    {
    public:
		UIRect () {}

		virtual ~UIRect () {}

		I32 & Width ()
		{
			return m_Width;
		}

		const I32 & Width () const
		{
			return m_Width;
		}

		I32 & Height ()
		{
			return m_Height;
		}

		const I32 & Height () const
		{
			return m_Height;
		}

	protected:
		I32 m_Width;
		I32 m_Height;

    private:


    };

}


#endif // _UI_UIRECT_H_