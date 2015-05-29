#ifndef _UI_UIOBJECT_H_
#define _UI_UIOBJECT_H_

#include <map>
#include "External/boost/uuid/uuid.hpp"
#include "External/boost/uuid/random_generator.hpp"


namespace ZGE
{
	class UIObject
	{
	public:
		UIObject ()
			: m_Uuid ( boost::uuids::random_generator () () )
		{
			ObjectMap[ m_Uuid ] = this;
		}

		virtual ~UIObject ()
		{
			ObjectMap.erase ( m_Uuid );
		}

		const boost::uuids::uuid & Uuid () const
		{
			return m_Uuid;
		}

		static bool operator == ( const UIObject &lhs, const UIObject &rhs )
		{
			return lhs.Uuid () == rhs.Uuid ();
		}

		static UIObject * FindUIObjectByUUID ( const boost::uuids::uuid &uuid )
		{
			UIObject *object = nullptr;
			do 
			{
				auto iter = ObjectMap.find ( uuid );
				if ( iter != ObjectMap.end () )
				{
					object = iter->second;
				}
			} while ( 0 );
			return object;
		}
	private:
		boost::uuids::uuid m_Uuid;

		static std::map< boost::uuids::uuid, UIObject * > ObjectMap;
	};

	std::map< boost::uuids::uuid, UIObject * > UIObject::ObjectMap;
}

#endif // _UI_UIOBJECT_H_