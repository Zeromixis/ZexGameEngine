#ifndef _PATTERN_UUIDOBJECT_H_
#define _PATTERN_UUIDOBJECT_H_

#include <sstream>
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/operators.hpp"

namespace ZGE
{
	class TagObject
		: public boost::equality_comparable< TagObject >
	{
		
	public:
		TagObject ()
			: m_Uuid ( boost::uuids::random_generator () ( ) )
		{

		}

		TagObject ( const TagObject &rhs )
			: m_Uuid ( rhs.m_Uuid )
		{

		}

		virtual ~TagObject ()
		{

		}

		bool EqualTo ( const TagObject &rhs ) const
		{
			return m_Uuid == rhs.m_Uuid;
		}

		bool operator == ( const TagObject &rhs ) const
		{
			return this->EqualTo ( rhs );
		}

		bool operator = ( const TagObject &rhs )
		{
			m_Uuid = rhs.m_Uuid;
		}

		std::string UUIDString () const
		{
			std::stringstream strStream;
			strStream << m_Uuid;
			return strStream.str ();
		}
	private:
		boost::uuids::uuid m_Uuid;
	};
}

#endif
