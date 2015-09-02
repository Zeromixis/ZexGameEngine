#ifndef _CORE_ASSETLOADER_H_
#define _CORE_ASSETLOADER_H_

#include "AssetHandle.h"
#include "Asset.h"

namespace ZGE
{
	template< typename T >
	class AssetLoader
		: public std::enable_if< std::is_convertible< T, Asset >::value >
	{
	public:
		typedef T AssetType;

		virtual ~AssetLoader () {}

		template< typename... Args >
		virtual bool Load ( AssetHandle< T > &asset, Args... args ) = nullptr;
	};
}

#endif