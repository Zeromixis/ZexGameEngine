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
            glBindTexture ( GL_TEXTURE_2D, m_CharTexture );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glDisable ( GL_CULL_FACE );
			glBindBuffer ( GL_ARRAY_BUFFER, m_VertexBuffer->Handle () );
			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferHandle ); 
        }

        void OnDraw ()
        {
			m_Shader.Bind ();
			glDrawElements
			(
				GL_TRIANGLES,
				6,
				GL_UNSIGNED_INT,
				nullptr
			);
		}
        
        void OnPostDraw ()
        {
			m_Shader.UnBind ();
			glEnable ( GL_CULL_FACE );
            glBindBuffer ( GL_ARRAY_BUFFER, 0 );
            glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
        }

        // Text render test
        FT_Library m_FTLibrary;
        FT_Face    m_FTFace;

        GLuint m_CharTexture;
        GLuint m_VertexArray;

        BufferGL< GL_ARRAY_BUFFER > *m_VertexBuffer;
        BufferGL< GL_ARRAY_BUFFER > *m_TexcoordBuffer;

		ShaderObject m_Shader;

		GLuint m_IndexBufferHandle;
		
    };
}


#endif