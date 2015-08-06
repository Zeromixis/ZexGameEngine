#include "Test/RenderTextTest.h"

namespace ZGE
{

	const Vector2f RenderTextTest::DrawPosition = Vector2f ( 100.0f, 100.0f );

	RenderTextTest::RenderTextTest ()
	{
		WindowWin *windowWin = dynamic_cast< WindowWin * > ( Context::GetInstance ()->GetWindowPtr ().get () );

		FT_Init_FreeType ( &m_FTLibrary );
		FT_New_Face ( m_FTLibrary, "Font/calibri.ttf", 0, &m_FTFace );
		FT_Set_Char_Size
			(
			m_FTFace,
			0,
			13 * 64,
			::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSX ),
			::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSY )
			);

		FT_Vector pen;
		pen.x = DrawPosition.x ();
		pen.y = DrawPosition.y ();

		FT_Set_Transform ( m_FTFace, 0, &pen );

		FT_Load_Char ( m_FTFace, 'Z', FT_LOAD_RENDER );
		auto slot = m_FTFace->glyph;
		FT_Bitmap bitmap = slot->bitmap;

		glGenVertexArrays ( 1, &m_VertexArray );
		glBindVertexArray ( m_VertexArray );

		// Init Vertex Data

		// Position Data

		Vector2f leftTop = Vector2f ( slot->bitmap_left, slot->bitmap_top );
		Vector2f rightTop = Vector2f ( leftTop.x () + slot->metrics.width, leftTop.y () );
		Vector2f leftBottom = Vector2f ( leftTop.x (), leftTop.y () - slot->metrics.height );
		Vector2f rightBottom = Vector2f ( leftBottom.x () + slot->metrics.width, leftBottom.y () );

		std::vector < Vector2f > vertexs;
		vertexs.push_back ( leftTop );
		vertexs.push_back ( rightTop );
		vertexs.push_back ( leftBottom );
		vertexs.push_back ( rightBottom );

		m_VertexBuffer = new ArrayBuffer ( vertexs.size () * 2 * 4, ArrayBuffer::ArrayBufferUsage::STATIC );
		m_VertexBuffer->SetData ( &vertexs[ 0 ], vertexs.size () * 2 * 4 );
		m_VertexBuffer->TransferData ();

		// Texcoord Data

		std::vector < Vector2f > texcoords;
		texcoords.push_back ( Vector2f ( 0.0f, 1.0f ) );
		texcoords.push_back ( Vector2f ( 1.0f, 1.0f ) );
		texcoords.push_back ( Vector2f ( 0.0f, 0.0f ) );
		texcoords.push_back ( Vector2f ( 1.0f, 0.0f ) );

		m_TexcoordBuffer = new ArrayBuffer ( texcoords.size () * 2 * 4, ArrayBuffer::ArrayBufferUsage::STATIC );
		m_TexcoordBuffer->SetData ( &texcoords[ 0 ], texcoords.size () * 2 * 4 );
		m_TexcoordBuffer->TransferData ();


		// Texture Data

		U32 *texData = new U32[ bitmap.width * bitmap.rows ];
		for ( size_t i = 0; i < bitmap.width * bitmap.rows; ++i )
		{
			if ( bitmap.buffer[ i ] >= 128 )
			{
				texData[ i ] = 255 | 0 << 8 | 0 << 16 | 255 << 24;
			}
			else
			{
				texData[ i ] = 0;
			}
		}

		glGenTextures ( 1, &m_CharTexture );
		glTexImage2D
			(
			GL_TEXTURE_2D,                  // target
			0,                              // level
			GL_RGBA,                        // internal format
			bitmap.width,                   // width
			bitmap.rows,                    // height
			0,                              // must be zero
			GL_RGBA,                        // format
			GL_UNSIGNED_BYTE,               // data type
			texData                         // source
			);

		delete[] texData;

		// Vertex and Texcoord Binding

		glBindBuffer ( GL_ARRAY_BUFFER, m_VertexBuffer->Handle () );
		glEnableVertexAttribArray ( 0 );
		glVertexAttribPointer
			(
			0,                      // index
			2,                      // size
			GL_FLOAT,               // type
			GL_FALSE,               // isNormalized
			0,                      // stride
			0                       // pointer
			);

		glBindBuffer ( GL_ARRAY_BUFFER, m_TexcoordBuffer->Handle () );
		glEnableVertexAttribArray ( 1 );
		glVertexAttribPointer
			(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
			);

		// Index Data 
		std::vector< GLuint > indexList = { 0, 1, 2, 1, 3, 2 };

		glGenBuffers ( 1, &m_IndexBufferHandle );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferHandle );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indexList.size (), &indexList[ 0 ], GL_STATIC_DRAW );


		// Init Shader
		std::wstring vertexShaderPath = L"Shader/TextVS.vp";
		std::wstring vertexShaderSource = LoadShaderSourceFromFile ( vertexShaderPath );
		m_Shader.AttachShader ( ShaderObject::ShaderType::Vertex, ConvertWStringToString ( vertexShaderSource ) );

		std::wstring fragmentShaderPath = L"Shader/TextFS.fp";
		std::wstring fragmentShaderSource = LoadShaderSourceFromFile ( fragmentShaderPath );
		m_Shader.AttachShader ( ShaderObject::ShaderType::Fragment, ConvertWStringToString ( fragmentShaderSource ) );

		m_Shader.LinkProgram ();

		auto orthoMatrixLoc = glGetUniformLocation ( m_Shader.GLSLProgram (), "orthoMatrix" );
		auto window = Context::GetInstance ()->GetWindowPtr ();
		auto orthoMatrix = OrthoLH< F32 > ( window->Width (), window->Height (), 0.0f, 1.0f );
		Transpose ( orthoMatrix );
		glUniformMatrix4fv
			(
			orthoMatrixLoc,
			1,
			GL_FALSE,
			static_cast< GLfloat * >( &orthoMatrix[ 0 ] )
			);

		GLint textureUniform = glGetUniformLocation ( m_Shader.GLSLProgram (), "tex2D" );
		glUniform1i ( textureUniform, 0 );

	}

}