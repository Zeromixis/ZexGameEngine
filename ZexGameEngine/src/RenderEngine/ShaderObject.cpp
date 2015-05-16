#include "Render/ShaderObject.h"
#include <assert.h>
#include <iostream>


namespace ZGE
{
    ShaderObject::ShaderObject ()
    {
        m_GLSLProgram = glCreateProgram ();
    }

    ShaderObject::~ShaderObject ()
    {
        glDeleteProgram ( m_GLSLProgram );
    }

    void ShaderObject::AttachShader ( ShaderType shaderType, const std::string& sourceCode )
    {
        GLuint shader;

        switch ( shaderType )
        {
        case Vertex:
            shader = glCreateShader ( GL_VERTEX_SHADER );
            break;
        case Fragment:
            shader = glCreateShader ( GL_FRAGMENT_SHADER );
            break;
        default:
            assert ( false );
            break;
        }
        const char *src = sourceCode.c_str ();
        glShaderSource ( shader, 1, &src, nullptr );
        glCompileShader ( shader );

        GLint isCompiled = false;
        glGetShaderiv ( shader, GL_COMPILE_STATUS, &isCompiled );
		if ( !isCompiled )
		{
			std::cout << "Shader Compile failed!" << std::endl;
			//throw new std::exception ();
		}

        glAttachShader ( m_GLSLProgram, shader );

        glDeleteShader ( shader );
    }

    void ShaderObject::LinkProgram ()
    {
        glLinkProgram ( m_GLSLProgram );
		
        GLint isLinked = false;
        glGetProgramiv ( m_GLSLProgram, GL_LINK_STATUS, &isLinked );
		if ( !isLinked)
		{
			std::cout << "Shader Link failed!" << std::endl;
			//throw new std::exception ();
		}
    }

    GLuint ShaderObject::GLSLProgram () const
    {
        return m_GLSLProgram;
    }

    void ShaderObject::Bind ()
    {
        glUseProgram ( m_GLSLProgram );
    }

    void ShaderObject::UnBind ()
    {
        glUseProgram ( 0 );
    }
}

