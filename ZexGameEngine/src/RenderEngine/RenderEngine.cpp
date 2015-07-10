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

        glProvokingVertex ( GL_FIRST_VERTEX_CONVENTION );

        /*
        glMatrixLoadIdentityEXT ( GL_PROJECTION );
        glMatrixScalefEXT ( GL_PROJECTION, 1, -1, 0 );          // y方向取反
        glMatrixTranslatefEXT ( GL_PROJECTION, 0.5f / window->Width (), 0.5f / window->Height (), 0 ); // 调整到D3D9的话还需要偏移0.5个像素
        */

        // The Triangle is clock-wise ( like DirectX )
        glFrontFace ( GL_CW );
        
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

		// glEnable ( GL_TEXTURE_1D );
		// lightningTest.Draw ();

        Context::GetInstance ()->GetWindowPtr ()->SwapBuffer ();
    }

    void RenderEngine::FullScreen ( bool isFullScreen )
    {
        
    }

    void RenderEngine::SetCamera ( const Camera &camera )
    {
        m_Camera = camera;
    }

    void RenderEngine::RenderChar ( U32 charCode, Vector2f position )
    {

        
    }

}