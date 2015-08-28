#ifndef _CORE_ASSETLOADER_H_
#define _CORE_ASSETLOADER_H_

#include "AssetHandle.h"

namespace ZGE
{
	template< typename T >
	class AssetLoader
	{
	public:
		typedef T AssetType;

		virtual ~AssetLoader () {}

		template< typename... Args >
		bool Load ( AssetHandle< T > &asset, Args... args ) = nullptr;
	};
}

#endif