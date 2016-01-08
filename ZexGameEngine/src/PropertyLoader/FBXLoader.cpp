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

    FbxAMatrix FBXLoader::GetGeometryTransformation ( FbxNode *node )
    {
        if ( node == nullptr )
        {
            throw std::exception ( "Null for mesh geometry" );
        }

        const FbxVector4 lT = node->GetGeometricTranslation ( FbxNode::eSourcePivot );
        const FbxVector4 lR = node->GetGeometricRotation ( FbxNode::eSourcePivot );
        const FbxVector4 lS = node->GetGeometricScaling ( FbxNode::eSourcePivot );

        return FbxAMatrix ( lT, lR, lS );
    }

    FBXLoader::~FBXLoader ()
    {
        DestroyFBXSdkObject ();
    }

    std::string FBXLoader::GetFBXSDKVersion ()
    {
        return m_FBXSDKVersion;
    }

    bool FBXLoader::LoadFBXMeshes( const std::string &fileName, std::vector< PMesh > &inoutMeshList )
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

        auto sceneStatus = importer->Import ( scene );

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

        LoadFBXMeshesRescursive ( scene->GetRootNode (), inoutMeshList );

        scene->Destroy ();
        importer->Destroy ();
        return true;
    }

    bool FBXLoader::LoadFBXAnimNode ( FbxScene *scene, const std::vector< PMesh > &meshList, std::vector< PAnimNode > &jointAnimList )
    {
        for ( const auto &mesh : meshList )
        {
            for ( const auto &joint : mesh->JointList )
            {
                FbxNode *jointNode = scene->FindNodeByName ( joint->Name.c_str () );

                if ( nullptr == jointNode )
                    continue;

                PAnimNode animNode = Property::CreateProperty< PAnimNode > ();
                animNode->Name = joint->Name;

                for ( int stackIndex = 0; stackIndex < scene->GetSrcObjectCount< FbxAnimStack > (); ++stackIndex )
                {
                    FbxAnimStack *animStack = scene->GetSrcObject< FbxAnimStack > ( stackIndex );
                    FbxTakeInfo* takeInfo = scene->GetTakeInfo ( animStack->GetName () );
                    FbxTime start = takeInfo->mLocalTimeSpan.GetStart ();
                    FbxTime end = takeInfo->mLocalTimeSpan.GetStop ();

                    PAnimData animData = Property::CreateProperty< PAnimData > ();

                    for (
                        FbxLongLong frameIndex = start.GetFrameCount ( FbxTime::eFrames60 );
                        frameIndex <= end.GetFrameCount ( FbxTime::eFrames60 );
                        ++frameIndex )
                    {
                        PAnimKeyFrame animKeyFrame = Property::CreateProperty< PAnimKeyFrame > ();

                        FbxTime currTime;
                        currTime.SetFrame ( frameIndex, FbxTime::eFrames60 );
                        FbxAMatrix globalTransform = jointNode->EvaluateGlobalTransform ( currTime );

                        animKeyFrame->Frame = frameIndex;
                        FbxVector42Vector4f ( globalTransform.GetT (), animKeyFrame->Translation );
                        FbxQuaternion2QuaternionF ( globalTransform.GetQ (), animKeyFrame->Rotation );
                        FbxVector42Vector4f ( globalTransform.GetS (), animKeyFrame->Scaling );

                        animData->KeyFrameList.push_back ( animKeyFrame );
                    }

                    animNode->AnimDataList.push_back ( animData );
                }

                if ( false == animNode->AnimDataList.empty () )
                    jointAnimList.push_back ( animNode );
            }
        }
        return true;
    }

    void FBXLoader::LoadFBXMeshesRescursive ( FbxNode *node, std::vector< PMesh > &inoutMeshList )
    {
        auto nodeAttr = node->GetNodeAttribute ();
        if ( nodeAttr )
        {
            if ( nodeAttr->GetAttributeType () == FbxNodeAttribute::eMesh )
            {
                PMesh mesh = Property::CreateProperty< PMesh > ();
                ProcessMeshVertices ( node, mesh );
                ProcessMeshJoints ( node, mesh );
                inoutMeshList.push_back ( mesh );
            }
        }

        auto nodeChildCount = node->GetChildCount ();
        for ( int i = 0; i < nodeChildCount; ++i )
        {
            LoadFBXMeshesRescursive ( node->GetChild ( i ), inoutMeshList );
        }
    }

    void FBXLoader::ProcessMeshVertices( FbxNode *node, PMesh &inoutMesh )
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
        
        inoutMesh->VertexList.resize ( polygonVertexCount );
        inoutMesh->VertexIndexList.resize ( polygonCount * TRIGANLE_VERTEX_COUNT );
        inoutMesh->ControlPointList.resize ( controlPointCount );

        // Read All Control Point
        for ( U32 i = 0; i < controlPointCount; ++i )
        {
            PControlPoint _controlPoint = Property::CreateProperty< PControlPoint > ();
            _controlPoint->Position.x () = controlPoint[ i ][ 0 ];
            _controlPoint->Position.y () = controlPoint[ i ][ 1 ];
            _controlPoint->Position.z () = controlPoint[ i ][ 2 ];
            _controlPoint->Position.w () = controlPoint[ i ][ 3 ];
            inoutMesh->ControlPointList.push_back ( _controlPoint );
        }

        if ( vertexDataControlModeIsByControlPoint )
        {
            // By Control Point
            // Read Position
            for ( int index = 0; index < polygonVertexCount; ++index )
            {
                PVertex outMeshVertex = Property::CreateProperty< PVertex > ();

//                 outMeshVertex->Position.x () = controlPoint[ index ][ 0 ];
//                 outMeshVertex->Position.y () = controlPoint[ index ][ 1 ];
//                 outMeshVertex->Position.z () = controlPoint[ index ][ 2 ];
//                 outMeshVertex->Position.w () = controlPoint[ index ][ 3 ];

                outMeshVertex->VertexControlPoint = inoutMesh->ControlPointList[ index ];

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
                    outMeshVertex->Normal.x () = normal[ 0 ];
                    outMeshVertex->Normal.y () = normal[ 1 ];
                    outMeshVertex->Normal.z () = normal[ 2 ];
                    outMeshVertex->Normal.w () = normal[ 3 ];
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
                    outMeshVertex->UV.x () = uv[ 0 ];
                    outMeshVertex->UV.y () = uv[ 1 ];
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
                    outMeshVertex->Color.x () = color[ 0 ];
                    outMeshVertex->Color.y () = color[ 1 ];
                    outMeshVertex->Color.z () = color[ 2 ];
                    outMeshVertex->Color.w () = color[ 3 ];
                }

                inoutMesh->VertexList[ index ] = outMeshVertex;
            }          
        }
        else
        {
            // By polygon Vertex
            int vertexCounter = 0;
            for ( auto polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex )
            {
                PVertex outMeshVertex = Property::CreateProperty< PVertex > ();

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

                    // Read Indices
                    inoutMesh->VertexIndexList[ lIndexOffset + triganleVertexIndex ] = static_cast<unsigned int>( vertexCounter );

                    // Read Position
                    outMeshVertex->VertexControlPoint = inoutMesh->ControlPointList[ controlPointIndex ];

//                     outMeshVertex->Position.x () = vertex[ 0 ];
//                     outMeshVertex->Position.y () = vertex[ 1 ];
//                     outMeshVertex->Position.z () = vertex[ 2 ];
//                     outMeshVertex->Position.w () = vertex[ 3 ];


                    // Read Normal
         
                    if ( hasNormal )
                    {
                        FbxVector4 normal;
                        mesh->GetPolygonVertexNormal ( polygonIndex, triganleVertexIndex, normal );
                        outMeshVertex->Normal.x () = normal[ 0 ];
                        outMeshVertex->Normal.y () = normal[ 1 ];
                        outMeshVertex->Normal.z () = normal[ 2 ];
                        outMeshVertex->Normal.w () = normal[ 3 ];
                    }
                    
                    // Read UV
                    if ( hasUV )
                    {
                        FbxVector2 uv;
                        const char *uvName = NULL;
                        bool isUnmappedUV;
                        mesh->GetPolygonVertexUV ( polygonIndex, triganleVertexIndex, uvName, uv, isUnmappedUV );
                        outMeshVertex->UV.x () = uv[ 0 ];
                        outMeshVertex->UV.y () = uv[ 1 ];
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
                        outMeshVertex->Color.x () = color[ 0 ];
                        outMeshVertex->Color.y () = color[ 1 ];
                        outMeshVertex->Color.z () = color[ 2 ];
                        outMeshVertex->Color.w () = color[ 3 ];
                    }
                    inoutMesh->VertexList[ vertexCounter ] = outMeshVertex;
                    ++vertexCounter;
                }
                mSubMeshes[ lMaterialIndex ]->TriangleCount += 1;
            }
        }     
    }

    void FBXLoader::ProcessMeshJoints ( FbxNode *node, PMesh &inoutMesh )
    {
        FbxMesh *mesh = node->GetMesh ();
        auto deformerCount = mesh->GetDeformerCount ();

        FbxAMatrix geometryTransformAMatrix = GetGeometryTransformation ( node );
        FbxMatrix geometryTransformMatrix { geometryTransformAMatrix };
        FbxMatrix2Float44 ( geometryTransformMatrix, inoutMesh->GeometryTransformMatrix );

        // Now We only support one deformer per mesh
        for ( auto deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex )
        {
            FbxSkin *skin = reinterpret_cast< FbxSkin * > ( mesh->GetDeformer ( deformerIndex, FbxDeformer::eSkin ) );
            if ( skin == nullptr )
            {
                continue;
            }

            auto nClusters = skin->GetClusterCount ();

            if ( nClusters > 0 )
            {
                FbxAMatrix transformAMatrix;
                FbxCluster *cluster = skin->GetCluster ( 0 );
                cluster->GetTransformMatrix ( transformAMatrix );
                FbxMatrix transformMatrix { transformAMatrix };
                FbxMatrix2Float44 ( transformMatrix, inoutMesh->Mesh2WorldMatrix );
            }

            for ( auto clusterIndex = 0; clusterIndex < nClusters; ++clusterIndex )
            {
                PJoint joint = Property::CreateProperty< PJoint > ();
                auto jointIndex = inoutMesh->JointList.size ();

                FbxCluster *cluster = skin->GetCluster ( clusterIndex );
                std::string jointName = cluster->GetLink ()->GetName ();

                FbxAMatrix joint2WorldBindPoseAMatrix;
                cluster->GetTransformLinkMatrix ( joint2WorldBindPoseAMatrix );
                
                auto nIndices = cluster->GetControlPointIndicesCount ();

                for ( int i = 0; i < nIndices; ++i )
                {
                    std::pair< U32, I32 > indexWeightPair;
                    indexWeightPair.first = jointIndex;
                    indexWeightPair.second = cluster->GetControlPointWeights ()[ i ];
                    inoutMesh->ControlPointList[ cluster->GetControlPointIndices ()[ i ] ]->JointIndexWeightPairList.push_back ( indexWeightPair );
                }

                joint->Name     = jointName;
                FbxMatrix2Float44 ( joint2WorldBindPoseAMatrix, joint->Joint2WorldBindPoseMatrix );

                inoutMesh->JointList.push_back ( joint );
            }

            // Set All Control Point Joint-Weight Pair Num to ControlPoint::VERTEX_LINK_JOINT_MAX_NUM
            // Fill With Zero element if not enough
            // Remove element if over ControlPoint::VERTEX_LINK_JOINT_MAX_NUM
            for ( auto &controlPoint : inoutMesh->ControlPointList )
            {
                if ( controlPoint->JointIndexWeightPairList.size () < ControlPoint::VERTEX_LINK_JOINT_MAX_NUM )
                {
                    for ( int i = 0; i < ControlPoint::VERTEX_LINK_JOINT_MAX_NUM - controlPoint->JointIndexWeightPairList.size (); ++i )
                    {
                        controlPoint->JointIndexWeightPairList.push_back ( std::make_pair ( 0, 0.0f ) );
                    }
                }
                else if ( controlPoint->JointIndexWeightPairList.size () > ControlPoint::VERTEX_LINK_JOINT_MAX_NUM )
                {
                    controlPoint->JointIndexWeightPairList.resize ( ControlPoint::VERTEX_LINK_JOINT_MAX_NUM );
                }
            }
            break;
        }
    }
}