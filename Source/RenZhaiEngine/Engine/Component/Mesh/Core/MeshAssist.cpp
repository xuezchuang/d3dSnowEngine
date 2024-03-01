#include "MeshAssist.h"
#include "../../../Mesh/Core/Material/Material.h"
#include "../../../Core/Construction/MacroConstruction.h"
#include "MeshComponent.h"
#include "../SkinnedMeshComponent.h"
#include "../../../Animation/SkinnedAnimationInstance.h"
#include "../../../Math/EngineMath.h"
#include "../../../Animation/Core/AnimationSequence.h"

#if THIRD_PARTY_LIBRARY
#include "MeshImportExport.h"

#pragma comment(lib, "RenZhaiMeshImportExportTool.lib")

#endif // THIRD_PARTY_LIBRARY

template<class T>
inline bool FMeshAssist::LoadFBXFromBuff_Interior(
	CMeshComponent* InMeshComponent,
	const string& InPath,
	FRenderContent<T>& MeshData, 
	const FIEParam& InParam,
	std::function<void(const FIERenderData&)> ExtendFunction)
{
	if (!InMeshComponent)
	{
		return false;
	}

#if THIRD_PARTY_LIBRARY
	FIERenderData RenderData;
	AssetImport::LoadMeshData(InPath.c_str(), EIEMeshType::IE_FBX, RenderData, InParam);

	for (auto& TmpModel : RenderData.ModelData)
	{
		auto FindSection = [&](int InMatrealIndex)->FMeshSection*
		{
			for (auto& Tmp : MeshData.SectionDescribe)
			{
				if (Tmp.MatrealIndex == InMatrealIndex)
				{
					return &Tmp;
				}
			}

			MeshData.SectionDescribe.push_back(FMeshSection());
			FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

			Section.MatrealIndex = InMatrealIndex;

			return &Section;
		};

		//模型处理
		for (auto& MeshTmp : TmpModel.MeshData)
		{
			for (auto& VertexTmp : MeshTmp.VertexData)
			{
				//三角形赋值
				for (int i = 0; i < 3; i++)
				{
					MeshData.Data.VertexData.push_back(T());
					T& InVertex = MeshData.Data.VertexData[MeshData.Data.VertexData.size() - 1];

					InVertex.Position.x = VertexTmp.Vertexs[i].Position.x;
					InVertex.Position.y = VertexTmp.Vertexs[i].Position.y;
					InVertex.Position.z = VertexTmp.Vertexs[i].Position.z;

					InVertex.Normal.x = VertexTmp.Vertexs[i].Normal.x;
					InVertex.Normal.y = VertexTmp.Vertexs[i].Normal.y;
					InVertex.Normal.z = VertexTmp.Vertexs[i].Normal.z;

					InVertex.UTangent.x = VertexTmp.Vertexs[i].Tangent.x;
					InVertex.UTangent.y = VertexTmp.Vertexs[i].Tangent.y;
					InVertex.UTangent.z = VertexTmp.Vertexs[i].Tangent.z;

					InVertex.TexCoord.x = VertexTmp.Vertexs[i].UV.x;
					InVertex.TexCoord.y = VertexTmp.Vertexs[i].UV.y;
				}

				//Section
				if (FMeshSection* Section = FindSection(VertexTmp.MaterialID))
				{
					Section->IndexSize += 3;
					Section->VertexSize += 3;
				}
			}

			//拷贝
			MeshData.Data.IndexData.insert(
				MeshData.Data.IndexData.end(),
				MeshTmp.IndexData.begin(),
				MeshTmp.IndexData.end());
		}

		//多材质
		BUILD_OBJECT_PARAMETERS_BY_COMPONENT(,InMeshComponent);
		if (vector<CMaterial*>* InMaterials = InMeshComponent->GetMaterials())
		{
			for (auto& MaterialTmp : TmpModel.MaterialMap)
			{
				InMaterials->push_back(CreateObject<CMaterial>(Param, new CMaterial()));
				if (CMaterial* InMaterial = (*InMaterials)[InMaterials->size() - 1])
				{
					InMaterial->Rename(MaterialTmp.second.Name);

					auto& Diffuse = MaterialTmp.second.DiffuseColor;
					InMaterial->SetBaseColor(fvector_4d(Diffuse.Buffer()[0], Diffuse.Buffer()[1], Diffuse.Buffer()[2], 1.f));

					auto& Specular = MaterialTmp.second.Specular;
					InMaterial->SetSpecular(fvector_3d((float)Specular.Buffer()[0], (float)Specular.Buffer()[1], (float)Specular.Buffer()[2]));

					auto& Reflectivity = MaterialTmp.second.Reflectivity;
					InMaterial->SetRefractiveValue(*(float*)Reflectivity.Buffer());

					//shader设置
					if (MaterialTmp.second.ShadingModel == "Lambert")
					{
						InMaterial->SetMaterialType(EMaterialType::Lambert);
					}
					else if (MaterialTmp.second.ShadingModel == "Phong")
					{
						InMaterial->SetMaterialType(EMaterialType::Phong);
					}

					if (!MaterialTmp.second.DiffuseColor.TextureFilename.empty())
					{


						InMaterial->SetBaseColor(MaterialTmp.second.DiffuseColor.TextureFilename);
					}
				}
			}
		}
	}

	//为其他扩展准备
	ExtendFunction(RenderData);

#endif
	return true;
}

bool FMeshAssist::LoadFBXSkinnedMeshFromBuff(CMeshComponent* InMeshComponent, const string& InPath, FSkinnedMeshRenderingData& MeshData, const FIEParam& InParam)
{
	return LoadFBXFromBuff_Interior(InMeshComponent, InPath, MeshData, InParam,
	[&](const FIERenderData &InIERenderData)
	{
		if (CSkinnedMeshComponent* InSkinnedMeshComponent = dynamic_cast<CSkinnedMeshComponent*>(InMeshComponent))
		{
			if (CSkinnedAnimationInstance* AnimInstance = InSkinnedMeshComponent->GetAnimationInstance())
			{
				if (InIERenderData.ModelData.size() > 0)
				{
					for (auto& Tmp : InIERenderData.ModelData)
					{
						if (FSkinnedAnimationData* InSkinnedAnimationData = AnimInstance->GetAnimationData())
						{
							//提出骨骼和蒙皮信息
							for (int i = 0 ;i < Tmp.Skeleton.Bones.size();i++)
							{
								if (!InSkinnedAnimationData->Skeleton)
								{
									BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(,AnimInstance);
									InSkinnedAnimationData->Skeleton = CreateObject<CSkeleton>(Param, new CSkeleton());//NewObject;
								}

								//添加骨骼
								InSkinnedAnimationData->Skeleton->Bones.push_back(*(FBone*)&Tmp.Skeleton.Bones[i]);
								
								//骨骼层次结构
								InSkinnedAnimationData->Skeleton->BoneHierarchy = Tmp.Skeleton.BoneInherits;

								//权重
								for (auto& WidgetBone : Tmp.Skeleton.Bones[i].Widgets)
								{
									for (int k = 0; k < Tmp.MeshData[0].FBXDefaultIndexData.size(); k++)
									{
										if (Tmp.MeshData[0].FBXDefaultIndexData[k] == WidgetBone.LinkIndex)
										{
											FSkinnedVertex& VertexTmp = MeshData.Data.VertexData[k];
											float* InFloatPtr = (float*)&VertexTmp.BoneWeights;

											for (int j = 0; j < 4; j++)
											{
												if (VertexTmp.BoneIndices[j] == 0)
												{
													//BoneIndices
													VertexTmp.BoneIndices[j] = WidgetBone.LinkBoneIndex + 1;//默认 0为空

													//权重
													if (j < 3)
													{
														InFloatPtr[j] = WidgetBone.WeightValue;
													}

													break;
												}
											}

											break;
										}
									}
								}
							}

							//提出动画
							if (Tmp.AnimationData.AnimationSequences.size() > 0)
							{
								for (auto& Sequences : Tmp.AnimationData.AnimationSequences)
								{
									if (Sequences.first != "BaseLayer")
									{
										InSkinnedAnimationData->CurrentAnimationSequence = Sequences.first;

										BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(, AnimInstance);
										CAnimationSequence *InAnimationSequence = CreateObject<CAnimationSequence>(Param, new CAnimationSequence());
										
										InSkinnedAnimationData->Sequences.insert({Sequences.first,InAnimationSequence});
									
										InAnimationSequence->SequenceData = *(std::vector<FBoneAnimation>*)&Sequences.second;
									}
								}
							}
						}
					}
				}
			}
		}
	});
}

bool FMeshAssist::LoadFBXMeshFromBuff(CMeshComponent* InMeshComponent, const string& InPath, FMeshRenderingData& MeshData, const FIEParam& InParam)
{
	return LoadFBXFromBuff_Interior(InMeshComponent, InPath, MeshData, InParam,
	[](const FIERenderData &InIERenderData)
	{


	});
}
