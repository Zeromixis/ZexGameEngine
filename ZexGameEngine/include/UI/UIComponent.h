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
		, public Node2D
		, public Subject< const UIEvent * >
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
			for ( auto &node : m_Childs )
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

		I32 m_Width;
		I32 m_Height;

	private:
		U8 m_Alpha;
		bool m_Enabled;

	};
}



#endif // _UI_UICOMPONENT_H_