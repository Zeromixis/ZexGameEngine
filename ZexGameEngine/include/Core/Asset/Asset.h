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
		enum AssetState
		{
			IS_LOAD = 0,
			ON_DISK,
			ON_RAM,
			
			OTHER_STATUS_BEGIN,
		};

		Asset ()
			: TagObject ()
			, m_Size { 0 }
		{
			
		}

		virtual ~Asset () {}

		const std::wstring & Name () const
		{
			return m_Name;
		}

		const size_t & Size () const
		{
			return m_Size;
		}

		const std::wstring & Path () const
		{
			return m_Path;
		}

		bool CheckStatus ( U16 statePos ) const
		{
			return m_Flags[ statePos ];
		}

	protected:
		std::wstring m_Name;

		size_t m_Size;

		std::wstring m_Path;

		std::bitset< 32 > m_Flags;
	};
}


#endif