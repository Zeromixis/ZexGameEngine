#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "Core/Asset/Property.h"


namespace ZGE
{
	struct Texture
		: public Property
	{
		enum Format
		{
			L8,
			R8G8B8A8,
		};

		Texture ()
			: TexData ( nullptr )
			, Width ( 0 )
			, Height ( 0 )
			, TexFormat ( R8G8B8A8 )
		{

		}

		virtual ~Texture ()
		{
			delete[] TexData;
		}

		Format TexFormat;

		U32 Width;

		U32 Height;

		U8 *TexData;
	};

	typedef PropertyHandle< Texture > PTexture;
}
