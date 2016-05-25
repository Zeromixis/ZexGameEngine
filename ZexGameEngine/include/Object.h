#pragma once

#include "ZGEDecl.h"

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/operators.hpp"

namespace ZGE
{
	class Object
		: public boost::equality_comparable< Object >
	{
		
	public:
		Object ()
			: m_Uuid ( boost::uuids::random_generator () ( ) )
		{

		}

		Object ( const Object &rhs )
			: m_Uuid ( rhs.m_Uuid )
		{

		}

        Object ( Object &&rhs )
            : m_Uuid ( rhs.m_Uuid )
        {
            
        }

		virtual ~Object ()
		{

		}

		bool EqualTo ( const Object &rhs ) const
		{
			return m_Uuid == rhs.m_Uuid;
		}

		bool operator == ( const Object &rhs ) const
		{
			return this->EqualTo ( rhs );
		}

        void operator = ( const Object &rhs )
		{
			m_Uuid = rhs.m_Uuid;
		}

        // Default return this address value
        virtual std::string ToString () const
        {
            void *thisPointer = ( void * )this;
            char buffer[ 42 ];
            sprintf_s ( buffer, "%p", thisPointer );
            return std::string ( buffer );
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
