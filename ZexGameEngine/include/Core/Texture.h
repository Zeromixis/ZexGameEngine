#ifndef _CORE_TEXTURE_H_
#define _CORE_TEXTURE_H_

#include "Core/Asset/Asset.h"
#include "Core/Asset/AssetHandle.h"


namespace ZGE
{
	class TextureAsset
		: public Asset
	{
	public:
		enum Format
		{
			L8,
			R8G8B8A8,
		};

		TextureAsset ()
			: TexData ( nullptr )
			, Width ( 0 )
			, Height ( 0 )
			, TexFormat ( R8G8B8A8 )
		{

		}

		virtual ~TextureAsset ()
		{
			delete[] TexData;
		}

		Format TexFormat;

		U32 Width;

		U32 Height;

		U8 *TexData;

	};

	typedef AssetHandle< TextureAsset > Texture;
}


#endif