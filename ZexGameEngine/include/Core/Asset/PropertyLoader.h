#ifndef _CORE_ASSET_PROPERTYLOADER_H_
#define _CORE_ASSET_PROPERTYLOADER_H_

#include "External/boost/concept_check.hpp"
#include "Property.h"

namespace ZGE
{
	struct PropertyLoader
	{
		virtual ~PropertyLoader () {}

	protected:
		PropertyLoader () 
		{

		}
	};
}

#endif