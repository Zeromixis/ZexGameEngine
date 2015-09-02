#ifndef _CORE_FONT_H_
#define _CORE_FONT_H_

#include "External/freetype/include/ft2build.h"
#include FT_FREETYPE_H
#include "Core/Texture.h"
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

	class FontAsset
		: public Asset
	{
	public:
		FontAsset ( const std::string &path, const I32 &charSize )
		{
			WindowWin *windowWin = dynamic_cast< WindowWin * > ( Context::GetInstance ()->GetWindowPtr ().get () );

			Path = ConvertStringToWString ( path );
			CharSize = charSize;

			FT_New_Face ( FONT_LIBRARY.GetFontLibrary (), path.c_str (), 0, &m_FTFace );

			FT_Set_Char_Size
				(
					m_FTFace,
					0,
					13 * 64,
					::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSX ),
					::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSY )
				);
		}

		virtual ~FontAsset ()
		{
			FT_Done_Face ( m_FTFace );
		}

		Texture TextTextureOut ( const std::wstring &text )
		{

		}

		I32 CharSize;

	private:
		FT_Face m_FTFace;
		
	};

	typedef AssetHandle< FontAsset > Font;


}

#endif
