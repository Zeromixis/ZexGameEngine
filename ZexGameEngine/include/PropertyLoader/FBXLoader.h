#ifndef _PROPERTYLOADER_FBXLOADER_H_
#define _PROPERTYLOADER_FBXLOADER_H_

#include "ZGEDecl.h"

#include "External/FBXSDK/fbxsdk.h"

#include "Core/Asset/PropertyLoader.h"
#include "Property/Mesh.h"
#include "Pattern/Singleton.h"

namespace ZGE
{
    // For every material, record the offsets in every VBO and triangle counts
    struct SubMesh
    {
        SubMesh () : IndexOffset ( 0 ), TriangleCount ( 0 ) {}

        int IndexOffset;
        int TriangleCount;
    };

    class FBXLoader
        : public PropertyLoader
        , public Singleton< FBXLoader >
    {
        friend class Singleton< FBXLoader >;
    public:
        constexpr static U16 TRIGANLE_VERTEX_COUNT = 3;

        ~FBXLoader ();

        std::string GetFBXSDKVersion ();

        bool LoadFBX ( const std::string &fileName, Mesh &outMesh );

    protected:
        void LoadFBXRescursive ( FbxNode *node, Mesh &outMesh );

        void LoadFBXMesh ( FbxNode *node, Mesh &outMesh );

    private:
        FBXLoader ();

        void InitFBXSdkObject ();

        void DestroyFBXSdkObject ();


    private:
        FbxManager *m_FbxManager;
        
        std::string m_FBXSDKVersion;

        FbxArray<SubMesh*> mSubMeshes;

    };
}

#endif
