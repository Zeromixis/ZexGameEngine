#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "Property/Mesh.h"
#include "Render/ShaderObject.h"
#include "Render/ShaderProgram.h"
#include "Core/Camera.h"
#include "Core/CameraController.h"
#include "Core/BufferGL.h"

namespace ZGE
{
    class FBXTest
    {
    public:
        FBXTest ();

        ~FBXTest ();

        void OnPreDraw ();

        void OnDraw ();
        
        void OnPostDraw ();

    protected:
        PMesh m_PFBXMesh;

        ShaderObject m_Shader;

        PShaderProgram m_NewShader;

        CameraController *m_CameraController;

        Camera m_Camera;

        GLuint m_VertexArrayObject;

        GLuint m_TextureObject;

    private:
        BufferGL< GL_ARRAY_BUFFER > *m_PositionBuffer;
        BufferGL< GL_ARRAY_BUFFER > *m_NormalBuffer;
        BufferGL< GL_ARRAY_BUFFER > *m_UVBuffer;
        BufferGL< GL_ELEMENT_ARRAY_BUFFER > *m_IndexBuffer;



    };
}

