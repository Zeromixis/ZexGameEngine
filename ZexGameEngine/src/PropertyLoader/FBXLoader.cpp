#include "PropertyLoader/FBXLoader.h"

namespace ZGE
{
    FBXLoader::FBXLoader ()
    {
        InitFBXSdkObject ();
    }

    void FBXLoader::InitFBXSdkObject ()
    {
        m_FbxManager = FbxManager::Create ();
        m_FBXSDKVersion = m_FbxManager->GetVersion ();

        FbxIOSettings* ios = FbxIOSettings::Create ( m_FbxManager, IOSROOT );
        m_FbxManager->SetIOSettings ( ios );
    }

    void FBXLoader::DestroyFBXSdkObject ()
    {
        m_FbxManager->Destroy ();
    }

    FBXLoader::~FBXLoader ()
    {
        DestroyFBXSdkObject ();
    }

    std::string FBXLoader::GetFBXSDKVersion ()
    {
        return m_FBXSDKVersion;
    }

    bool FBXLoader::LoadFBX ( const std::string &fileName, Mesh &outMesh )
    {
        FbxImporter *importer = FbxImporter::Create ( m_FbxManager, "" );

        auto importStatus = importer->Initialize ( fileName.c_str (), -1, m_FbxManager->GetIOSettings () );

        if ( !importStatus )
        {
            auto error = importer->GetStatus ().GetErrorString ();
            assert ( false );
            return false;
        }

        FbxScene *scene = FbxScene::Create ( m_FbxManager, "ZGE Scene" );

        auto sceneStatus =importer->Import ( scene );

        if ( !sceneStatus )
        {
            assert ( false );
            return false;
        }

        // Convert Axis System to what is used in this example, if needed
        FbxAxisSystem SceneAxisSystem = scene->GetGlobalSettings ().GetAxisSystem ();
        FbxAxisSystem OurAxisSystem ( FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eLeftHanded );
        if ( SceneAxisSystem != OurAxisSystem )
        {
            OurAxisSystem.ConvertScene ( scene );
        }

        // Convert mesh, NURBS and patch into triangle mesh
        FbxGeometryConverter geomertryConverter ( m_FbxManager );
        geomertryConverter.Triangulate ( scene, true );

        // Rescursive Load Mesh From Scene

        LoadFBXRescursive ( scene->GetRootNode (), outMesh );

        scene->Destroy ();
        importer->Destroy ();
        return true;
    }

    void FBXLoader::LoadFBXRescursive ( FbxNode *node, Mesh &outMesh )
    {
        auto nodeAttr = node->GetNodeAttribute ();
        if ( nodeAttr )
        {
            if ( nodeAttr->GetAttributeType () == FbxNodeAttribute::eMesh )
            {
                LoadFBXMesh ( node, outMesh );
            }
        }

        auto nodeChildCount = node->GetChildCount ();
        for ( int i = 0; i < nodeChildCount; ++i )
        {
            LoadFBXRescursive ( node->GetChild ( i ), outMesh );
        }
    }

    void FBXLoader::LoadFBXMesh ( FbxNode *node, Mesh &outMesh )
    {
        auto mesh               = node->GetMesh ();
        const auto polygonCount = mesh->GetPolygonCount ();
        auto controlPoint       = mesh->GetControlPoints ();
        const auto controlPointCount = mesh->GetControlPointsCount ();

        auto hasNormal    = mesh->GetElementNormalCount () > 0;
        auto hasUV        = mesh->GetElementUVCount () > 0;
        auto hasColor     = mesh->GetElementVertexColorCount () > 0;

        // Count the polygon count of each material
        FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
        FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;

        if ( mesh->GetElementMaterial () )
        {
            lMaterialIndice = &mesh->GetElementMaterial ()->GetIndexArray ();
            lMaterialMappingMode = mesh->GetElementMaterial ()->GetMappingMode ();
            if ( lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon )
            {
                assert ( lMaterialIndice->GetCount () == polygonCount );
                if ( lMaterialIndice->GetCount () == polygonCount )
                {
                    // Count the faces of each material
                    for ( int lPolygonIndex = 0; lPolygonIndex < polygonCount; ++lPolygonIndex )
                    {
                        const int lMaterialIndex = lMaterialIndice->GetAt ( lPolygonIndex );
                        if ( mSubMeshes.GetCount () < lMaterialIndex + 1 )
                        {
                            mSubMeshes.Resize ( lMaterialIndex + 1 );
                        }
                        if ( mSubMeshes[ lMaterialIndex ] == NULL )
                        {
                            mSubMeshes[ lMaterialIndex ] = new SubMesh;
                        }
                        mSubMeshes[ lMaterialIndex ]->TriangleCount += 1;
                    }

                    // Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
                    // if, in the loop above, we resized the mSubMeshes by more than one slot.
                    for ( int i = 0; i < mSubMeshes.GetCount (); i++ )
                    {
                        if ( mSubMeshes[ i ] == NULL )
                            mSubMeshes[ i ] = new SubMesh;
                    }

                    // Record the offset (how many vertex)
                    const int lMaterialCount = mSubMeshes.GetCount ();
                    int lOffset = 0;
                    for ( int lIndex = 0; lIndex < lMaterialCount; ++lIndex )
                    {
                        mSubMeshes[ lIndex ]->IndexOffset = lOffset;
                        lOffset += mSubMeshes[ lIndex ]->TriangleCount * 3;
                        // This will be used as counter in the following procedures, reset to zero
                        mSubMeshes[ lIndex ]->TriangleCount = 0;
                    }
                    assert ( lOffset == polygonCount * 3 );
                }
            }
        }

        // All faces will use the same material.
        if ( mSubMeshes.GetCount () == 0 )
        {
            mSubMeshes.Resize ( 1 );
            mSubMeshes[ 0 ] = new SubMesh ();
        }


        //  The Mesh Vertex Data is saved by controlpoint ( true ) or polygon vertex ( false ).
        bool vertexDataControlModeIsByControlPoint = true;

        if ( hasNormal )
        {
            auto normalMappingMode = mesh->GetElementNormal ( 0 )->GetMappingMode ();
            if ( normalMappingMode == FbxLayerElement::eNone )
            {
                hasNormal = false;
            }
            if ( hasNormal && normalMappingMode != FbxLayerElement::eByControlPoint )
            {
                vertexDataControlModeIsByControlPoint = false;
            }
        }

        if ( hasUV )
        {
            auto uvMappingMode = mesh->GetElementUV ( 0 )->GetMappingMode ();
            if ( uvMappingMode == FbxLayerElement::eNone )
            {
                hasUV = false;
            }
            if ( hasUV && uvMappingMode != FbxLayerElement::eByControlPoint )
            {
                vertexDataControlModeIsByControlPoint = false;
            }
        }

        if ( hasColor )
        {
            auto colorMappingMode = mesh->GetElementVertexColor ( 0 )->GetMappingMode ();
            if ( colorMappingMode == FbxLayerElement::eNone )
            {
                hasColor = false;
            }
            if ( hasColor && colorMappingMode != FbxLayerElement::eByControlPoint )
            {
                vertexDataControlModeIsByControlPoint = false;
            }
        }

        auto polygonVertexCount = 0;

        if ( vertexDataControlModeIsByControlPoint )
        {
            polygonVertexCount = controlPointCount;
        }
        else
        {
            polygonVertexCount = polygonCount * TRIGANLE_VERTEX_COUNT;
        }
        
        outMesh.GetVerticesRef ().resize ( polygonVertexCount );
        outMesh.GetVertexIndicesRef ().resize ( polygonCount * TRIGANLE_VERTEX_COUNT );

        Vertex outMeshVertex;

        if ( vertexDataControlModeIsByControlPoint )
        {
            // By Control Point
            // Read Position
            for ( int index = 0; index < polygonVertexCount; ++index )
            {
                outMeshVertex.Position.x () = controlPoint[ index ][ 0 ];
                outMeshVertex.Position.y () = controlPoint[ index ][ 1 ];
                outMeshVertex.Position.z () = controlPoint[ index ][ 2 ];

                if ( hasNormal )
                {
                    int normalIndex = 0;
                    switch ( mesh->GetElementNormal ( 0 )->GetReferenceMode () )
                    {
                    case FbxLayerElement::eDirect:
                    case FbxLayerElement::eIndex:
                    {
                        normalIndex = index;
                        break;
                    }
                    case FbxLayerElement::eIndexToDirect:
                    {
                        normalIndex = mesh->GetElementNormal ( 0 )->GetIndexArray ().GetAt ( index );
                        break;
                    }
                    }

                    auto normal = mesh->GetElementNormal ( 0 )->GetDirectArray ().GetAt ( normalIndex );
                    outMeshVertex.Normal.x () = normal[ 0 ];
                    outMeshVertex.Normal.y () = normal[ 1 ];
                    outMeshVertex.Normal.z () = normal[ 2 ];
                }

                if ( hasUV )
                {
                    int uvIndex = 0;
                    switch ( mesh->GetElementUV ( 0 )->GetReferenceMode () )
                    {
                    case FbxLayerElement::eDirect:
                    case FbxLayerElement::eIndex:
                    {
                        uvIndex = index;
                        break;
                    }
                    case FbxLayerElement::eIndexToDirect:
                    {
                        uvIndex = mesh->GetElementUV ( 0 )->GetIndexArray ().GetAt ( index );
                        break;
                    }
                    }

                    auto uv = mesh->GetElementUV ( 0 )->GetDirectArray ().GetAt ( uvIndex );
                    outMeshVertex.UVPosition.x () = uv[ 0 ];
                    outMeshVertex.UVPosition.y () = uv[ 1 ];
                }

                // Read Color
                if ( hasColor )
                {
                    int colorIndex = 0;
                    switch ( mesh->GetElementVertexColor ( 0 )->GetReferenceMode () )
                    {
                    case FbxLayerElement::eDirect:
                    case FbxLayerElement::eIndex:
                    {
                        colorIndex = index;
                        break;
                    }
                    case FbxLayerElement::eIndexToDirect:
                    {
                        colorIndex = mesh->GetElementVertexColor ( 0 )->GetIndexArray ().GetAt ( index );
                        break;
                    }
                    }

                    auto color = mesh->GetElementVertexColor ( 0 )->GetDirectArray ().GetAt ( colorIndex );
                    outMeshVertex.Color.x () = color[ 0 ];
                    outMeshVertex.Color.y () = color[ 1 ];
                    outMeshVertex.Color.z () = color[ 2 ];
                    outMeshVertex.Color.w () = color[ 3 ];
                }

                outMesh.GetVerticesRef ()[ index ] = outMeshVertex;
            }          
        }
        else
        {
            // By polygon Vertex
            int vertexCounter = 0;
            for ( auto polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex )
            {
                // The material for current face.
                int lMaterialIndex = 0;
                if ( lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon )
                {
                    lMaterialIndex = lMaterialIndice->GetAt ( polygonIndex );
                }

                // Where should I save the vertex attribute index, according to the material
                const int lIndexOffset = mSubMeshes[ lMaterialIndex ]->IndexOffset +
                    mSubMeshes[ lMaterialIndex ]->TriangleCount * 3;

                for ( auto triganleVertexIndex = 0; triganleVertexIndex < TRIGANLE_VERTEX_COUNT; ++triganleVertexIndex )
                {
                    auto controlPointIndex = mesh->GetPolygonVertex ( polygonIndex, triganleVertexIndex );
                    auto vertex = controlPoint[ controlPointIndex ];

                    // Read Indices
                    outMesh.GetVertexIndicesRef ()[ lIndexOffset + triganleVertexIndex ] = static_cast<unsigned int>( vertexCounter );

                    // Read Position
                    outMeshVertex.Position.x () = vertex[ 0 ];
                    outMeshVertex.Position.y () = vertex[ 1 ];
                    outMeshVertex.Position.z () = vertex[ 2 ];

                    // Read Normal
                    if ( hasNormal )
                    {
                        FbxVector4 normal;
                        mesh->GetPolygonVertexNormal ( polygonIndex, triganleVertexIndex, normal );
                        outMeshVertex.Normal.x () = normal[ 0 ];
                        outMeshVertex.Normal.y () = normal[ 1 ];
                        outMeshVertex.Normal.z () = normal[ 2 ];
                    }
                    
                    // Read UV
                    if ( hasUV )
                    {
                        FbxVector2 uv;
                        const char *uvName = NULL;
                        bool isUnmappedUV;
                        mesh->GetPolygonVertexUV ( polygonIndex, triganleVertexIndex, uvName, uv, isUnmappedUV );
                        if ( uv[ 0 ] != 0.0f || uv[ 1 ] != 0.0f )
                            DebugBreak ();
                        outMeshVertex.UVPosition.x () = uv[ 0 ];
                        outMeshVertex.UVPosition.y () = uv[ 1 ];
                    }

                    if ( hasColor )
                    {
                        int colorIndex = 0;
                        switch ( mesh->GetElementVertexColor ( 0 )->GetReferenceMode () )
                        {
                        case FbxLayerElement::eDirect:
                        case FbxLayerElement::eIndex:
                        {
                            colorIndex = controlPointIndex;
                            break;
                        }
                        case FbxLayerElement::eIndexToDirect:
                        {
                            colorIndex = mesh->GetElementVertexColor ( 0 )->GetIndexArray ().GetAt ( controlPointIndex );
                            break;
                        }
                        }

                        auto color = mesh->GetElementVertexColor ( 0 )->GetDirectArray ().GetAt ( colorIndex );
                        outMeshVertex.Color.x () = color[ 0 ];
                        outMeshVertex.Color.y () = color[ 1 ];
                        outMeshVertex.Color.z () = color[ 2 ];
                        outMeshVertex.Color.w () = color[ 3 ];
                    }
                    outMesh.GetVerticesRef ()[ vertexCounter ] = outMeshVertex;
                    ++vertexCounter;
                }
                mSubMeshes[ lMaterialIndex ]->TriangleCount += 1;
            }
        }

//         // Read Indices.
//         for ( auto polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex )
//         {
//             for ( auto vertexIndex = 0; vertexIndex < TRIGANLE_VERTEX_COUNT; ++vertexIndex )
//             {
//                 const auto controlPointIndex = mesh->GetPolygonVertex ( polygonIndex, vertexIndex );
//                 outMesh.GetVertexIndicesRef ()[ polygonIndex * TRIGANLE_VERTEX_COUNT + vertexIndex ] = controlPointIndex;
//             }
//         }

        
    }

}