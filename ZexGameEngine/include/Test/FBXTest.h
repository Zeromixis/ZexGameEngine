#ifndef _TEST_FBXTEST_H_
#define _TEST_FBXTEST_H_

#include "Property/Mesh.h"
#include "Render/ShaderObject.h"
#include "Core/Camera.h"
#include "Core/CameraController.h"
#include "Core/ArrayBuffer.h"

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
        Mesh m_FBXMesh;

        ShaderObject m_Shader;

        CameraController *m_CameraController;

        Camera m_Camera;

        GLuint m_VertexArrayObject;

        GLuint m_TextureObject;

    private:
        ArrayBuffer< GL_ARRAY_BUFFER > *m_PositionBuffer;
        ArrayBuffer< GL_ARRAY_BUFFER > *m_NormalBuffer;
        ArrayBuffer< GL_ARRAY_BUFFER > *m_UVBuffer;
        ArrayBuffer< GL_ELEMENT_ARRAY_BUFFER > *m_IndexBuffer;



    };
}

#endif
