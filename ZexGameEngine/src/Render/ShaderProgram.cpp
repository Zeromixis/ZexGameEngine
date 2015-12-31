#include "Render/ShaderProgram.h"

namespace ZGE
{
    PShaderProgram ShaderProgram::Create ()
    {
        PShaderProgram pProgram { new ShaderProgram () };
        return pProgram;
    }

    ShaderProgram::~ShaderProgram ()
    {
        for ( auto &pCode : m_ShaderCodeList )
        {
            glDetachShader ( m_ProgramObject, pCode->m_ShaderObject );
        }

        if ( m_ProgramObject != 0 )
        {
            glDeleteProgram ( m_ProgramObject );
        }
    }

    void ShaderProgram::AttachCode ( const PShaderCode &pCode )
    {
        m_ShaderCodeList.insert ( pCode );
    }

    void ShaderProgram::DetachCode ( const PShaderCode &pCode )
    {
        m_ShaderCodeList.erase ( pCode );
    }

    bool ShaderProgram::Link ( std::string &outErrorStr )
    {
        for ( auto &pCode : m_ShaderCodeList )
        {
            glAttachShader ( m_ProgramObject, pCode->m_ShaderObject );
        }

        glLinkProgram ( m_ProgramObject );

        GLint isLinked = false;
        glGetProgramiv ( m_ProgramObject, GL_LINK_STATUS, &isLinked );
        if ( !isLinked )
        {
            std::cout << "Shader Program Link Failed!" << std::endl;
            GLsizei logSize = 0;
            glGetProgramiv ( m_ProgramObject, GL_INFO_LOG_LENGTH, &logSize );

            std::vector< char > errorlog ( logSize );
            glGetProgramInfoLog ( m_ProgramObject, logSize, &logSize, &errorlog[ 0 ] );

            outErrorStr.assign ( errorlog.begin (), errorlog.end () );
        }
        return isLinked;
    }

    ZGE::I32 ShaderProgram::UniformLocation ( const std::string &uniformName )
    {
        return glGetUniformLocation ( m_ProgramObject, uniformName.c_str () );
    }

    void ShaderProgram::Uniform1i ( const I32 &uniformLoc, const I32 &value )
    {
        glUniform1i ( m_ProgramObject, value );
    }

    void ShaderProgram::UniformMatrix4fv ( const I32 &uniformLoc, const Float44 &mat )
    {
        glUniformMatrix4fv ( uniformLoc, 1, false, &mat[ 0 ] );
    }

    void ShaderProgram::BindAttrLocation ( const U32 &index, const std::string &attrName )
    {
        glBindAttribLocation ( m_ProgramObject, index, attrName.c_str () );
    }

    ShaderProgram::ShaderProgram ()
    {
        m_ProgramObject = glCreateProgram ();
    }

}