#ifndef _CORE_ASSET_ENTITY_H_
#define _CORE_ASSET_ENTITY_H_

#include <set>
#include "Property.h"

namespace ZGE
{
	class Entity
	{
	public:
		Entity ();

		virtual ~Entity () {}

		template < typename T >
		void AddProperty ( PropertyHandle< T > &propertyHandle )
		{
			m_ProprotyHandleList.insert ( propertyHandle );
		}

		template < typename T >
		void RemoveProperty ()
		{
			for ( auto e : m_ProprotyHandleList )
			{
				if ( nullptr != property_cast< T > ( e ) )
				{
					m_ProprotyHandleList.erase ( e );
					break;
				}
			}
		}

		template < typename T >
		PropertyHandle< T > ReadProperty ()
		{
			for ( auto e : m_ProprotyHandleList )
			{
				if ( nullptr != property_cast< T > ( e ) )
				{
					return e;
				}
			}
			return nullptr;
		}

	private:
		std::set< PropertyHandle< Property > > m_ProprotyHandleList;
	};
}

#endif