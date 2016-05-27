#include "Test/FBXTest.h"
#include "PropertyLoader/FBXLoader.h"
#include "Core/BufferGL.h"
#include "Util.h"
#include "App/Context.h"
#include "Math/MathConst.h"

namespace ZGE
{

    FBXTest::FBXTest ()
    {
        m_CameraController = new CameraController{&m_Camera};
        auto window = Context::GetInstance ()->GetWindowPtr ();
        F32 cameraAspect = (F32)(window->Width ()) / window->Height ();
        m_Camera.SetProj (MathConst::PI / 4, cameraAspect, 0.1f, 200.0f);
        m_Camera.SetView (Vector3f (-20.0f, 20.0f, 0.0f), Vector3f (0.0f, 0.0f, 0.0f));

        std::vector< PMesh > meshList;

        FBXLoader::GetInstance ()->LoadFBXFile ("FBX/JEMINA vase.fbx", meshList);

        m_PFBXMesh = meshList [0];

        auto &vertices = meshList [0]->VertexList;
        auto &indices = meshList [0]->IndexList;

        m_PositionBuffer = new BufferGL< GL_ARRAY_BUFFER >{vertices.size () * sizeof (Vector3f), ArrayBufferUsage::STATIC};
        m_NormalBuffer = new BufferGL< GL_ARRAY_BUFFER >{vertices.size () * sizeof (Vector3f), ArrayBufferUsage::STATIC};
        m_UVBuffer = new BufferGL< GL_ARRAY_BUFFER >{vertices.size () * sizeof (Vector2f), ArrayBufferUsage::STATIC};

        m_IndexBuffer = new BufferGL< GL_ELEMENT_ARRAY_BUFFER >{indices.size () * sizeof (U32), ArrayBufferUsage::STATIC};

        auto positions = new Vector3f [vertices.size ()];
        auto normals = new Vector3f [vertices.size ()];
        auto uvs = new Vector2f [vertices.size ()];
        auto index = new U32 [indices.size ()];

        int counter = 0;
        for (auto &vertex : vertices)
        {
            positions [counter] = vertex.Position;
            normals [counter] = vertex.Normal;
            uvs [counter] = vertex.UV;
            ++counter;
        }

        counter = 0;
        for (auto &_index : indices)
        {
            index [counter] = _index;
            ++counter;
        }

        m_PositionBuffer->CopyData (positions, vertices.size () * sizeof (Vector3f));
        m_NormalBuffer->CopyData (normals, vertices.size () * sizeof (Vector3f));
        m_UVBuffer->CopyData (uvs, vertices.size () * sizeof (Vector2f));
        m_IndexBuffer->CopyData (index, indices.size () * sizeof (U32));

        m_PositionBuffer->SendData ();
        m_NormalBuffer->SendData ();
        m_UVBuffer->SendData ();
        m_IndexBuffer->SendData ();

        delete [] positions;
        delete [] normals;
        delete [] uvs;
        delete [] index;

        // Prepare Vertex Array Data
        glGenVertexArrays (1, &m_VertexArrayObject);
        glBindVertexArray (m_VertexArrayObject);

        // Position
        glBindBuffer (m_PositionBuffer->TARGET, m_PositionBuffer->Handle ());
        glVertexAttribPointer
        (
            0,                      // index
            3,                      // size
            GL_FLOAT,               // type
            GL_FALSE,               // isNormalized
            0,                      // stride
            0                       // pointer
        );

        // Normal
        glBindBuffer (m_NormalBuffer->TARGET, m_NormalBuffer->Handle ());
        glVertexAttribPointer
        (
            1,                      // index
            3,                      // size
            GL_FLOAT,               // type
            GL_FALSE,               // isNormalized
            0,                      // stride
            0                       // pointer
        );

        // UV
        glBindBuffer (m_UVBuffer->TARGET, m_UVBuffer->Handle ());
        glVertexAttribPointer
        (
            2,                      // index
            2,                      // size
            GL_FLOAT,               // type
            GL_FALSE,               // isNormalized
            0,                      // stride
            0                       // pointer
        );

        // Prepare Shader.
        m_NewShader = ShaderProgram::Create ();

        std::wstring vertexShaderPath = L"Shader/FBXVS.vp";
        std::wstring vertexShaderSource = LoadShaderSourceFromFile (vertexShaderPath);

        PShaderCode vertexShaderCode = ShaderCode::Create (eShaderType::eVERTEX_SHADER, ConvertWStringToString (vertexShaderSource));
        m_NewShader->AttachCode (vertexShaderCode);

        std::wstring fragmentShaderPath = L"Shader/FBXFS.fp";
        std::wstring fragmentShaderSource = LoadShaderSourceFromFile (fragmentShaderPath);

        PShaderCode fragmentShaderCode = ShaderCode::Create (eShaderType::eFRAGMENT_SHADER, ConvertWStringToString (fragmentShaderSource));
        m_NewShader->AttachCode (fragmentShaderCode);

        // Vertex Attr location must be done before link.

        m_NewShader->BindAttrLocation (0, "In_Position");
        m_NewShader->BindAttrLocation (1, "In_Normal");
        m_NewShader->BindAttrLocation (2, "In_UV");

        std::string errorStr;
        m_NewShader->Link (errorStr);

        // Init Texture
        std::vector<unsigned char> Pixels;
        std::fstream hFile ("FBX/jemina vase_df.bmp", std::ios::in | std::ios::binary);
        if (!hFile.is_open ()) throw std::invalid_argument ("Error: File Not Found.");

        hFile.seekg (0, std::ios::end);
        int Length = hFile.tellg ();
        hFile.seekg (0, std::ios::beg);
        std::vector<std::uint8_t> FileInfo (Length);
        hFile.read (reinterpret_cast<char*>(FileInfo.data ()), 54);

        if (FileInfo [0] != 'B' && FileInfo [1] != 'M')
        {
            hFile.close ();
            throw std::invalid_argument ("Error: Invalid File Format. Bitmap Required.");
        }

        if (FileInfo [28] != 24 && FileInfo [28] != 32)
        {
            hFile.close ();
            throw std::invalid_argument ("Error: Invalid File Format. 24 or 32 bit Image Required.");
        }

        short BitsPerPixel = FileInfo [28];
        int width = FileInfo [18] + (FileInfo [19] << 8);
        int height = FileInfo [22] + (FileInfo [23] << 8);
        std::uint32_t PixelsOffset = FileInfo [10] + (FileInfo [11] << 8);
        std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
        Pixels.resize (size);

        hFile.seekg (PixelsOffset, std::ios::beg);
        hFile.read (reinterpret_cast<char*>(Pixels.data ()), size);
        hFile.close ();

        glEnable (GL_TEXTURE_2D);
        glActiveTexture (GL_TEXTURE0);
        glGenTextures (1, &m_TextureObject);
        glBindTexture (GL_TEXTURE_2D, m_TextureObject);
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            Pixels.data ()
        );
        m_NewShader->Uniform1i ("BmpSampler", 0);
    }

    FBXTest::~FBXTest ()
    {

    }

    void FBXTest::OnPreDraw ()
    {
        glBindVertexArray (m_VertexArrayObject);
        glEnableVertexAttribArray (0);
        glEnableVertexAttribArray (1);
        glEnableVertexAttribArray (2);

        glEnable (GL_CULL_FACE);
        glBindBuffer (GL_ARRAY_BUFFER, m_PositionBuffer->Handle ());
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->Handle ());

        glBindTexture (GL_TEXTURE_2D, m_TextureObject);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        m_NewShader->Bind ();
    }

    void FBXTest::OnDraw ()
    {
        auto modelMatrix = Float44::Identity () * MathFunc::CreateScaleMatrix (10.0f);
        auto viewMatrix = m_CameraController->GetCamera ()->ViewMatrix ();
        auto projMatrix = m_CameraController->GetCamera ()->ProjMatrix ();
        auto mvpMatrix = modelMatrix * viewMatrix * projMatrix;

        m_NewShader->UniformMatrix4fv ("mvpMatrix", mvpMatrix);

        auto indexBufferSize = m_IndexBuffer->Size ();

        glDrawElements
        (
            GL_TRIANGLES,               // Element Draw Mode
            m_IndexBuffer->Size () / 4, // Index Count
            GL_UNSIGNED_INT,            // Index Type
            0                           // VBO offset set to 0
        );

        //         glBegin ( GL_LINES );
        //         {
        //             glColor3i (255, 0, 0);
        //             glVertex3f (0.0f, 0.0f, 0.0f);
        //             glVertex3f (100.0f, 0.0f, 0.0f);
        //         }
        //         glEnd ();
        // 
        //         glBegin (GL_LINES);
        //         {
        //             glColor3i (0, 255, 0);
        //             glVertex3f (0.0f, 0.0f, 0.0f);
        //             glVertex3f (0.0f, 100.0f, 0.0f);
        //         }
        //         glEnd ();
        // 
        //         glBegin (GL_LINES);
        //         {
        //             glColor3i (0, 0, 255);
        //             glVertex3f (0.0f, 0.0f, 0.0f);
        //             glVertex3f (0.0f, 0.0f, 100.0f);
        //         }
        //         glEnd ();
    }

    void FBXTest::OnPostDraw ()
    {
        glDisableVertexAttribArray (0);
        glDisableVertexAttribArray (1);
        glDisableVertexAttribArray (2);
        glBindVertexArray (0);

        glBindTexture (GL_TEXTURE_2D, 0);

        glDisable (GL_CULL_FACE);

        m_NewShader->UnBind ();
    }

}