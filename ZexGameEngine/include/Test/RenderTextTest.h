#ifndef _TEST_RENDERTEXTTEST_H_
#define _TEST_RENDERTEXTTEST_H_

#include "External/freetype/include/ft2build.h"
#include "App/Context.h"
#include FT_FREETYPE_H

namespace ZGE
{
    struct RenderTextTest
    {
        static const Vector2f DrawPosition;

        RenderTextTest ()
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

            Vector2f leftTop        = Vector2f ( slot->bitmap_left, slot->bitmap_top );
            Vector2f rightTop       = Vector2f ( leftTop.x () + slot->metrics.width, leftTop.y () );
            Vector2f leftBottom     = Vector2f ( leftTop.x (), leftTop.y () - slot->metrics.height );
            Vector2f rightBottom    = Vector2f ( leftBottom.x () + slot->metrics.width, leftBottom.y () );

            std::vector < Vector2f > vertexs;
            vertexs.push_back ( leftTop );
            vertexs.push_back ( rightTop );
            vertexs.push_back ( leftBottom );
            vertexs.push_back ( rightBottom );

            m_VertexBuffer = new ArrayBuffer ( vertexs.size () * 2 * 4, ArrayBuffer::ArrayBufferUsage::STATIC );
            m_VertexBuffer->CopyData ( &vertexs[ 0 ], vertexs.size () * 2 * 4 );
            m_VertexBuffer->TransferData ();

            // Texcoord Data

            std::vector < Vector2f > texcoords;
            texcoords.push_back ( Vector2f ( 0.0f, 1.0f ) );
            texcoords.push_back ( Vector2f ( 1.0f, 1.0f ) );
            texcoords.push_back ( Vector2f ( 0.0f, 0.0f ) );
            texcoords.push_back ( Vector2f ( 1.0f, 0.0f ) );

            m_TexcoordBuffer = new ArrayBuffer ( texcoords.size () * 2 * 4, ArrayBuffer::ArrayBufferUsage::STATIC );
            m_TexcoordBuffer->CopyData ( &texcoords[ 0 ], texcoords.size () * 2 * 4 );
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

            delete[] texData;

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

            // Vertex and Texcoord Binding

            glBindBuffer ( GL_ARRAY_BUFFER, m_VertexBuffer->BufferHandle () );
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

            glBindBuffer ( GL_ARRAY_BUFFER, m_TexcoordBuffer->BufferHandle () );
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


            // Init Shader


        }

        ~RenderTextTest ()
        {

        }

        void OnPreDraw ()
        {
            glBindTexture ( GL_TEXTURE_2D, m_CharTexture );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glDisable ( GL_CULL_FACE );

            
        }

        void OnDraw ()
        {

        }
        
        void OnPostDraw ()
        {

        }

        // Text render test
        FT_Library m_FTLibrary;
        FT_Face    m_FTFace;

        GLuint m_CharTexture;
        GLuint m_VertexArray;

        ArrayBuffer *m_VertexBuffer;
        ArrayBuffer *m_TexcoordBuffer;

        ShaderObject m_VertexShader;
        ShaderObject m_FragmentShader;
    };

    const Vector2f RenderTextTest::DrawPosition = Vector2f ( 100.0f, 100.0f );
}


#endif