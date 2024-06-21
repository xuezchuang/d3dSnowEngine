#include "ThirdPartyLibrary/FBX/include/fbxsdk.h"
#include "FBXAssist.h"

//#pragma comment(lib, "libfbxsdk.lib")
//#pragma comment(lib, "libfbxsdk-md.lib")
//#pragma comment(lib, "libfbxsdk-mt.lib")

//FBX基础API
void InitializeSdkObjects(class FbxManager*& InManager, FbxScene*& InScene);
bool LoadScene(FbxManager* InManager, FbxDocument* InScene, const char* InFilename);
void DestroySdkObjects(FbxManager* InManager);

//和模型相关
void RecursiveLoadMesh(FbxNode* InNode, FIERenderData& OutData, const FIEParam& InParam);
void GetMesh(FbxNode* InNode, FIEModel& InModel, const FIEParam& InParam);
void GetPolygons(FbxMesh* InMesh, FIEMesh& OutData);

//材质相关
void LinkMaterial(FbxGeometry* InGeometry, std::map<int, FIEMaterial>& OutMap);
void LinkMaterialTexture(FbxGeometry* InGeometry,std::map<int, FIEMaterial>& OutMap);
void MaterialMappings(FbxMesh* InGeometry, FIEMesh& OutMesh);

//骨骼动画相关
void LinkSkinnedWidget(FbxMesh* InGeometry, FIEBones& OutBones);
//获取骨骼动画
void SkeletonAnimation(FbxScene* pScene, FIERenderData& InRenderData);
//提取多个动画序列
void StackAnimation(FbxAnimStack* InAnimStack, FbxNode* InNode, FIEAnimationData& OutAnimationData, bool bSwitcher = false);
//提取单个动画序列
void StackAnimation(FbxAnimLayer* InAnimLayer, FbxNode* InNode, FIEAnimationSequence& OutAnimationSequence, bool isSwitcher = false);
//按曲线通道提取
void StackChannels(FbxAnimLayer* InAnimLayer, FbxNode* InNode, FIEBoneAnimation& OutAnimationSequence, bool isSwitcher);
//提取曲线
void GetCurveKeys(FbxAnimCurve* InCurve, vector<FIEKeyValue>& OutValues);
//拿到最初姿势方面的信息
void SkeletonMeshMatrixPose(FbxScene* InScene, FIERenderData& InRenderData);

//转为四元数
void EulerToQuaternion(FIERenderData& InRenderData);

//读取骨骼层次结构
void BoneHierarchy(FbxScene* InScene, FIERenderData& InRenderData);

namespace IEAssist
{
	void FbxAMatrixToMatrix4x4(const FbxDouble4x4 *InFBXMatrix,fmatrix_4x4 &OutMatrix)
	{
		OutMatrix.m11 = InFBXMatrix->Buffer()[0][0];
		OutMatrix.m12 = InFBXMatrix->Buffer()[0][1];
		OutMatrix.m13 = InFBXMatrix->Buffer()[0][2];
		OutMatrix.m14 = InFBXMatrix->Buffer()[0][3];

		OutMatrix.m21 = InFBXMatrix->Buffer()[1][0];
		OutMatrix.m22 = InFBXMatrix->Buffer()[1][1];
		OutMatrix.m23 = InFBXMatrix->Buffer()[1][2];
		OutMatrix.m24 = InFBXMatrix->Buffer()[1][3];

		OutMatrix.m31 = InFBXMatrix->Buffer()[2][0];
		OutMatrix.m32 = InFBXMatrix->Buffer()[2][1];
		OutMatrix.m33 = InFBXMatrix->Buffer()[2][2];
		OutMatrix.m34 = InFBXMatrix->Buffer()[2][3];

		OutMatrix.m41 = InFBXMatrix->Buffer()[3][0];
		OutMatrix.m42 = InFBXMatrix->Buffer()[3][1];
		OutMatrix.m43 = InFBXMatrix->Buffer()[3][2];
		OutMatrix.m44 = InFBXMatrix->Buffer()[3][3];
	}
}

/// <summary>
/// 具体实现
/// </summary>
/// <param name="InManager"></param>
/// <param name="InScene"></param>
/// 
void InitializeSdkObjects(FbxManager*& InManager, FbxScene*& InScene)
{
	InManager = FbxManager::Create();

	FbxIOSettings* FBXIO = FbxIOSettings::Create(InManager, IOSROOT);
	InManager->SetIOSettings(FBXIO);

	FbxString FBXPath = FbxGetApplicationDirectory();
	InManager->LoadPluginsDirectory(FBXPath);

	InScene = FbxScene::Create(InManager, "OK My");
}

bool LoadScene(FbxManager* InManager, FbxDocument* InScene, const char* InFilename)
{
	FFBXVersion SDKVersion;
	FbxManager::GetFileFormatVersion(
		SDKVersion.Major,
		SDKVersion.Minor,
		SDKVersion.Revision);

	FbxImporter* FBXImporterPtr = FbxImporter::Create(InManager, "");

	bool bReturn = FBXImporterPtr->Initialize(
		InFilename,
		-1,
		InManager->GetIOSettings());

	FFBXVersion FileVersion;
	FBXImporterPtr->GetFileVersion(
		FileVersion.Major,
		FileVersion.Minor,
		FileVersion.Revision);

	if (!bReturn)
	{
		return false;
	}

	if (FBXImporterPtr->IsFBX())
	{
		//打印数据
	}

	bReturn = FBXImporterPtr->Import(InScene);
	if (bReturn &&
		FBXImporterPtr->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		//密码
	}

	//删除
	FBXImporterPtr->Destroy();

	return true;
}

void GetPolygons(FbxMesh* InMesh, FIEMesh& OutData, const FIEParam& InParam)
{
	int PolygonCount = InMesh->GetPolygonCount();
	FbxVector4* ControlPoints = InMesh->GetControlPoints();

	OutData.VertexData.resize(PolygonCount);
	OutData.FBXDefaultIndexData.resize(PolygonCount * 3);

	int VertexID = 0;
	for (int i = 0; i < PolygonCount; i++)//图元
	{
		FIETriangle& InTriangle = OutData.VertexData[i];

		int PolygonSize = InMesh->GetPolygonSize(i);
		for (int j = 0; j < PolygonSize; j++)
		{
			int ControlPointIndex = InMesh->GetPolygonVertex(i, j);
			OutData.FBXDefaultIndexData[i * PolygonSize + j] = ControlPointIndex;

			int Indexs[3] = { 0,1,2 };
			int CoordinateZSymbol = 1;
			if (!InParam.bOriginalCoordinate)
			{
				Indexs[1] = 2;
				Indexs[2] = 1;

				CoordinateZSymbol = -1;
			}

			//Coordinates
			//拿到位置
			//FbxVector4 Coordinates;
			//获取位置
			{
				//考虑缩放
				FbxDouble3 Scaling = InMesh->GetNode()->LclScaling;

				InTriangle.Vertexs[j].Position.x = ControlPoints[ControlPointIndex].mData[Indexs[0]] * Scaling[Indexs[0]];
				InTriangle.Vertexs[j].Position.y = ControlPoints[ControlPointIndex].mData[Indexs[1]] * Scaling[Indexs[1]];
				InTriangle.Vertexs[j].Position.z = CoordinateZSymbol * ControlPoints[ControlPointIndex].mData[Indexs[2]] * Scaling[Indexs[2]];
			}

			//顶点颜色
			for (int l = 0; l < InMesh->GetElementVertexColorCount(); l++) {}

			//UV
			for (int l = 0; l < InMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* TextureUV = InMesh->GetElementUV(l);
				auto ModeType = TextureUV->GetMappingMode();

				auto ReferenceMode = TextureUV->GetReferenceMode();
				if (ModeType == fbxsdk::FbxLayerElement::eByControlPoint)
				{
					if (ReferenceMode == fbxsdk::FbxLayerElement::eDirect)
					{
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ControlPointIndex);

						InTriangle.Vertexs[j].UV.x = UV.mData[0];
						InTriangle.Vertexs[j].UV.y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
					}
					else if (ReferenceMode == fbxsdk::FbxLayerElement::eIndexToDirect)
					{
						int ID = TextureUV->GetIndexArray().GetAt(ControlPointIndex);

						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].UV.x = UV.mData[0];
						InTriangle.Vertexs[j].UV.y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
					}
				}
				else if (ModeType == fbxsdk::FbxLayerElement::eByPolygonVertex)
				{
					int TextureUVIndex = InMesh->GetTextureUVIndex(i, j);
					FbxVector2 UV = TextureUV->GetDirectArray().GetAt(TextureUVIndex);
					switch (ReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ControlPointIndex);

						InTriangle.Vertexs[j].UV.x = UV.mData[0];
						InTriangle.Vertexs[j].UV.y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
						break;
					}
					}
				}
			}

			//法线
			for (int l = 0; l < InMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* Normal = InMesh->GetElementNormal(l);
				auto NormalReferenceMode = Normal->GetReferenceMode();

				if (Normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (NormalReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(VertexID);

						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];

						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int ID = Normal->GetIndexArray().GetAt(VertexID);

						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];
					}
					}
				}
				else if (Normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (NormalReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ControlPointIndex);
						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];
						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int ID = Normal->GetIndexArray().GetAt(ControlPointIndex);
						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];
						break;
					}
					}
				}
			}

			//切线 T
			for (int l = 0; l < InMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* Tangent = InMesh->GetElementTangent(l);

				if (Tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (Tangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						FbxVector4 TangentValue = Tangent->GetDirectArray().GetAt(VertexID);

						InTriangle.Vertexs[j].Tangent.x = TangentValue.mData[Indexs[0]];
						InTriangle.Vertexs[j].Tangent.y = TangentValue.mData[Indexs[1]];
						InTriangle.Vertexs[j].Tangent.z = TangentValue.mData[Indexs[2]];
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int ID = Tangent->GetIndexArray().GetAt(VertexID);

						FbxVector4 TangentValue = Tangent->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Tangent.x = TangentValue.mData[Indexs[0]];
						InTriangle.Vertexs[j].Tangent.y = TangentValue.mData[Indexs[1]];
						InTriangle.Vertexs[j].Tangent.z = TangentValue.mData[Indexs[2]];
						break;
					}
					}
				}
			}

			//B
			for (int l = 0; l < InMesh->GetElementBinormalCount(); ++l)
			{
				FbxGeometryElementBinormal* Binormal = InMesh->GetElementBinormal(l);

				if (Binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (Binormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						Binormal->GetDirectArray().GetAt(VertexID);
						break;
					}
					}
				}
			}

			VertexID++;
		}
	}
}

void GetIndex(FIEMesh& InMesh)
{
	uint16_t VertexDataSize = InMesh.VertexData.size() * 3.f;
	InMesh.IndexData.resize(InMesh.VertexData.size() * 3.f);

	for (uint16_t i = 0; i < VertexDataSize; i++)
	{
		InMesh.IndexData[i] = i;
	}
}

void GetMesh(FbxNode* InNode, FIEModel& InModel, const FIEParam& InParam)
{
	FbxMesh* NodeMesh = (FbxMesh*)InNode->GetNodeAttribute();

	InModel.MeshData.push_back(FIEMesh());
	FIEMesh& InMesh = InModel.MeshData[InModel.MeshData.size() - 1];

	GetPolygons(NodeMesh, InMesh, InParam);

	GetIndex(InMesh);

	LinkMaterial(NodeMesh,InModel.MaterialMap);

	LinkMaterialTexture(NodeMesh, InModel.MaterialMap);

	MaterialMappings(NodeMesh,InMesh);

	LinkSkinnedWidget(NodeMesh, InModel.Skeleton.Bones);
}

void RecursiveLoadMesh(FbxNode* InNode, FIERenderData& OutData, const FIEParam& InParam)
{
	//XML
	if (InNode->GetNodeAttribute() == NULL)
	{
		//NULL Node
	}
	else
	{
		FbxNodeAttribute::EType AttributeType = (InNode->GetNodeAttribute()->GetAttributeType());

		if (AttributeType == fbxsdk::FbxNodeAttribute::eMesh)
		{
			OutData.ModelData.push_back(FIEModel());
			FIEModel& InModel = OutData.ModelData[OutData.ModelData.size() - 1];
			GetMesh(InNode, InModel, InParam);
		}
		else if (AttributeType == fbxsdk::FbxNodeAttribute::eSkeleton)
		{

		}
	}

	for (int i = 0; i < InNode->GetChildCount(); i++)
	{
		RecursiveLoadMesh(InNode->GetChild(i), OutData, InParam);
	}
}

void DestroySdkObjects(FbxManager* InManager)
{
	if (InManager)
	{
		InManager->Destroy();
	}
}

void LinkMaterial(FbxGeometry *InGeometry ,std::map<int, FIEMaterial>& OutMap)
{
	int MaterialCount = 0;
	FbxNode* Node = NULL;

	if (InGeometry)
	{
		Node = InGeometry->GetNode();
		if (Node)
		{
			MaterialCount = Node->GetMaterialCount();
		}
	}

	if (MaterialCount > 0)
	{
		FbxPropertyT<FbxDouble3> KFbxDouble3;
		FbxPropertyT<FbxDouble> KFbxDouble1;
		FbxColor TheColor;

		for (size_t i = 0; i < MaterialCount; i++)
		{
			OutMap.insert({i,FIEMaterial()});
			FIEMaterial &InMap = OutMap[i];

			FbxSurfaceMaterial *FBXMaterialInst = Node->GetMaterial(i);

			//材质名字
			InMap.Name = FBXMaterialInst->GetName();

			if (FBXMaterialInst->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				KFbxDouble3 = ((FbxSurfacePhong*)FBXMaterialInst)->Ambient;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Ambient.Update(&TheColor);

				KFbxDouble3 = ((FbxSurfacePhong*)FBXMaterialInst)->Diffuse;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.DiffuseColor.Update(&TheColor);

				KFbxDouble3 = ((FbxSurfacePhong*)FBXMaterialInst)->Specular;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Specular.Update(&TheColor);

				KFbxDouble3 = ((FbxSurfacePhong*)FBXMaterialInst)->Emissive;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Emissive.Update(&TheColor);

				KFbxDouble1 = ((FbxSurfacePhong*)FBXMaterialInst)->TransparencyFactor;
				auto Value = 1.0 - KFbxDouble1.Get();
				InMap.Opacity.Update(&Value);

				KFbxDouble1 = ((FbxSurfacePhong*)FBXMaterialInst)->Shininess;
				Value = KFbxDouble1.Get();
				InMap.Shininess.Update(&Value);

				KFbxDouble1 = ((FbxSurfacePhong*)FBXMaterialInst)->ReflectionFactor;
				Value = KFbxDouble1.Get();
				InMap.Reflectivity.Update(&Value);
			}
			else if (FBXMaterialInst->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				KFbxDouble3 = ((FbxSurfaceLambert*)FBXMaterialInst)->Ambient;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Ambient.Update(&TheColor);

				KFbxDouble3 = ((FbxSurfaceLambert*)FBXMaterialInst)->Diffuse;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.DiffuseColor.Update(&TheColor);

				KFbxDouble3 = ((FbxSurfaceLambert*)FBXMaterialInst)->Emissive;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Emissive.Update(&TheColor);

				KFbxDouble1 = ((FbxSurfaceLambert*)FBXMaterialInst)->TransparencyFactor;
				auto Value = 1.0 - KFbxDouble1.Get();
				InMap.Opacity.Update(&Value);
			}
			else
			{
				//"Unknown type of Material";
			}

			//Shading类型
			InMap.ShadingModel = FBXMaterialInst->ShadingModel.Get().Buffer();
		}
	}
}

void MaterialMappings(FbxMesh* InFbxMesh, FIEMesh& OutMesh)
{
	int MaterialCount = 0;
	FbxNode* Node = NULL;

	if (InFbxMesh)
	{
		Node = InFbxMesh->GetNode();
		if (Node)
		{
			MaterialCount = Node->GetMaterialCount();
		}
	}

	for (size_t i = 0; i < InFbxMesh->GetElementMaterialCount(); i++)
	{
		if (FbxGeometryElementMaterial* FBXGElementMaterial = InFbxMesh->GetElementMaterial(i))
		{
			int IndexArrayCount = FBXGElementMaterial->GetIndexArray().GetCount();
			for (size_t j = 0; j < IndexArrayCount; j++)
			{
				//材质ID
				auto ValueIndex = FBXGElementMaterial->GetIndexArray().GetAt(j);
				OutMesh.VertexData[j].MaterialID = ValueIndex;
			}
		}
	}
}

void LinkSkinnedWidget(FbxMesh* InGeometry, FIEBones& OutBones)
{
	int SkinCount = InGeometry->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < SkinCount; i++)
	{
		int ClusterCount = ((FbxSkin*)InGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		for (int j = 0; j < ClusterCount; j++)
		{
			if (FbxCluster* Cluster = ((FbxSkin*)InGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j))
			{
				OutBones.push_back(FIEBone());
				FIEBone& InBone = OutBones[OutBones.size() - 1];

				if (Cluster->GetLink() != NULL)
				{
					InBone.Name = (char*)Cluster->GetLink()->GetName();
				}

				int* Indices = Cluster->GetControlPointIndices();
				double* Weights = Cluster->GetControlPointWeights();

				int IndexCount = Cluster->GetControlPointIndicesCount();
				for (int k = 0; k < IndexCount; k++)
				{
					InBone.Widgets.push_back(FIESkinnedWidget(Indices[k], (float)Weights[k], j));
				}

				FbxAMatrix Matrix;
				Matrix = Cluster->GetTransformMatrix(Matrix);
			
				IEAssist::FbxAMatrixToMatrix4x4(&Matrix,InBone.BoneMatrix);
			}
		}
	}
}

void SkeletonAnimation(FbxScene* InScene, FIERenderData& InRenderData)
{
	for (int i = 0; i < InScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FIEModel* Model = &InRenderData.ModelData[i];

		FbxAnimStack* AnimStack = InScene->GetSrcObject<FbxAnimStack>(i);

		StackAnimation(AnimStack,InScene->GetRootNode(),Model->AnimationData);
	}
}

void StackAnimation(FbxAnimStack* InAnimStack, FbxNode* InNode, FIEAnimationData& OutAnimationData, bool bSwitcher)
{
	int AnimLayersNumber = InAnimStack->GetMemberCount<FbxAnimLayer>();

	for (int l = 0; l < AnimLayersNumber; l++)
	{
		FbxAnimLayer* AnimLayer = InAnimStack->GetMember<FbxAnimLayer>(l);

		FIEAnimationSequence& AnimationSequence = OutAnimationData.AnimationSequences[InAnimStack->GetName()];
	
		StackAnimation(AnimLayer, InNode, AnimationSequence);
	}
}

void StackAnimation(FbxAnimLayer* InAnimLayer, FbxNode* InNode, FIEAnimationSequence& OutAnimationSequence, bool bSwitcher)
{
	if (InNode->GetNodeAttribute())
	{
		FbxNodeAttribute::EType AttributeType = InNode->GetNodeAttribute()->GetAttributeType();
		if (AttributeType == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			OutAnimationSequence.push_back(FIEBoneAnimation());
			FIEBoneAnimation& InBoneAnimation = OutAnimationSequence[OutAnimationSequence.size() - 1];
			
			InBoneAnimation.Name = InNode->GetName();;

			StackChannels(InAnimLayer, InNode, InBoneAnimation, bSwitcher);
		}
	}

	for (int i = 0; i < InNode->GetChildCount(); i++)
	{
		StackAnimation(InAnimLayer, InNode->GetChild(i),OutAnimationSequence,bSwitcher);
	}
}

void StackChannels(FbxAnimLayer* InAnimLayer, FbxNode* InNode, FIEBoneAnimation& OutAnimationSequence, bool bSwitcher)
{
	if (!bSwitcher)
	{
		auto GetAnimCurveKey =[&](FbxAnimCurve* AnimCurve,std::function<void(const FIEKeyValue&,int)> Fun)
		{
			if (AnimCurve)
			{
				int KeyCount = AnimCurve->KeyGetCount();
				if (OutAnimationSequence.KeyAnim.size() == 0)
				{
					OutAnimationSequence.KeyAnim.resize(KeyCount);
				}
				
				vector<FIEKeyValue> Values;
				GetCurveKeys(AnimCurve, Values);

				for (int i = 0; i < OutAnimationSequence.KeyAnim.size(); i++)
				{
					Fun(Values[i],i);
				}
			}
		};
		
		//Translation.x
		GetAnimCurveKey(
		InNode->LclTranslation.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_X),
		[&](const FIEKeyValue &InKeyValue,int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Translation.x = InKeyValue.Value;
		});

		//Translation.y
		GetAnimCurveKey(
		InNode->LclTranslation.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Translation.y = InKeyValue.Value;
		});

		//Translation.Z
		GetAnimCurveKey(
		InNode->LclTranslation.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Translation.z = InKeyValue.Value;
		});

		//Rotator.x
		GetAnimCurveKey(
		InNode->LclRotation.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_X),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Rotation.x = InKeyValue.Value;
		});

		//Rotator.y
		GetAnimCurveKey(
		InNode->LclRotation.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Rotation.y = InKeyValue.Value;
		});

		//Rotator.z
		GetAnimCurveKey(
		InNode->LclRotation.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Rotation.z = InKeyValue.Value;
		});

		//Scale.x
		GetAnimCurveKey(
		InNode->LclScaling.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_X),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Scale.x = InKeyValue.Value;
		});

		//Scale.y
		GetAnimCurveKey(
		InNode->LclScaling.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Scale.y = InKeyValue.Value;
		});

		//Scale.z
		GetAnimCurveKey(
		InNode->LclScaling.GetCurve(InAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		[&](const FIEKeyValue& InKeyValue, int Index)
		{
			OutAnimationSequence.KeyAnim[Index].KeyTime = InKeyValue.Time;
			OutAnimationSequence.KeyAnim[Index].Scale.z = InKeyValue.Value;
		});
	}
}

void GetCurveKeys(FbxAnimCurve* InCurve, vector<FIEKeyValue>& OutValues)
{
	OutValues.clear();
	int KeyCount = InCurve->KeyGetCount();

	for (int i = 0; i < KeyCount; i++)
	{
		OutValues.push_back(FIEKeyValue());
		FIEKeyValue& InValue = OutValues[OutValues.size() - 1];

		InValue.Time = InCurve->KeyGetTime(i).GetSecondDouble();
		InValue.Value = static_cast<float>(InCurve->KeyGetValue(i));;
	}
}

void SkeletonMeshMatrixPose(FbxScene* InScene, FIERenderData& InRenderData)
{
	FIEModel* Model = NULL;
	if (InRenderData.ModelData.size() > 0)
	{
		Model = &InRenderData.ModelData[0];
	};

	int PoseCount = InScene->GetPoseCount();
	for (int i = 0; i < PoseCount; i++)
	{
		FbxPose* Pose = InScene->GetPose(i);

		for (int j = 0; j < Pose->GetCount(); j++)
		{
			FbxString Name = Pose->GetNodeName(j).GetCurrentName();

			FIEBone* Bone = NULL;
			for (auto &Tmp : Model->Skeleton.Bones)
			{
				if (Tmp.Name == Name.Buffer())
				{
					Bone = &Tmp;
				}
			}

			fmatrix_4x4* MyMatrixPtr = nullptr;
			//骨骼方面的赋值
			if (Bone)
			{
				Bone->bBindPose = Pose->IsBindPose();
				if (!Bone->bBindPose)
				{
					Bone->bLocalMatrix = Pose->IsLocalMatrix(j);
				}

				MyMatrixPtr = &Bone->BoneMatrix;
			}
			else
			{
				Model->Name = Name.Buffer();
				Model->bBindPose = Pose->IsBindPose();
				if (Model->bBindPose)
				{
					Model->bLocalMatrix = Pose->IsLocalMatrix(j);
				}

				MyMatrixPtr = &Model->Matrix;			
			}

			if (MyMatrixPtr)
			{
				int Index = 0;
				for (int k = 0; k < 4; k++)
				{
					FbxMatrix Matrix = Pose->GetMatrix(j);
					IEAssist::FbxAMatrixToMatrix4x4(&Matrix,*MyMatrixPtr);
				}
			}
		}
	}
}

void EulerToQuaternion(FIERenderData& InRenderData)
{
	for (auto& Tmp : InRenderData.ModelData)
	{
		for (auto& TmpSequences : Tmp.AnimationData.AnimationSequences)
		{
			for (auto& TmpAnim : TmpSequences.second)
			{
				for (auto& KeyTmp : TmpAnim.KeyAnim)
				{
					frotator Rot(KeyTmp.Rotation.x, KeyTmp.Rotation.z, KeyTmp.Rotation.y);

					fquat Quat;
					Quat.object_to_inertia(Rot);

					memcpy(&KeyTmp.Rotation, &Quat, sizeof(fquat));
				}
			}
		}
	}
}

int GetBoneIndexByNme(const FIEBones& Bones, const std::string& InName)
{
	if (!InName.empty())
	{
		for (size_t i = 0; i < Bones.size(); i++)
		{
			if (Bones[i].Name == InName)
			{
				return i;
			}
		}
	}

	return -1;
}

void BoneHierarchy(FbxNode* InNode, const string& InParent, int InDepth, FIESkeleton* Skeleton)
{
	if (InNode)
	{
		FbxNodeAttribute::EType InType = InNode->GetNodeAttribute()->GetAttributeType();

		if (InType == FbxNodeAttribute::EType::eSkeleton)
		{
			//获取上层级的Index
			int ParentIndex = GetBoneIndexByNme(Skeleton->Bones, InParent);

			FbxString MyFBXNme = InNode->GetName();
			std::string MyNme = MyFBXNme.Buffer();

			//获取当前我的位置
			int Index = GetBoneIndexByNme(Skeleton->Bones, MyNme);
			Skeleton->BoneInherits[Index] = ParentIndex;

			int ChildCount = InNode->GetChildCount();
			for (int i = 0; i < ChildCount; i++)
			{
				BoneHierarchy(InNode->GetChild(i),MyNme,InDepth + 1, Skeleton);
			}
		}
	}
}

void BoneHierarchy(FbxScene* InScene, FIERenderData& InRenderData)
{
	if (InScene)
	{
		FIESkeleton* Skeleton = NULL;
		if (InRenderData.ModelData.size() > 0)
		{
			Skeleton = &InRenderData.ModelData[0].Skeleton;
		}

		if (Skeleton != nullptr)
		{
			if (Skeleton->Bones.size() != 0)
			{
				Skeleton->BoneInherits.resize(Skeleton->Bones.size());
				if (FbxNode* RootNode = InScene->GetRootNode())
				{
					int ChildCount = RootNode->GetChildCount();
					for (int i = 0; i < ChildCount; i++)
					{
						BoneHierarchy(RootNode->GetChild(i),"",0,Skeleton);
					}
				}
			}
		}
	}
}

void LinkMaterialTexture(FbxGeometry* InGeometry,std::map<int, FIEMaterial>& OutMap)
{
	int MaterialIndex = -1;
	FbxProperty Property;

	if (InGeometry->GetNode() == NULL)
	{
		return;
	}

	int SurfaceNumber = InGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (MaterialIndex = 0; MaterialIndex < SurfaceNumber; MaterialIndex++)
	{
		if (FbxSurfaceMaterial* InSurfaceMaterial = InGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(MaterialIndex))
		{
			FIEMaterial &InMaterial = OutMap[MaterialIndex];

			int i = 0;
			FBXSDK_FOR_EACH_TEXTURE(i)
			{
				const char* FindName = FbxLayerElement::sTextureChannelNames[i];
				Property = InSurfaceMaterial->FindProperty(FindName);

				if (Property.IsValid())
				{
					int TextureCount = Property.GetSrcObjectCount<FbxTexture>();
					if (FIEPropertyObject* InProperty = InMaterial.Find(FindName))
					{
						for (int j = 0; j < TextureCount; j++)
						{
							//分层纹理
							if (FbxLayeredTexture* LayeredTexture = Property.GetSrcObject<FbxLayeredTexture>(j))
							{
								//分层纹理
							}
							else
							{
								if (FbxTexture* Texture = Property.GetSrcObject<FbxTexture>(j))
								{
									InProperty->TextureName = Texture->GetName();
								
									if (FbxFileTexture* FileTexture = FbxCast<FbxFileTexture>(Texture))
									{
										InProperty->TextureType = "File Texture";
										InProperty->TextureFilename = FileTexture->GetFileName();
									}
									else if(FbxProceduralTexture* ProceduralTexture = FbxCast<FbxProceduralTexture>(Texture))
									{
										InProperty->TextureType = "Procedural Texture";
									}

									//Texture->GetScaleU();
									//Texture->GetScaleV();

									//Texture->GetTranslationU();
									//Texture->GetTranslationV();

									//Texture->GetSwapUV();

									//Texture->GetRotationU();
									//Texture->GetRotationV();
									//Texture->GetRotationW();

									//Texture->GetCroppingLeft();
									//Texture->GetCroppingTop();
									//Texture->GetCroppingRight();
									//Texture->GetCroppingBottom();
								}
							}
						}
					}
				}
			}
		}
	}
}

namespace FBXAssetImport
{
	void LoadMeshData(const char* InPath, FIERenderData& OutData, const FIEParam& InParam)
	{
		//创建基础管理和场景
		FbxManager* SdkManager = NULL;
		FbxScene* Scene = NULL;

		//初始化场景对象
		InitializeSdkObjects(SdkManager, Scene);

		//读取FBX模型
		FbxString FBXPath(InPath);
		bool bResult = LoadScene(SdkManager, Scene, FBXPath.Buffer());

		//FbxAxisSystem::DirectX.ConvertScene(Scene);

		//XML 解析我们的FBX
		if (FbxNode* Node = Scene->GetRootNode())
		{
			for (int i = 0; i < Node->GetChildCount(); i++)
			{
				RecursiveLoadMesh(Node->GetChild(i), OutData, InParam);
			}
		}
		//拿到姿势
		SkeletonMeshMatrixPose(Scene, OutData);

		//获取骨骼的层次结构
		BoneHierarchy(Scene, OutData);

		//获取骨骼动画
		SkeletonAnimation(Scene, OutData);

		//把欧拉角转为四元数
		EulerToQuaternion(OutData);

		//干掉
		DestroySdkObjects(SdkManager);
	}
}


//int main()
//{
//	//
//	FFBXRenderData Out;
//	std::string FBXPath = "P:/FBXTest/Heart.fbx";
//
//	FFBXAssetImport().LoadMeshData(FBXPath, Out);
//
//	return 0;
//}
//
