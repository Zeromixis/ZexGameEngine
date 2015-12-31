#ifndef _RENDER_SHADERPROGRAM_H_
#define _RENDER_SHADERPROGRAM_H_

#include "ZGEDecl.h"

#include "Math/Matrix.h"
#include "Render/ShaderCode.h"

namespace ZGE
{
    using PShaderProgram = std::shared_ptr< ShaderProgram >;

    class ShaderProgram
        : public Object
    {
    public:
        static PShaderProgram Create ();

        ~ShaderProgram ();

        void AttachCode ( const PShaderCode &pCode );

        void DetachCode ( const PShaderCode &pCode );

        bool Link ( std::string &outErrorStr );

        I32 UniformLocation ( const std::string &uniformName );

        void Uniform1i ( const I32 &uniformLoc, const I32 &value );

        void UniformMatrix4fv ( const I32 &uniformLoc, const Float44 &mat );

        void BindAttrLocation ( const U32 &index, const std::string &attrName );

    private:
        ShaderProgram ();

    private:
        U32 m_ProgramObject;

        std::set< PShaderCode > m_ShaderCodeList;
    };
}

#endif // !_RENDER_SHADERPROGRAM_H_

