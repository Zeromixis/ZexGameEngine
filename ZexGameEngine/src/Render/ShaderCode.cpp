#include "Render/ShaderCode.h"

namespace ZGE
{
    PShaderCode ShaderCode::Create ( const eShaderType &shaderType, const std::string &sourceCode )
    {
        PShaderCode pShaderCode;
        pShaderCode.reset ( new ShaderCode () );

        pShaderCode->m_ShaderType = shaderType;
        pShaderCode->m_SourceCode = sourceCode;

        return pShaderCode;
    }

    ShaderCode::~ShaderCode ()
    {
        if ( m_ShaderObject != 0 )
        {
            glDeleteShader ( m_ShaderObject );
        }
    }

    bool ShaderCode::Compile ( std::string &outErrorStr )
    {
        switch ( m_ShaderType )
        {
        case VERTEX_SHADER:
        {
            m_ShaderObject = glCreateShader ( GL_VERTEX_SHADER );
            break;
        }
        case GEOMETRY_SHADER:
        {
            m_ShaderObject = glCreateShader ( GL_GEOMETRY_SHADER );
            break;
        }
        case FRAGMENT_SHADER:
        {
            m_ShaderObject = glCreateShader ( GL_FRAGMENT_SHADER );
            break;
        }
        default:
        {
            assert ( false );
            break;
        }
        }
        auto sourceCode = m_SourceCode.c_str ();

        glShaderSource 
            ( 
                m_ShaderObject,                 // Shader Object
                1,                              // Source Code Number
                &sourceCode,                    // Pointer
                nullptr 
              );
        glCompileShader ( m_ShaderObject );

        GLint isCompiled = false;
        glGetShaderiv ( m_ShaderObject, GL_COMPILE_STATUS, &isCompiled );
        if ( !isCompiled )
        {
            std::cout << "Shader Code Compile Failed!" << std::endl;
            GLsizei logSize = 0;
            glGetShaderiv ( m_ShaderObject, GL_INFO_LOG_LENGTH, &logSize );

            std::vector< char > errorlog ( logSize );
            glGetShaderInfoLog ( m_ShaderObject, logSize, &logSize, &errorlog[ 0 ] );

            outErrorStr.assign ( errorlog.begin (), errorlog.end () );
        }

        return isCompiled;
    }

    ShaderCode::ShaderCode ()
    {
        m_ShaderType = NONE;
        m_ShaderObject = 0;
    }
}