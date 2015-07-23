#ifndef _RENDER_RENDERENGINE_H_
#define _RENDER_RENDERENGINE_H_

#include <memory>
#include "CorePrerequisites.h"
#include "Render/ShaderObject.h"
#include "Core/Camera.h"
#include "Core/CameraController.h"
#include "Test/LightningTest.h"
#include "Test/RenderTextTest.h"

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

		const std::shared_ptr< ShaderObject > GetShader () const
		{
			return m_Shader;
		}

        void RenderChar ( U32 charCode, Vector2f position );

    protected:
        std::shared_ptr< CameraController > m_CameraController;
        Camera m_Camera;

    private:
        std::shared_ptr < ShaderObject > m_Shader;

        GLuint m_VertexArray;
        GLuint m_VertexBuffer;
		GLuint m_TexCoordBuffer;
        GLuint m_ColorBuffer;

		LightningTest lightningTest;
		RenderTextTest *renderTextTest;


    };
}




#endif