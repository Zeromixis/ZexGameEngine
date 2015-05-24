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
		lightningTest.GenerateRandomPoints ();
//         glGenVertexArrays ( 1, &m_VertexArray );
//         glBindVertexArray ( m_VertexArray );
// 
// 
//         // Bind Position Data
// //         F32 positionData[] =
// //         {
// //             -0.8f, -0.8f, 0.5f,
// //             0.8f, -0.8f, 0.5f,
// //             0.0f, 0.8f, 0.5f,
// //         };
// 
// 		F32 *positionData = new F32[ lightningTest.m_Points.size () * 3 ];
// 		for ( size_t i = 0; i < lightningTest.m_Points.size (); ++i )
// 		{
// 			positionData[ i * 3 ] = lightningTest.m_Points[ i ].x ();
// 			positionData[ i * 3 + 1 ] = lightningTest.m_Points[ i ].y ();
// 			positionData[ i * 3 + 2 ] = lightningTest.m_Points[ i ].z ();
// 		}
// 
//         glGenBuffers ( 1, &m_VertexBuffer );
//         glBindBuffer ( GL_ARRAY_BUFFER, m_VertexBuffer );
// 		glBufferData ( GL_ARRAY_BUFFER, lightningTest.m_Points.size () * 3 * 4, positionData, GL_STATIC_DRAW );
// 
// //         // Bind Color Data
// //         F32 colorData[] =
// //         {
// //             1.0f, 0.0f, 0.0f,
// //             0.0f, 1.0f, 0.0f, 
// //             0.0f, 0.0f, 1.0f,
// //         };
// // 
// //         glGenBuffers ( 1, &m_ColorBuffer );
// //         glBindBuffer ( GL_ARRAY_BUFFER, m_ColorBuffer );
// //         glBufferData ( GL_ARRAY_BUFFER, sizeof( colorData ), colorData, GL_STATIC_DRAW );
// 
// 
// 		// Bind Tex Data
// 		F32 *texData = new F32[ lightningTest.m_Points.size () * 2 ];
// 		for ( size_t i = 0; i < lightningTest.m_Points.size (); ++i )
// 		{
// 			//positionData[ i ] = lightningTest.m_Points[ i ].x ();
// 			if ( i % 2 == 0 )
// 			{
// 				texData[ i * 2 ] = 0.0f;
// 			}
// 			else
// 			{
// 				texData[ i * 2] = 1.0f;
// 			}
// 			texData[ i * 2 + 1 ] = 0.0f;
// 		}
// 
// 		glGenBuffers ( 1, &m_TexCoordBuffer );
// 		glBindBuffer ( GL_ARRAY_BUFFER, m_TexCoordBuffer );
// 		glBufferData ( GL_ARRAY_BUFFER, lightningTest.m_Points.size () * 2 * 4, texData, GL_STATIC_DRAW );
// 
//         glEnableVertexAttribArray ( 0 );
//         glBindBuffer ( GL_ARRAY_BUFFER, m_VertexBuffer );
//         glVertexAttribPointer 
//         (
//             0,
//             3,
//             GL_FLOAT,
//             GL_FALSE,
//             0,
//             0
//         );
// 
// 		glEnableVertexAttribArray ( 1 );
// 		glBindBuffer ( GL_ARRAY_BUFFER, m_TexCoordBuffer );
// 		glVertexAttribPointer
// 		(
// 			1,
// 			2,
// 			GL_FLOAT,
// 			GL_FALSE,
// 			0,
// 			0
// 		);

//         glEnableVertexAttribArray ( 1 );
//         glBindBuffer ( GL_ARRAY_BUFFER, m_ColorBuffer );
//         glVertexAttribPointer
//         (
//             1,
//             3,
//             GL_FLOAT,
//             GL_FALSE,
//             0,
//             0
//         );


        //Init Shader
        m_Shader = std::shared_ptr< ShaderObject > ( new ShaderObject ( ) );
        
        std::wstring vertexShaderPath = L"vertex.vp";
        std::wstring vertexShaderSource = LoadShaderSourceFromFile ( vertexShaderPath );        
        m_Shader->AttachShader ( ShaderObject::ShaderType::Vertex, ConvertWStringToString ( vertexShaderSource ) );

        std::wstring fragmentShaderPath = L"frag.fp";
        std::wstring fragmentShaderSource = LoadShaderSourceFromFile ( fragmentShaderPath );
        m_Shader->AttachShader ( ShaderObject::ShaderType::Fragment, ConvertWStringToString ( fragmentShaderSource ) );

        m_Shader->LinkProgram ();

        Camera *myCamera = new Camera ();
        auto window = Context::GetInstance ()->GetWindowPtr ();
        F32 cameraAspect = ( F32 )( window->Width () ) / window->Height ();
        myCamera->SetProj ( PI / 4, cameraAspect, 0.1f, 1000.0f );
        myCamera->SetView ( Vector3f ( 0.0f, 0.0f, -8.0f ), Vector3f ( 0.0f, 0.0f, 0.0f ) );

        m_CameraController = std::make_shared< CameraController > ( myCamera );
    }

    RenderEngine::~RenderEngine ()
    {

    }

    void RenderEngine::Refresh ()
    {
        glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        m_Shader->Bind ();
		
        GLuint mvpMatrixLoc = glGetUniformLocation ( m_Shader->GLSLProgram (), "mvpMatrix" );
        auto modelMatrix = Float44::Identity ();
        auto viewMatrix = m_CameraController->GetCamera()->ViewMatrix();
        auto projMatrix = m_CameraController->GetCamera()->ProjMatrix();
        auto mvpMatrix = modelMatrix * viewMatrix * projMatrix;
        glUniformMatrix4fv ( mvpMatrixLoc, 1, GL_FALSE, static_cast< GLfloat * >( &mvpMatrix[ 0 ] ) );
		GLint textureUniform = glGetUniformLocation ( m_Shader->GLSLProgram (), "tex1D" );
		glUniform1i ( textureUniform, 0 );

		//glEnable ( GL_TEXTURE_1D );
		lightningTest.Draw ();

        Context::GetInstance ()->GetWindowPtr ()->SwapBuffer ();
    }

    void RenderEngine::FullScreen ( bool isFullScreen )
    {
        
    }

    void RenderEngine::SetCamera ( const Camera &camera )
    {
        m_Camera = camera;
    }

}