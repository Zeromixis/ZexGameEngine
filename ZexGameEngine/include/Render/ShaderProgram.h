#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "Math/Matrix.h"
#include "Render/ShaderCode.h"

namespace ZGE
{
    typedef std::shared_ptr< ShaderProgram > PShaderProgram;

    class ShaderProgram
        : public Object
    {
    public:
        static PShaderProgram Create ();

        ~ShaderProgram ();

        void AttachCode (const PShaderCode &pCode);

        void DetachCode (const PShaderCode &pCode);

        bool Link (std::string &outErrorStr);

        void Bind ();

        void UnBind ();

        I32 GetUniformLocation (const std::string &uniformName);

        void Uniform1i (const I32 &uniformLoc, const I32 &value);

        void Uniform1i (const std::string &uniformName, const I32 &value);

        void UniformMatrix4fv (const I32 &uniformLoc, const Float44 &mat);

        void UniformMatrix4fv (const std::string &uniformName, const Float44 &mat);

        void BindAttrLocation (const U32 &index, const std::string &attrName);

    private:
        ShaderProgram ();

    private:
        U32 m_Program;

        bool m_IsLink;

        std::set< PShaderCode > m_ShaderCodeList;
    };
}


