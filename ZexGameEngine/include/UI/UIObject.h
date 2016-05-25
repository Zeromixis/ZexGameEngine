#pragma once

#include "ZGEDecl.h"

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

    static bool operator == ( const UIObject &lhs, const UIObject &rhs )
    {
        return lhs.Uuid () == rhs.Uuid ();
    }

	std::map< boost::uuids::uuid, UIObject * > UIObject::ObjectMap;
}
