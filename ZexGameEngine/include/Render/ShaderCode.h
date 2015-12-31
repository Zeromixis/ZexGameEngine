#ifndef _RENDER_SHADERCODE_H_
#define _RENDER_SHADERCODE_H_

#include "ZGEDecl.h"

#include "Object.h"

namespace ZGE
{
    class ShaderCode;

    enum eShaderType
    {
        NONE,
        VERTEX_SHADER,
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
    };

    using PShaderCode = std::shared_ptr< ShaderCode >;

    class ShaderCode
        : public Object
    {
        friend class ShaderProgram;
    public:
        static PShaderCode Create ( const eShaderType &shaderType, const std::string &sourceCode );

        ~ShaderCode ();

        bool Compile ( std::string &outErrorStr );

    private:
        ShaderCode ();

    private:
        eShaderType m_ShaderType;

        GLuint      m_ShaderObject;

        std::string m_SourceCode;
    };
}


#endif // !_RENDER_SHADERCODE_H_
