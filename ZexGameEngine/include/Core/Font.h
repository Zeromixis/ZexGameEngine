#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "External/freetype/include/ft2build.h"
#include FT_FREETYPE_H
#include "Property/Texture.h"
#include "Util.h"
#include "App/WindowWin.h"
#include "App/Context.h"

namespace ZGE
{
	class FontLibrary
	{
	public:
		FontLibrary ()
		{
			FT_Init_FreeType ( &m_FTLibrary );
		}

		~FontLibrary ()
		{
			FT_Done_FreeType ( m_FTLibrary );
		}

		FT_Library GetFontLibrary ()
		{
			return m_FTLibrary;
		}

	private:
		FT_Library m_FTLibrary;
	};

	static FontLibrary FONT_LIBRARY;

	class Font
		: public Property
	{
	public:
		Font ( const std::string &path, const I32 &charSize )
		{
			WindowWin *windowWin = dynamic_cast< WindowWin * > ( Context::GetInstance ()->GetWindowPtr ().get () );

			Path = ConvertStringToWString ( path );
			CharSize = charSize;

			FT_New_Face ( FONT_LIBRARY.GetFontLibrary (), path.c_str (), 0, &m_FTFace );

			FT_Set_Char_Size
				(
					m_FTFace,
					0,
					CharSize * 64,
					::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSX ),
					::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSY )
				);
		}

		virtual ~Font ()
		{
			FT_Done_Face ( m_FTFace );
		}

		PTexture TextTextureOut ( const std::wstring &text )
		{
			
		}

		I32 CharSize;
        std::wstring Path;

	private:
		FT_Face m_FTFace;
		
	};


}

