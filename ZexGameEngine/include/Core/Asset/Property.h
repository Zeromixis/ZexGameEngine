#ifndef _CORE_ASSET_PROPERTY_H_
#define _CORE_ASSET_PROPERTY_H_

#include "ZGEDecl.h"

#include "External/boost/core/noncopyable.hpp"

namespace ZGE
{
	struct Property
		: public Object
		, private boost::noncopyable
	{
		virtual ~Property () 
        {

        }

	protected:
		Property ()
			: Object ()
		{

		}
	};
	
	template
    <
        typename T,
        typename = typename std::enable_if< std::is_convertible< T, Property >::value >::type
    >
	using PropertyHandle = std::shared_ptr< T >;

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

#endif
