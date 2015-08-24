#ifndef _CORE_ASSET_ASSETMANAGER_H_
#define _CORE_ASSET_ASSETMANAGER_H_

#include "Pattern/Singleton.h"

namespace ZGE
{
	class AssetManager
		: public Singleton< AssetManager >
	{
		friend class Singleton< AssetManager >;
	};
}


#endif
