#ifndef _CORE_ASSET_PROPERTY_H_
#define _CORE_ASSET_PROPERTY_H_

#include <memory>
#include "Pattern/TagObject.h"
#include "External/boost/core/noncopyable.hpp"

namespace ZGE
{
	struct Property
		: public TagObject
		, private boost::noncopyable
	{
		virtual ~Property () {}

	protected:
		Property ()
			: TagObject ()
		{

		}
	};
	
	template
		<
		typename T,
		class = std::enable_if < std::is_convertible< T, Property >, void >::type
		>
	using PropertyHandle = std::shared_ptr< T >;

	template< typename TargetClass, typename SourceClass >
	static PropertyHandle< TargetClass > property_cast ( PropertyHandle< SourceClass > &handle )
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
