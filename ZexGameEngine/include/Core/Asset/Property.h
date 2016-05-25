#pragma once

#include "ZGEDecl.h"

#include "External/boost/core/noncopyable.hpp"

#include "Object.h"

namespace ZGE
{
    template
        <
        typename T
        /*typename U = typename std::enable_if< std::is_convertible< T, Property >::value >::type*/
        >
        using PropertyHandle = std::shared_ptr< T >;

	struct Property
		: public Object
		, private boost::noncopyable
	{
        template < typename THandle >
        static PropertyHandle< typename THandle::element_type > CreateProperty ()
        {
            return std::make_shared< THandle::element_type > ();
        }

		virtual ~Property () 
        {

        }

	protected:
		Property ()
			: Object ()
		{

		}
	};
	


	template< typename TargetClass, typename SourceClass >
	static PropertyHandle< TargetClass > Property_Cast ( PropertyHandle< SourceClass > &handle )
	{
		if ( nullptr != dynamic_cast< TargetClass > ( handle.get () ) )
		{
			return handle;
		}
		else
		{
			return nullptr;
		}
	}
}
