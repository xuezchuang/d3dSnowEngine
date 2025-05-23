#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../DynamicMap/ShadowMap/DynamicShadowMap.h"
#include "../DynamicMap/ShadowMap/DynamicShadowCubeMap.h"
#include "../Engine/Mesh/Core/ObjectTransformation.h"
#include "GpuBuffer.h"
#include "TextureManager.h"
#include "../Engine/Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../Engine/Core/Viewport/ViewportTransformation.h"
#include "../Engine/Component/Light/Core/LightConstantBuffer.h"

enum ERenderingMeshType;
class CMaterial;
struct FRenderingTexture;
class CFogComponent;

enum EFindValueType
{
	TYPE_IN_PROGRAM,
	TYPE_COMPLETE,
};

struct FGeometry :public IDirectXDeviceInterfece_Struct
{
	friend struct FGeometryMap;

	bool IsRenderingDataExistence(CMeshComponent* InKey);

	void BuildMesh(const size_t InMeshHash,CMeshComponent* InMesh, const FMeshRenderingData& MeshData,int InKey);
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FSkinnedMeshRenderingData& SkinnedMeshData, int InKey);
	
	void DuplicateMesh(CMeshComponent* InMesh,std::shared_ptr<FRenderingData>& MeshData, int InKey);
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData,int InRenderLayerIndex = -1);

	//构建模型
	void Build(int InType);

	//获取基础构建数据
	bool GetRenderingDataInfo(
		ERenderingMeshType InMeshType,
		UINT& VertexSizeInBytes,
		UINT& IndexSizeInBytes,
		void*& VertexDataPtr,
		void*& IndexDataPtr);

	UINT GetDrawObjectNumber() const;

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(int InType);
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(int InType);

public:
	static void FindRenderingDatas(std::function<EFindValueType(std::shared_ptr<FRenderingData>&)> InFun);

	//复制
private:
	template<class T>
	void DuplicateMesh_Interior(
		CMeshComponent* InMesh,
		std::shared_ptr<FRenderingData>& MeshData,
		FMeshData<T>& InMyRenderingData,
		int InKey);

	void DuplicateMeshRenderingSection(
		const std::shared_ptr<FRenderingData>& MeshData,
		std::shared_ptr<FRenderingData>& InMeshRenderingData);
	//构建
private:
	template<class T>
	void BuildMesh_Interior(
		const size_t InMeshHash,
		CMeshComponent* InMesh,
		const FRenderContent<T>& MeshData,
		int InKey,
		FMeshData<T>& InMeshRenderingData,
		std::function<void(std::shared_ptr<FRenderingData>)> InFun);

	template<class T>
	void BuildBoundingBox(const FRenderContent<T>& MeshData, BoundingBox& OutBounds);

	template<class T>
	void BuildRenderingSection(
		const FRenderContent<T>& MeshData,
		std::shared_ptr<FRenderingData> InRenderingData,
		FMeshData<T>& InMeshRenderingData);

	void BuildUniqueRenderingSection(
		const std::shared_ptr<FRenderingData>& MeshData,
		std::shared_ptr<FRenderingData>& InMeshRenderingData);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ByteAddressBuffer m_GPUVertexBufferPtr;
	ByteAddressBuffer m_GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	//真实的数据
	FVertexMeshData MeshRenderingData;
	FSkinnedVertexMeshData SkinnedMeshRenderingData;
protected:
	//唯一性的渲染池
	static map<size_t, std::shared_ptr<FRenderingData>> UniqueRenderingDatas;

public:
	//真正的渲染池 里面会有重复的 key (size_t)
	static vector<std::shared_ptr<FRenderingData>> RenderingDatas;
};

//提供渲染内容的接口
struct FGeometryMap :public IDirectXDeviceInterfece_Struct
{
	friend class FRenderLayer;
	friend class FDynamicCubeMap;
	friend class FDynamicShadowMap;
	friend class FRenderingPipeline;

	FGeometryMap();
	~FGeometryMap();

	void SetDescriptorHeaps();
	void Draw(GraphicsContext& gfxContext, float DeltaTime);
	void PostDraw(float DeltaTime);

	virtual void OnResetSize(int InWidth, int InHeight);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateCalculationsViewport(
		float DeltaTime, 
		const FViewportInfo& ViewportInfo,
		UINT InConstantBufferOffset);

	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateLight(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateFog(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateSkinned(float DeltaTime, CComponent* InSkinComponent);

	//收集动态反射模型
	void BuildDynamicReflectionMesh();
	void BuildFog();
	void BuildShadow();
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FSkinnedMeshRenderingData& MeshData);
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	void DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData);
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

	void LoadTexture();

	//构建模型
	void Build();

	//描述堆
	void BuildDescriptorHeap();

	//构建Mesh常量缓冲区
	void BuildMeshConstantBuffer();

	//构建雾气常量缓冲区
	void BuildFogConstantBuffer();

	//构建Material常量缓冲区
	void BuildMaterialShaderResourceView();

	//构建Material常量缓冲区
	void BuildLightConstantBuffer();

	//构建蒙皮网格
	void BuildSkinnedMeshConstantBuffer();

	//该接口会有变化
	UINT GetDrawMeshObjectNumber();

	//该接口会有变化
	UINT GetDrawMaterialObjectNumber();

	//该接口会有变化
	UINT GetDrawLightObjectNumber();
	
	//2D贴图数量
	UINT GetDrawTexture2DResourcesNumber();

	//CubeMap贴图数量
	//UINT GetDrawCubeMapResourcesNumber();

	//动态摄像机
	UINT GetDynamicReflectionViewportNum();

	//构建我们的贴图SRV视图
	void BuildTextureConstantBuffer();

public:
	UINT GetDynamicReflectionMeshComponentsSize();
	CMeshComponent* GetDynamicReflectionMeshComponents(int Index);
public:
	bool IsStartUPFog();

public:
	void DrawShadow(float DeltaTime);
	void DrawLight(float DeltaTime);
	void DrawMesh(float DeltaTime);
	void DrawCubeMapTexture(float DeltaTime);
	void DrawFog(float DeltaTime);
	void DrawSkinned(float DeltaTime, int InSkinnedIndex = 0);
	void DrawEmptySkinned(float DeltaTime);
public:
	ID3D12DescriptorHeap* GetHeap()const {return DescriptorHeap.GetHeap();}

protected:
	map<int, FGeometry> Geometrys;
	FDirectXDescriptorHeap DescriptorHeap;

	std::vector<FObjectTransformation> MeshObjectConstant;
	FViewportTransformation ViewportTransformation;
	FLightConstantBuffer LightConstantBuffer;

	//FConstantBufferViews MeshConstantBufferViews;
	//FConstantBufferViews MaterialConstantBufferViews;
	//FConstantBufferViews ViewportConstantBufferViews;
	FConstantBufferViews FogConstantBufferViews;
	//FConstantBufferViews LightConstantBufferViews;
	FConstantBufferViews SkinnedConstantBufferViews;

	//std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTexture2DResources;
	//std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingCubeMapResources;

	std::vector<CMaterial*> Materials;
	std::vector<CMeshComponent*> DynamicReflectionMeshComponents;
	CFogComponent* Fog;

	FDynamicShadowMap DynamicShadowMap;
	FDynamicShadowCubeMap DynamicShadowCubeMap;
	std::vector<TextureRef> m_Textures;
	//D3D12_CPU_DESCRIPTOR_HANDLE* m_TexturesDesHandle;
	std::unique_ptr<D3D12_CPU_DESCRIPTOR_HANDLE[]> m_TexturesDesHandle;
	std::unordered_map<std::string, int> m_TexName2ID;
	std::vector<FMaterialConstantBuffer> m_MaterialConstantBufferViews;
};