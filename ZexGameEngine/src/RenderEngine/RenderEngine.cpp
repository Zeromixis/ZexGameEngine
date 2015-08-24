#include "Render/RenderEngine.h"
#include "App/WindowWin.h"
#include <App/Context.h>
#include "Util.h"
#include <functional>
#include "Test/LightningTest.h"


namespace ZGE
{
    RenderEngine::RenderEngine ()
	{
        Camera *myCamera = new Camera ();
        auto window = Context::GetInstance ()->GetWindowPtr ();
        F32 cameraAspect = ( F32 )( window->Width () ) / window->Height ();
        myCamera->SetProj ( PI / 4, cameraAspect, 0.1f, 1000.0f );
        myCamera->SetView ( Vector3f ( 0.0f, 0.0f, -8.0f ), Vector3f ( 0.0f, 0.0f, 0.0f ) );

        m_CameraController = std::make_shared< CameraController > ( myCamera );

        glProvokingVertex ( GL_FIRST_VERTEX_CONVENTION );

        /*
        glMatrixLoadIdentityEXT ( GL_PROJECTION );
        glMatrixScalefEXT ( GL_PROJECTION, 1, -1, 0 );          // y方向取反
        glMatrixTranslatefEXT ( GL_PROJECTION, 0.5f / window->Width (), 0.5f / window->Height (), 0 ); // 调整到D3D9的话还需要偏移0.5个像素
        */

        // The Triangle is clock-wise ( like DirectX )
        glFrontFace ( GL_CW );

		renderTextTest = new RenderTextTest;

		// InitFontRender ();
    }

    RenderEngine::~RenderEngine ()
    {

    }

    void RenderEngine::Refresh ()
    {
		glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


//         m_Shader->Bind ();
// 		
//         GLuint mvpMatrixLoc = glGetUniformLocation ( m_Shader->GLSLProgram (), "mvpMatrix" );
//         auto modelMatrix = Float44::Identity ();
//         auto viewMatrix = m_CameraController->GetCamera()->ViewMatrix();
//         auto projMatrix = m_CameraController->GetCamera()->ProjMatrix();
//         auto mvpMatrix = modelMatrix * viewMatrix * projMatrix;
//         glUniformMatrix4fv ( mvpMatrixLoc, 1, GL_FALSE, static_cast< GLfloat * >( &mvpMatrix[ 0 ] ) );
// 		GLint textureUniform = glGetUniformLocation ( m_Shader->GLSLProgram (), "tex1D" );
// 		glUniform1i ( textureUniform, 0 );



		// glEnable ( GL_TEXTURE_1D );
		// lightningTest.Draw ();

		renderTextTest->OnPreDraw ();
		renderTextTest->OnDraw ();
		renderTextTest->OnPostDraw ();

        Context::GetInstance ()->GetWindowPtr ()->SwapBuffer ();
    }

    void RenderEngine::FullScreen ( bool isFullScreen )
    {
        
    }

    void RenderEngine::SetCamera ( const Camera &camera )
    {
        m_Camera = camera;
    }

	void RenderEngine::RenderText ( const std::wstring & text, const Vector2i & position, const Vector3f & color, U32 size )
	{
		WindowWin *windowWin = dynamic_cast< WindowWin * > ( Context::GetInstance ()->GetWindowPtr ().get () );

		FT_Set_Char_Size
		(
			m_FTFace,
			0,
			size * 64,
			::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSX ),
			::GetDeviceCaps ( windowWin->Hdc (), LOGPIXELSY )
		);

		Vector2i drawPosition = position;


		for ( auto &iChar : text )
		{
			FT_Vector pen;
			pen.x = drawPosition.x () * 64;
			pen.y = drawPosition.y () * 64;
			FT_Set_Transform ( m_FTFace, 0, &pen );

			FT_Load_Char ( m_FTFace, iChar, FT_LOAD_RENDER );

			auto slot = m_FTFace->glyph;
			FT_Bitmap bitmap = slot->bitmap;



			// Step Add DrawPosition.
		}


	}

	void RenderEngine::InitFontRender ()
	{
		FT_Init_FreeType ( &m_FTLibrary );
		FT_New_Face ( m_FTLibrary, "Font/msyh.ttf", 0, &m_FTFace );
	}

}