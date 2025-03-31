//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author(s):  James Stanard
//             Chuck Walbourn (ATG)
//
// This code depends on DirectXTex
//

#include "ModelLoader.h"
#include "Renderer.h"
#include "Model.h"
#include "glTF.h"
#include "ModelH3D.h"
#include "TextureManager.h"
#include "TextureConvert.h"
#include "GraphicsCommon.h"

#include <fstream>
#include <unordered_map>
#include "tiny_obj_loader.h"

using namespace Renderer;
using namespace Graphics;

std::unordered_map<uint32_t, uint32_t> g_SamplerPermutations;

D3D12_CPU_DESCRIPTOR_HANDLE GetSampler(uint32_t addressModes)
{
    SamplerDesc samplerDesc;
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE(addressModes & 0x3);
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE(addressModes >> 2);
    return samplerDesc.CreateDescriptor();
}

void LoadMaterials(Model& model,
    const std::vector<MaterialTextureData>& materialTextures,
    const std::vector<std::wstring>& textureNames,
    const std::vector<uint8_t>& textureOptions,
    const std::wstring& basePath)
{
    static_assert((_alignof(MaterialConstants) & 255) == 0, "CBVs need 256 byte alignment");

    // Load textures
    const uint32_t numTextures = (uint32_t)textureNames.size();
    model.textures.resize(numTextures);
    for (size_t ti = 0; ti < numTextures; ++ti)
    {
        std::wstring originalFile = basePath + textureNames[ti];
        CompileTextureOnDemand(originalFile, textureOptions[ti]);

        std::wstring ddsFile = Utility::RemoveExtension(originalFile) + L".dds";
        model.textures[ti] = TextureManager::LoadDDSFromFile(ddsFile);
    }

    // Generate descriptor tables and record offsets for each material
    const uint32_t numMaterials = (uint32_t)materialTextures.size();
    std::vector<uint32_t> tableOffsets(numMaterials);

    for (uint32_t matIdx = 0; matIdx < numMaterials; ++matIdx)
    {
        const MaterialTextureData& srcMat = materialTextures[matIdx];

        DescriptorHandle TextureHandles = Renderer::s_TextureHeap.Alloc(kNumTextures);
        uint32_t SRVDescriptorTable = Renderer::s_TextureHeap.GetOffsetOfHandle(TextureHandles);

        uint32_t DestCount = kNumTextures;
        uint32_t SourceCounts[kNumTextures] = { 1, 1, 1, 1, 1 };

        D3D12_CPU_DESCRIPTOR_HANDLE DefaultTextures[kNumTextures] =
        {
            GetDefaultTexture(kWhiteOpaque2D),
            GetDefaultTexture(kWhiteOpaque2D),
            GetDefaultTexture(kWhiteOpaque2D),
            GetDefaultTexture(kBlackTransparent2D),
            GetDefaultTexture(kDefaultNormalMap)
        };

        D3D12_CPU_DESCRIPTOR_HANDLE SourceTextures[kNumTextures];
        for (uint32_t j = 0; j < kNumTextures; ++j)
        {
            if (srcMat.stringIdx[j] == 0xffff)
                SourceTextures[j] = DefaultTextures[j];
            else
                SourceTextures[j] = model.textures[srcMat.stringIdx[j]].GetSRV();
        }

        g_Device->CopyDescriptors(1, &TextureHandles, &DestCount,
            DestCount, SourceTextures, SourceCounts, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // See if this combination of samplers has been used before.  If not, allocate more from the heap
        // and copy in the descriptors.
        uint32_t addressModes = srcMat.addressModes;
        auto samplerMapLookup = g_SamplerPermutations.find(addressModes);

        if (samplerMapLookup == g_SamplerPermutations.end())
        {
            DescriptorHandle SamplerHandles = Renderer::s_SamplerHeap.Alloc(kNumTextures);
            uint32_t SamplerDescriptorTable = Renderer::s_SamplerHeap.GetOffsetOfHandle(SamplerHandles);
            g_SamplerPermutations[addressModes] = SamplerDescriptorTable;
            tableOffsets[matIdx] = SRVDescriptorTable | SamplerDescriptorTable << 16;

            D3D12_CPU_DESCRIPTOR_HANDLE SourceSamplers[kNumTextures];
            for (uint32_t j = 0; j < kNumTextures; ++j)
            {
                SourceSamplers[j] = GetSampler(addressModes & 0xF);
                addressModes >>= 4;
            }
            g_Device->CopyDescriptors(1, &SamplerHandles, &DestCount,
                DestCount, SourceSamplers, SourceCounts, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
        }
        else
        {
            tableOffsets[matIdx] = SRVDescriptorTable | samplerMapLookup->second << 16;
        }
    }

    // Update table offsets for each mesh
    uint8_t* meshPtr = model.m_MeshData.get();
    for (uint32_t i = 0; i < model.m_NumMeshes; ++i)
    {
        Mesh& mesh = *(Mesh*)meshPtr;
        uint32_t offsetPair = tableOffsets[mesh.materialCBV];
        mesh.srvTable = offsetPair & 0xFFFF;
        mesh.samplerTable = offsetPair >> 16;
        mesh.pso = Renderer::GetPSO(mesh.psoFlags);
        meshPtr += sizeof(Mesh) + (mesh.numDraws - 1) * sizeof(Mesh::Draw);
    }
}

bool FileExists2(const std::wstring& fileName)
{
	struct _stat64 fileStat;
	return _wstat64(fileName.c_str(), &fileStat) == 0;
}

bool Renderer::BuildModel_OBJ(ModelData& model, const std::wstring& filePath)
{
	const std::wstring basePath = Utility::GetBasePath(filePath);

	std::ifstream inputFile(filePath);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &inputFile, nullptr, true);

	model.m_SceneGraph.resize(1);

	GraphNode& node = model.m_SceneGraph[0];
	node.xform = Matrix4(kIdentity);
	node.rotation = Quaternion(kIdentity);
	node.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	node.matrixIdx = 0;
	node.hasSibling = 0;

	model.m_MaterialTextures.resize(materials.size());
	model.m_MaterialConstants.resize(materials.size());
	model.m_TextureNames.clear();
	model.m_TextureOptions.clear();

	std::map<std::string, uint16_t> fileNameMap;

	for (size_t i = 0; i < materials.size(); i++)
	{
		MaterialTextureData& textureData = model.m_MaterialTextures[i];
		textureData.addressModes = 0x55555;

		const std::string& baseColorPath = materials[i].diffuse_texname;
		const std::string& metallicRoughnessPath = materials[i].specular_texname;
		const std::string& emissivePath = materials[i].emissive_texname;
		const std::string& normalPath = materials[i].normal_texname;

		const std::wstring baseColorFullPath = basePath + Utility::UTF8ToWideString(baseColorPath);
		const std::wstring metallicRoughnessFullPath = basePath + Utility::UTF8ToWideString(metallicRoughnessPath);
		const std::wstring emissiveFullPath = basePath + Utility::UTF8ToWideString(emissivePath);
		const std::wstring normalFullPath = basePath + Utility::UTF8ToWideString(normalPath);

		textureData.stringIdx[kBaseColor] = 0xFFFF;
		textureData.stringIdx[kMetallicRoughness] = 0xFFFF;
		textureData.stringIdx[kOcclusion] = 0xFFFF;
		textureData.stringIdx[kEmissive] = 0xFFFF;
		textureData.stringIdx[kNormal] = 0xFFFF;

		bool alphaTest = false;
		const std::string lowerCaseName = Utility::ToLower(baseColorPath);
		if (lowerCaseName.find("thorn") != std::string::npos ||
			lowerCaseName.find("plant") != std::string::npos ||
			lowerCaseName.find("chain") != std::string::npos)
		{
			alphaTest = true;
		}

		// Handle base color texture
		auto mapLookup = fileNameMap.find(baseColorPath);
		if (mapLookup != fileNameMap.end())
		{
			textureData.stringIdx[kBaseColor] = mapLookup->second;
		}
		else if (baseColorPath.size() > 0 && FileExists2(baseColorFullPath))
		{
			fileNameMap[baseColorPath] = (uint16_t)model.m_TextureNames.size();
			textureData.stringIdx[kBaseColor] = (uint16_t)model.m_TextureNames.size();
			model.m_TextureNames.push_back(baseColorPath);
			model.m_TextureOptions.push_back(TextureOptions(true, alphaTest, true));
		}

		// Handle occlusionMetallicRoughness texture
		mapLookup = fileNameMap.find(metallicRoughnessPath);
		if (mapLookup != fileNameMap.end())
		{
			textureData.stringIdx[kMetallicRoughness] = mapLookup->second;
		}
		else if (metallicRoughnessPath.size() > 0 && FileExists2(metallicRoughnessFullPath))
		{
			fileNameMap[metallicRoughnessPath] = (uint16_t)model.m_TextureNames.size();
			textureData.stringIdx[kMetallicRoughness] = (uint16_t)model.m_TextureNames.size();
			textureData.stringIdx[kOcclusion] = (uint16_t)model.m_TextureNames.size();
			model.m_TextureNames.push_back(metallicRoughnessPath);
			model.m_TextureOptions.push_back(TextureOptions(false, false, true));
		}

		// Handle emissive texture
		mapLookup = fileNameMap.find(emissivePath);
		if (mapLookup != fileNameMap.end())
		{
			textureData.stringIdx[kEmissive] = mapLookup->second;
		}
		else if (emissivePath.size() > 0 && FileExists2(emissiveFullPath))
		{
			fileNameMap[emissivePath] = (uint16_t)model.m_TextureNames.size();
			textureData.stringIdx[kEmissive] = (uint16_t)model.m_TextureNames.size();
			model.m_TextureNames.push_back(emissivePath);
			model.m_TextureOptions.push_back(TextureOptions(true, false, true));
		}

		// Handle normal texture
		mapLookup = fileNameMap.find(normalPath);
		if (mapLookup != fileNameMap.end())
		{
			textureData.stringIdx[kNormal] = mapLookup->second;
		}
		else if (normalPath.size() > 0 && FileExists2(normalFullPath))
		{
			fileNameMap[normalPath] = (uint16_t)model.m_TextureNames.size();
			textureData.stringIdx[kNormal] = (uint16_t)model.m_TextureNames.size();
			model.m_TextureNames.push_back(normalPath);
			model.m_TextureOptions.push_back(TextureOptions(false, false, true));
		}

		MaterialConstantData& constantData = model.m_MaterialConstants[i];
		//constantData.baseColorFactor[0] = 1.0f;
		//constantData.baseColorFactor[1] = 1.0f;
		//constantData.baseColorFactor[2] = 1.0f;
		//constantData.baseColorFactor[3] = 1.0f;
		//constantData.emissiveFactor[0] = 1.0f;
		//constantData.emissiveFactor[1] = 1.0f;
		//constantData.emissiveFactor[2] = 1.0f;
		constantData.baseColorFactor[0] = materials[i].ambient[0];
		constantData.baseColorFactor[1] = materials[i].ambient[1];
		constantData.baseColorFactor[2] = materials[i].ambient[2];
		constantData.baseColorFactor[3] = 1.0f;
		constantData.emissiveFactor[0] = materials[i].emission[0];
		constantData.emissiveFactor[1] = materials[i].emission[1];
		constantData.emissiveFactor[2] = materials[i].emission[2];
		constantData.metallicFactor = 1.0f;
		constantData.roughnessFactor = 1.0f;
		constantData.normalTextureScale = 1.0f;
		constantData.flags = 0x3C000000;
		if (alphaTest)
			constantData.flags |= 0x60; // twoSided + alphaTest
	}

	ASSERT(model.m_TextureOptions.size() == model.m_TextureNames.size());
	for (size_t ti = 0; ti < model.m_TextureNames.size(); ++ti)
	{
		std::wstring fullPath = basePath + Utility::UTF8ToWideString(model.m_TextureNames[ti]);
		CompileTextureOnDemand(fullPath, model.m_TextureOptions[ti]);
	}

	model.m_BoundingSphere = BoundingSphere(kZero);
	model.m_BoundingBox = AxisAlignedBox(kZero);

	glTF::Accessor PosStream;
	PosStream.dataPtr = reinterpret_cast<byte*>(attrib.vertices.data());
	PosStream.stride = 0;// mesh.vertexStride;
	PosStream.count = (uint32_t)attrib.vertices.size();
	PosStream.componentType = glTF::Accessor::kFloat;
	PosStream.type = glTF::Accessor::kVec3;

	glTF::Accessor UVStream;
	UVStream.dataPtr = reinterpret_cast<byte*>(attrib.texcoords.data());
	UVStream.stride = 12;// mesh.vertexStride;
	UVStream.count = (uint32_t)attrib.texcoords.size();
	UVStream.componentType = glTF::Accessor::kFloat;
	UVStream.type = glTF::Accessor::kVec2;

	glTF::Accessor NormalStream;
	NormalStream.dataPtr = reinterpret_cast<byte*>(attrib.normals.data());
	NormalStream.stride = 20;
	NormalStream.count = (uint32_t)attrib.normals.size();
	NormalStream.componentType = glTF::Accessor::kFloat;
	NormalStream.type = glTF::Accessor::kVec3;

	// We're going to piggy-back off of the work to compile glTF meshes by pretending that's what we have.
	for (uint32_t i = 0; i < shapes.size(); ++i)
	{
		tinyobj::mesh_t& mesh = shapes[i].mesh;

		glTF::Accessor IndexStream;
		IndexStream.dataPtr = reinterpret_cast<byte*>(mesh.indices.data());
		IndexStream.stride = 2;
		IndexStream.count = (uint32_t)mesh.indices.size();
		IndexStream.componentType = glTF::Accessor::kUnsignedShort;
		IndexStream.type = glTF::Accessor::kScalar;

		//glTF::Material material;
		//material.flags = model.m_MaterialConstants[mesh.materialIndex].flags;
		//material.index = mesh.materialIndex;

		glTF::Mesh gltfMesh;
		gltfMesh.primitives.resize(1);

		glTF::Primitive& prim = gltfMesh.primitives[0];
		prim.attributes[glTF::Primitive::kPosition] = &PosStream;
		//prim.attributes[glTF::Primitive::kTexcoord0] = &UVStream;
		//prim.attributes[glTF::Primitive::kNormal] = &NormalStream;
		prim.indices = &IndexStream;
		//prim.material = &material;
		prim.attribMask = 0xB;
		prim.mode = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		//memcpy(prim.minPos, &mesh.boundingBox.GetMin(), 12);
		//memcpy(prim.maxPos, &mesh.boundingBox.GetMax(), 12);
		prim.minIndex = 0;
		prim.maxIndex = 0;

		BoundingSphere sphereOS;
		AxisAlignedBox boxOS;
		Renderer::CompileMesh(model.m_Meshes, model.m_GeometryData, gltfMesh, 0, Matrix4(kIdentity), sphereOS, boxOS);
		model.m_BoundingSphere = model.m_BoundingSphere.Union(sphereOS);
		model.m_BoundingBox.AddBoundingBox(boxOS);
	}
	return true;
}

std::shared_ptr<Model> Renderer::LoadModel(const std::wstring& filePath, bool forceRebuild)
{
    const std::wstring miniFileName = Utility::RemoveExtension(filePath) + L".mini";
    const std::wstring fileName = Utility::RemoveBasePath(filePath);

    struct _stat64 sourceFileStat;
    struct _stat64 miniFileStat;
    std::ifstream inFile;
    FileHeader header;

    bool sourceFileMissing = _wstat64(filePath.c_str(), &sourceFileStat) == -1;
    bool miniFileMissing = _wstat64(miniFileName.c_str(), &miniFileStat) == -1;

    if (sourceFileMissing)
        forceRebuild = false;

    if (sourceFileMissing && miniFileMissing)
    {
        Utility::Printf("Error: Could not find %ws\n", fileName.c_str());
        return nullptr;
    }

    bool needBuild = forceRebuild;

    // Check if .mini file exists and it is newer than source file
    if (miniFileMissing || !sourceFileMissing && sourceFileStat.st_mtime > miniFileStat.st_mtime)
        needBuild = true;

    // Check if it's an older version of .mini
    if (!needBuild)
    {
        inFile = std::ifstream(miniFileName, std::ios::in | std::ios::binary);
        inFile.read((char*)&header, sizeof(FileHeader));
        if (strncmp(header.id, "MINI", 4) != 0 || header.version != CURRENT_MINI_FILE_VERSION)
        {
            Utility::Printf("Model version deprecated.  Rebuilding %ws...\n", fileName.c_str());
            needBuild = true;
            inFile.close();
        }
    }

    if (needBuild)
    {
        if (sourceFileMissing)
        {
            Utility::Printf("Error: Could not find %ws\n", fileName.c_str());
            return nullptr;
        }

        ModelData modelData;

        const std::wstring fileExt = Utility::ToLower(Utility::GetFileExtension(filePath));

        if (fileExt == L"gltf" || fileExt == L"glb")
        {
            glTF::Asset asset(filePath);
            if (!BuildModel(modelData, asset))
                return nullptr;
        }
        else if (fileExt == L"h3d")
        {
            ModelH3D modelh3d;
            const std::wstring basePath = Utility::GetBasePath(filePath);
            if (!modelh3d.Load(filePath) || !modelh3d.BuildModel(modelData, basePath))
                return nullptr;
        }
		else if (fileExt == L"obj")
		{
			if (!BuildModel_OBJ(modelData, filePath))
				return nullptr;
		}
        else
        {
            Utility::Printf(L"Unsupported model file extension: %ws\n", fileExt.c_str());
            return nullptr;
        }

        if (!SaveModel(miniFileName, modelData))
            return nullptr;

        inFile = std::ifstream(miniFileName, std::ios::in | std::ios::binary);
        inFile.read((char*)&header, sizeof(FileHeader));
    }

    if (!inFile)
        return nullptr;

    ASSERT(strncmp(header.id, "MINI", 4) == 0 && header.version == CURRENT_MINI_FILE_VERSION);

    std::wstring basePath = Utility::GetBasePath(filePath);

    std::shared_ptr<Model> model(new Model);

    model->m_NumNodes = header.numNodes;
    model->m_SceneGraph.reset(new GraphNode[header.numNodes]);
    model->m_NumMeshes = header.numMeshes;
    model->m_MeshData.reset(new uint8_t[header.meshDataSize]);

	if (header.geometrySize > 0)
	{
		UploadBuffer modelData;
		modelData.Create(L"Model Data Upload", header.geometrySize);
		inFile.read((char*)modelData.Map(), header.geometrySize);
		modelData.Unmap();
		model->m_DataBuffer.Create(L"Model Data", header.geometrySize, 1, modelData);
	}

    inFile.read((char*)model->m_SceneGraph.get(), header.numNodes * sizeof(GraphNode));
    inFile.read((char*)model->m_MeshData.get(), header.meshDataSize);

	if (header.numMaterials > 0)
	{
		UploadBuffer materialConstants;
		materialConstants.Create(L"Material Constant Upload", header.numMaterials * sizeof(MaterialConstants));
		MaterialConstants* materialCBV = (MaterialConstants*)materialConstants.Map();
		for (uint32_t i = 0; i < header.numMaterials; ++i)
		{
			inFile.read((char*)materialCBV, sizeof(MaterialConstantData));
			materialCBV++;
		}
		materialConstants.Unmap();
		model->m_MaterialConstants.Create(L"Material Constants", header.numMaterials, sizeof(MaterialConstants), materialConstants);
	}

    // Read material texture and sampler properties so we can load the material
    std::vector<MaterialTextureData> materialTextures(header.numMaterials);
    inFile.read((char*)materialTextures.data(), header.numMaterials * sizeof(MaterialTextureData));

    std::vector<std::wstring> textureNames(header.numTextures);
    for (uint32_t i = 0; i < header.numTextures; ++i)
    {
        std::string utf8TextureName;
        std::getline(inFile, utf8TextureName, '\0');
        textureNames[i] = Utility::UTF8ToWideString(utf8TextureName);
    }

    std::vector<uint8_t> textureOptions(header.numTextures);
    inFile.read((char*)textureOptions.data(), header.numTextures * sizeof(uint8_t));

    LoadMaterials(*model, materialTextures, textureNames, textureOptions, basePath);

    model->m_BoundingSphere = BoundingSphere(*(XMFLOAT4*)header.boundingSphere);
    model->m_BoundingBox = AxisAlignedBox(Vector3(*(XMFLOAT3*)header.minPos), Vector3(*(XMFLOAT3*)header.maxPos));

    // Load animation data
    model->m_NumAnimations = header.numAnimations;

    if (header.numAnimations > 0)
    {
        ASSERT(header.keyFrameDataSize > 0 && header.numAnimationCurves > 0);
        model->m_KeyFrameData.reset(new uint8_t[header.keyFrameDataSize]);
        inFile.read((char*)model->m_KeyFrameData.get(), header.keyFrameDataSize);
        model->m_CurveData.reset(new AnimationCurve[header.numAnimationCurves]);
        inFile.read((char*)model->m_CurveData.get(), header.numAnimationCurves * sizeof(AnimationCurve));
        model->m_Animations.reset(new AnimationSet[header.numAnimations]);
        inFile.read((char*)model->m_Animations.get(), header.numAnimations * sizeof(AnimationSet));
    }

    model->m_NumJoints = header.numJoints;

    if (header.numJoints > 0)
    {
        model->m_JointIndices.reset(new uint16_t[header.numJoints]);
        inFile.read((char*)model->m_JointIndices.get(), header.numJoints * sizeof(uint16_t));
        model->m_JointIBMs.reset(new Matrix4[header.numJoints]);
        inFile.read((char*)model->m_JointIBMs.get(), header.numJoints * sizeof(Matrix4));
    }

    return model;
}
