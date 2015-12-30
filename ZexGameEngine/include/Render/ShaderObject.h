#ifndef _RENDER_SHADEROBJECT_H_
#define _RENDER_SHADEROBJECT_H_

#include <string>
#include "glloader/glloader.h"

namespace ZGE
{
    class ShaderObject
    {
    public:
        enum ShaderType
        {
            Vertex,
            Fragment,
        };

        ShaderObject ();

        ~ShaderObject ();

        void AttachShader ( ShaderType shaderType, const std::string& sourceCode );
        void LinkProgram ();

        GLuint Program () const;

        void Bind ();
        void UnBind ();

    protected:
        GLuint m_GLSLProgram;

    private:
        ShaderObject ( const ShaderObject & );
        void operator = ( const ShaderObject & );
    };
}

#endif