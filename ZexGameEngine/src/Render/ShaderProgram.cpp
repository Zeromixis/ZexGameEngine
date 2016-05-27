#include "Render/ShaderProgram.h"

namespace ZGE
{
    PShaderProgram ShaderProgram::Create ()
    {
        PShaderProgram pProgram{new ShaderProgram ()};
        return pProgram;
    }

    ShaderProgram::~ShaderProgram ()
    {
        for (auto &pCode : m_ShaderCodeList)
        {
            glDetachShader (m_Program, pCode->m_ShaderObject);
        }

        if (m_Program != 0)
        {
            glDeleteProgram (m_Program);
        }
    }

    void ShaderProgram::AttachCode (const PShaderCode &pCode)
    {
        std::string sError;
        if (pCode->Compile (sError))
        {
            m_ShaderCodeList.insert (pCode);
        }
        else
        {
            std::cout << sError << std::endl;
        }

    }

    void ShaderProgram::DetachCode (const PShaderCode &pCode)
    {
        m_ShaderCodeList.erase (pCode);
    }

    bool ShaderProgram::Link (std::string &outErrorStr)
    {
        for (auto &pCode : m_ShaderCodeList)
        {
            glAttachShader (m_Program, pCode->m_ShaderObject);
        }

        glLinkProgram (m_Program);

        GLint isLinked = GL_FALSE;
        glGetProgramiv (m_Program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            std::cout << "Shader Program Link Failed!" << std::endl;
            GLsizei logSize = 0;
            glGetProgramiv (m_Program, GL_INFO_LOG_LENGTH, &logSize);

            std::vector< char > errorlog (logSize);
            glGetProgramInfoLog (m_Program, logSize, &logSize, &errorlog [0]);

            outErrorStr.assign (errorlog.begin (), errorlog.end ());
        }
        if (isLinked == GL_TRUE)
        {
            m_IsLink = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    void ShaderProgram::Bind ()
    {
        glUseProgram (m_Program);
    }

    void ShaderProgram::UnBind ()
    {
        glUseProgram (0);
    }

    ZGE::I32 ShaderProgram::GetUniformLocation (const std::string &uniformName)
    {
        if (m_IsLink)
        {
            I32 nowUsingProgram = 0;
            glGetIntegerv (GL_CURRENT_PROGRAM, &nowUsingProgram);
            glUseProgram (m_Program);
            I32 loc = glGetUniformLocation (m_Program, uniformName.c_str ());
            glUseProgram (nowUsingProgram);
            return loc;
        }
        else
        {
            std::cout << "Shader Program it not Linked!" << std::endl;
            return -1;
        }
    }

    void ShaderProgram::Uniform1i (const I32 &uniformLoc, const I32 &value)
    {
        glUniform1i (m_Program, value);
    }

    void ShaderProgram::Uniform1i (const std::string &uniformName, const I32 &value)
    {
        auto loc = GetUniformLocation (uniformName);
        if (loc != -1)
        {
            Uniform1i (loc, value);
        }
    }

    void ShaderProgram::UniformMatrix4fv (const I32 &uniformLoc, const Float44 &mat)
    {
        glUniformMatrix4fv (uniformLoc, 1, false, static_cast<const GLfloat *>(&(mat [0] [0])));
    }

    void ShaderProgram::UniformMatrix4fv (const std::string &uniformName, const Float44 &mat)
    {
        auto loc = GetUniformLocation (uniformName);
        if (loc != -1)
        {
            UniformMatrix4fv (loc, mat);
        }
    }

    void ShaderProgram::BindAttrLocation (const U32 &index, const std::string &attrName)
    {
        glBindAttribLocation (m_Program, index, attrName.c_str ());
    }

    ShaderProgram::ShaderProgram ()
    {
        m_Program = glCreateProgram ();
        m_IsLink = false;
    }

}