#ifndef _TEST_RENDERTEXTTEST_H_
#define _TEST_RENDERTEXTTEST_H_

#include "External/freetype/include/ft2build.h"
#include FT_FREETYPE_H
#include "App/Context.h"
#include "Util.h"

namespace ZGE
{
    struct RenderTextTest
    {
        static const Vector2f DrawPosition;

		RenderTextTest ();

        ~RenderTextTest ()
        {

        }

        void OnPreDraw ()
        {
			glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glBindTexture ( GL_TEXTURE_2D, m_CharTexture );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glDisable ( GL_CULL_FACE );

			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferHandle ); 
        }

        void OnDraw ()
        {
			m_Shader.Bind ();
			glDrawElements
			(
				GL_TRIANGLES,
				2,
				GL_UNSIGNED_INT,
				nullptr
			);
		}
        
        void OnPostDraw ()
        {
			glEnable ( GL_CULL_FACE );
        }

        // Text render test
        FT_Library m_FTLibrary;
        FT_Face    m_FTFace;

        GLuint m_CharTexture;
        GLuint m_VertexArray;

        ArrayBuffer *m_VertexBuffer;
        ArrayBuffer *m_TexcoordBuffer;

		ShaderObject m_Shader;

		GLuint m_IndexBufferHandle;
		
    };
}


#endif