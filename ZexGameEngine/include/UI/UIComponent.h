#ifndef _UI_UICOMPONENT_H_
#define _UI_UICOMPONENT_H_

#include "UIObject.h"
#include "Node.h"
#include "UIEvent.h"
#include "Pattern/Observer.h"

namespace ZGE
{
    class UIComponent
        : public UIObject
        , public Subject < const UIEvent * >
    {
    public:
        UIComponent ()
            : m_Width ( 0 )
            , m_Height ( 0 )
            , m_Alpha ( 255 )
            , m_Enabled ( true )
        {

        }

        virtual ~UIComponent ();

        Vector2f & Position ()
        {
            return m_Node.Position ();
        }

        const Vector2f & Position () const
        {
            return m_Node.Position ();
        }

        Vector2f LocalPosToParent ( const Vector2f &localPos ) const
        {
            if ( nullptr != m_Node.Parent () )
            {
                return Vector2f ( localPos + m_Node.Position () );
            }
            else
            {
                return Vector2f ( localPos );
            }
        }

		U32 & Width ()
		{
			return m_Width;
		}

		const U32 & Width () const
		{
			return m_Width;
		}

		U32 & Height ()
		{
			return m_Height;
		}

		const U32 & Height () const
		{
			return m_Height;
		}

		U8 & Alpha ()
		{
			return m_Alpha;
		}

		const U8& Alpha () const
		{
			return m_Alpha;
		}

		bool & Enabled ()
		{
			return m_Enabled;
		}

		const bool & Enabled () const
		{
			return m_Enabled;
		}

		UIComponent * ComponentAt ( Vector2f localPos ) const
		{
			for ( auto &node : m_Node.Childs () )
			{
				UIComponent *uiComponent = dynamic_cast< UIComponent * > ( node );
				if ( uiComponent )
				{
					if (
						localPos.x () >= uiComponent->Position ().x () &&
						localPos.x () <= uiComponent->Position ().x () + uiComponent->Width () &&
						localPos.y () >= uiComponent->Position ().y () &&
						localPos.y () <= uiComponent->Position ().y () + uiComponent->Height ()
					   )
					{
						return uiComponent;
					}
				}
			}
			return nullptr;
		}
		
		void OnUIEvent ( const UIEvent *event )
		{
			OnPreUIEvent ( event );
			OnProcessUIEvent ( event );
			OnPostUIEvent ( event );
		}

	protected:
		virtual void OnPreUIEvent ( const UIEvent *event ) {}
		virtual void OnProcessUIEvent ( const UIEvent *event ) {}
		virtual void OnPostUIEvent ( const UIEvent *event ) {}

		U32 m_Width;
		U32 m_Height;

        Node2D m_Node;

	private:
		U8 m_Alpha;
		bool m_Enabled;

	};
}

#endif // _UI_UICOMPONENT_H_