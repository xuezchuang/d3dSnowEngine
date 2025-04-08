#include "CustomMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Core/Construction/MacroConstruction.h"
#include "Core/MeshAssist.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

CCustomMeshComponent::CCustomMeshComponent()
{

}

void CCustomMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData, 
	const string& InPath,
	const FIEParam& InParam)
{
	//C:/dd/dd/c/x.obj
	//x.obj
	char Buff[1024] = { 0 };
	get_path_clean_filename(Buff,InPath.c_str());

	if (find_string(Buff, ".obj", 0) != -1 || find_string(Buff, ".OBJ", 0) != -1)
	{
		if (LoadObj(InPath, MeshData))
			return;
			
		//拿到文件大小
		unsigned int FileSize = get_file_size_by_filename(InPath.c_str());

		//根据文件大小创建buff
		char* Buff = new char[FileSize + 1];
		//必须要初始化
		memset(Buff, 0, FileSize + 1);

		//提取buff
		get_file_buf(InPath.c_str(), Buff);

		if (!LoadObjFromBuff(Buff, FileSize, MeshData))
		{

		}

		delete Buff;
	}
	else if (find_string(Buff, ".fbx", 0) != -1 || find_string(Buff, ".FBX", 0) != -1)
	{
		char PathBuff[1024] = { 0 };
		get_full_path(PathBuff,1024, InPath.c_str());

		FMeshAssist::LoadFBXMeshFromBuff(this, InPath, MeshData, InParam);
	}
}

bool CCustomMeshComponent::LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData)
{
	if (InBuffSize > 0)
	{
		MeshData.SectionDescribe.push_back(FMeshSection());
		FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

		stringstream BuffStream(InBuff);
		char TmpLine[256] = { 0 };
		string MidTmpTag;

		vector<XMFLOAT3> Position;
		vector<XMFLOAT3> Normal;
		vector<XMFLOAT3> UTangent;
		vector<XMFLOAT2> TexCoord;//纹理坐标

		for (; !BuffStream.eof();)
		{
			memset(TmpLine, 0, 256);

			//读取一行数据
			BuffStream.getline(TmpLine, 256);
			if (strlen(TmpLine) > 0)
			{
				if (TmpLine[0] == 'v')
				{
					stringstream LineStream(TmpLine);
					LineStream >> MidTmpTag;

					if (TmpLine[1] == 'n')
					{
						//拿到位置
						Normal.push_back(XMFLOAT3());
						XMFLOAT3& Float3InNormal = Normal[Normal.size() - 1];

						//解析了位置
						LineStream >> Float3InNormal.x;
						LineStream >> Float3InNormal.y;
						LineStream >> Float3InNormal.z;
					}
					else if (TmpLine[1] == 't')
					{	
						TexCoord.push_back(XMFLOAT2());
						XMFLOAT2& Float2InTexCoord = TexCoord[TexCoord.size() - 1];

						LineStream >> Float2InTexCoord.x;
						LineStream >> Float2InTexCoord.y;
					}
					else
					{
						//拿到位置
						Position.push_back(XMFLOAT3());
						XMFLOAT3& Float3InPos = Position[Position.size() - 1];

						//解析了位置
						LineStream >> Float3InPos.x;
						LineStream >> Float3InPos.y;
						LineStream >> Float3InPos.z;
					}
				}
				else if (TmpLine[0] == 'f')
				{
					stringstream LineStream(TmpLine);
					LineStream >> MidTmpTag;

					char SaveLineString[256] = { 0 };
					char TmpBuff[256] = { 0 };
					for (size_t i = 0; i < 3; i++)
					{
						memset(SaveLineString, 0, 256);

						//输入一行数据
						LineStream >> SaveLineString;

						//找到索引的位置
						int StringPosA = find_string(SaveLineString, "/", 0);
						memset(TmpBuff, 0, 256);
						char* VPosIndex = string_mid(SaveLineString, TmpBuff, 0, StringPosA);

						//放到索引容器里面
						MeshData.Data.IndexData.push_back(atoi(VPosIndex) - 1);

						//纹理Index
						int StringPosB = find_string(SaveLineString, "/", StringPosA + 1);
						memset(TmpBuff, 0, 256);
						char* TexcoordIndex = string_mid(SaveLineString, TmpBuff, StringPosA + 1, StringPosB - (StringPosA + 1));

						//法线index
						memset(TmpBuff, 0, 256);
						char* NormalIndex = string_mid(SaveLineString, TmpBuff, StringPosB + 1, strlen(SaveLineString) - (StringPosB + 1));
					}
				}
			}
		}

		MeshData.Data.VertexData.resize((int)Position.size());
		for (size_t i = 0; i < Position.size(); i++)
		{
			MeshData.Data.VertexData[i].Position = Position[i];
			MeshData.Data.VertexData[i].Normal = Normal[i];
			MeshData.Data.VertexData[i].TexCoord = TexCoord[i];
			MeshData.Data.VertexData[i].Color = XMFLOAT4(Colors::White);

			//if (i > 1)
			//{
			//	XMFLOAT3 LastPos = Position[i - 1];
			//	XMFLOAT3 Pos = Position[i];
			//
			//	fvector_3d LastPos3D = EngineMath::ToVector3d(LastPos);
			//	fvector_3d PosVector3D = EngineMath::ToVector3d(Pos);
			//
			//	fvector_3d NewDir = LastPos3D - PosVector3D;
			//	MeshData.VertexData[i].UTangent = XMFLOAT3(NewDir.x, NewDir.y, NewDir.z);
			//}
			//else
			//{
			//	MeshData.VertexData[i].UTangent = XMFLOAT3(0.f, 1.f, 0.f);
			//}
		}

		Section.IndexSize = MeshData.Data.IndexData.size();
		Section.VertexSize = MeshData.Data.VertexData.size();

		SpawnDefaultMaterial();

		return true;
	}

	return false;
}

bool CCustomMeshComponent::LoadObj(const string& InBuff, FMeshRenderingData& MeshData)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(InBuff,
		aiProcess_Triangulate |        // 转换所有面为三角形
		aiProcess_GenNormals |         // 如果没有法线，则生成法线
		aiProcess_JoinIdenticalVertices // 合并相同顶点
	);

	int nTotalVertices = 0;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		nTotalVertices += mesh->mNumVertices;
	}

	int nIndex = 0;
	MeshData.Data.VertexData.resize(nTotalVertices);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		MeshData.SectionDescribe.push_back(FMeshSection());
		FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

		for (unsigned int k = 0; k < mesh->mNumVertices; k++)
		{
			MeshData.Data.VertexData[nIndex].Position.x = mesh->mVertices[k].x;
			MeshData.Data.VertexData[nIndex].Position.y = mesh->mVertices[k].y;
			MeshData.Data.VertexData[nIndex].Position.z = mesh->mVertices[k].z;

			if (mesh->HasNormals())
			{
				MeshData.Data.VertexData[nIndex].Normal.x = mesh->mNormals[k].x;
				MeshData.Data.VertexData[nIndex].Normal.y = mesh->mNormals[k].y;
				MeshData.Data.VertexData[nIndex].Normal.z = mesh->mNormals[k].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				MeshData.Data.VertexData[nIndex].TexCoord.x = mesh->mTextureCoords[0][k].x;
				MeshData.Data.VertexData[nIndex].TexCoord.y = mesh->mTextureCoords[0][k].y;
			}

			MeshData.Data.VertexData[nIndex].Color = XMFLOAT4(Colors::White);
			nIndex++;
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++) 
			{
				MeshData.Data.IndexData.push_back(face.mIndices[j]);
			}
		}

		Section.IndexSize = MeshData.Data.IndexData.size();
		Section.VertexSize = MeshData.Data.VertexData.size();

	}

	SpawnDefaultMaterial();

	return true;
}

void CCustomMeshComponent::BuildKey(size_t& OutHashKey, const std::string& InPath, const FIEParam& InParam)
{
	std::hash<string> FloatHash;
	
	OutHashKey = 3;
	OutHashKey += FloatHash(InPath);
}