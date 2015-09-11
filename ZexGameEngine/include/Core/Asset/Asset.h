#ifndef _CORE_ASSET_H_
#define _CORE_ASSET_H_

#include <bitset>
#include "CorePrerequisites.h"
#include "Pattern/TagObject.h"
#include "External/boost/core/noncopyable.hpp"


namespace ZGE
{
	class Asset
		: public TagObject
		, private boost::noncopyable
	{
	public:
		Asset ()
			: TagObject ()
		{

		}

		virtual ~Asset () {}
	};
}


#endif