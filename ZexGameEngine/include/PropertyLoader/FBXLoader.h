#ifndef _PROPERTYLOADER_FBXLOADER_H_
#define _PROPERTYLOADER_FBXLOADER_H_

#include "ZGEDecl.h"

#include "External/FBXSDK/fbxsdk.h"

#include "Math/Matrix.h"
#include "Core/Asset/PropertyLoader.h"
#include "Property/Mesh.h"
#include "Pattern/Singleton.h"
#include "Property/Animation.h"

namespace ZGE
{
    // For every material, record the offsets in every VBO and triangle counts
    struct SubMesh
    {
        SubMesh ()
            : IndexOffset ( 0 )
            , TriangleCount ( 0 ) 
        {

        }

        int IndexOffset;
        int TriangleCount;
    };

    struct ControlPoint
       : public Property
   {
       ControlPoint ()
       {

       }
   
       std::vector< std::pair < U32, I32 > > JointIndexWeightPairList;

       std::vector< PVertex > RelatedVertexList;                        // Record which Vertex is related to this ControlPoint
   };

    using PControlPoint = PropertyHandle< ControlPoint >;

    class FBXLoader
        : public PropertyLoader
        , public Singleton< FBXLoader >
    {
        friend class Singleton< FBXLoader >;
    public:
        constexpr static U16 TRIGANLE_VERTEX_COUNT = 3;

        ~FBXLoader ();

        std::string GetFBXSDKVersion ();

        bool LoadFBXMesh ( const std::string &fileName, std::vector< PMesh > &inoutMeshList );

        void FbxVector42Vector4f ( const FbxVector4 & inFbxVector4, Vector4f & outVector4f );

        static void FbxMatrix2Float44 ( const FbxMatrix &inFbxMatrix, Float44 &outFloat44Matrix )
        {

        }

        template< class TFbxVector, class TVector >
        static void FbxVector2Vector ( const TFbxVector &inFbxVector, TVector &outVector )
        {
            for ( auto i = 0; i < TVector::size; ++i )
            {
                outVector[ i ] = inFbxVector[ i ];
            }
        }

    protected:
        bool LoadFBXAnimNode ( FbxScene *scene, const std::vector< PMesh > &meshList, std::vector< PAnimNode > &jointAnimList );

        void LoadFBXMesh ( FbxNode *node, std::vector< PMesh > &inoutMeshList );

        void ProcessMeshVertices ( FbxNode *node, PMesh &inoutMesh );

        void ProcessMeshJoints ( FbxNode *node, PMesh &inoutMesh );

    private:
        FBXLoader ();

        void InitFBXSdkObject ();

        void DestroyFBXSdkObject ();

        FbxAMatrix GetGeometryTransformation ( FbxNode *node );


    private:
        FbxManager *m_FbxManager;
        
        std::string m_FBXSDKVersion;

        FbxArray< SubMesh * > mSubMeshes;

        std::vector< PControlPoint > m_ControlPointList;
    };
}

#endif
