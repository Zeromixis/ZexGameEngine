#ifndef _CORE_ASSET_H_
#define _CORE_ASSET_H_

#include <bitset>
#include "CorePrerequisites.h"
#include "Pattern/Object.h"
#include "External/boost/core/noncopyable.hpp"


namespace ZGE
{
	class Asset
		: public Object
		, private boost::noncopyable
	{
	public:
		Asset ()
			: Object ()
		{

		}

		virtual ~Asset () {}
	};
}


#endif