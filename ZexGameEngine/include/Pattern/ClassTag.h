#ifndef _PATTERN_CLASSTAG_H_
#define _PATTERN_CLASSTAG_H_

#include "Pattern/TagObject.h"

namespace ZGE
{
	template < typename T >
	class ClassTag
	{
	public:
		static TagObject Tag;
	};
}

#endif
