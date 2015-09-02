#ifndef _CORE_ASSET_H_
#define _CORE_ASSET_H_

#include <bitset>
#include "CorePrerequisites.h"
#include "Pattern/TagObject.h"


namespace ZGE
{
	class Asset
		: public TagObject
	{
	public:
		enum State
		{
			IS_LOAD = 0,
			ON_DISK,
			ON_RAM,
			
			DERIVED_STATE_BEGIN,
		};

		Asset ()
			: TagObject ()
			, Size { 0 }
		{
			
		}

		virtual ~Asset () {}

		std::wstring Name;

		U64 Size;

		std::wstring Path;

		std::bitset< 32 > Flags;
	};
}


#endif