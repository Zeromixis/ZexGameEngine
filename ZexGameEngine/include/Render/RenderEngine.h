#ifndef _RENDER_RENDERENGINE_H_
#define _RENDER_RENDERENGINE_H_

#include "ZGEDecl.h"

#include "Render/ShaderObject.h"
#include "Core/Camera.h"
#include "Core/CameraController.h"
#include "Test/LightningTest.h"
#include "Test/RenderTextTest.h"
#include "Test/FBXTest.h"


namespace ZGE
{
	struct RenderTextTest;

	// Test OpenGL Render temporarily

    class RenderEngine
    {
    public:
        RenderEngine ();

        ~RenderEngine ();
        
        virtual void FullScreen ( bool isFullScreen );

        virtual void Refresh ();

        void SetCamera ( const Camera &camera );

		void RenderText ( const std::wstring & text, const Vector2i & position, const Vector3f & color, U32 size );

		const std::shared_ptr< ShaderObject > GetShader () const
		{
			return m_Shader;
		}

        void RenderChar ( U32 charCode, Vector2f position );

    protected:
        std::shared_ptr< CameraController > m_CameraController;
        Camera m_Camera;

    private:
		void InitFontRender ();


        std::shared_ptr < ShaderObject > m_Shader;

        GLuint m_VertexArray;
        GLuint m_VertexBuffer;
		GLuint m_TexCoordBuffer;
        GLuint m_ColorBuffer;

		LightningTest lightningTest;
		RenderTextTest *renderTextTest;
        FBXTest *m_FbxTest;

		FT_Library m_FTLibrary;
		FT_Face    m_FTFace;


    };
}

#endif