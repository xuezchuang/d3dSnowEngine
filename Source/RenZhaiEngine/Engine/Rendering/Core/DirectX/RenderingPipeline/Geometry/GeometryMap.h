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
#include "../GpuBuffer.h"

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

	//����ģ��
	void Build(int InType);

	//��ȡ������������
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

	//����
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
	//����
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

	FGpuBuffer m_GPUVertexBufferPtr;
	FGpuBuffer m_GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	//��ʵ������
	FVertexMeshData MeshRenderingData;
	FSkinnedVertexMeshData SkinnedMeshRenderingData;
protected:
	//Ψһ�Ե���Ⱦ��
	static map<size_t, std::shared_ptr<FRenderingData>> UniqueRenderingDatas;

public:
	//��������Ⱦ�� ��������ظ��� key (size_t)
	static vector<std::shared_ptr<FRenderingData>> RenderingDatas;
};

//�ṩ��Ⱦ���ݵĽӿ�
struct FGeometryMap :public IDirectXDeviceInterfece_Struct
{
	friend class FRenderLayer;
	friend class FDynamicCubeMap;
	friend class FDynamicShadowMap;
	friend class FRenderingPipeline;

	FGeometryMap();
	~FGeometryMap();

	void SetDescriptorHeaps();
	void Draw(float DeltaTime);
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

	//�ռ���̬����ģ��
	void BuildDynamicReflectionMesh();
	void BuildFog();
	void BuildShadow();
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FSkinnedMeshRenderingData& MeshData);
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	void DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData);
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

	void LoadTexture();

	//����ģ��
	void Build();

	//������
	void BuildDescriptorHeap();

	//����Mesh����������
	void BuildMeshConstantBuffer();

	//������������������
	void BuildFogConstantBuffer();

	//����Material����������
	void BuildMaterialShaderResourceView();

	//����Material����������
	void BuildLightConstantBuffer();

	//������Ƥ����
	void BuildSkinnedMeshConstantBuffer();

	//�ýӿڻ��б仯
	UINT GetDrawMeshObjectNumber();

	//�ýӿڻ��б仯
	UINT GetDrawMaterialObjectNumber();

	//�ýӿڻ��б仯
	UINT GetDrawLightObjectNumber();
	
	//2D��ͼ����
	UINT GetDrawTexture2DResourcesNumber();

	//CubeMap��ͼ����
	UINT GetDrawCubeMapResourcesNumber();

	//��̬�����
	UINT GetDynamicReflectionViewportNum();

	//�������ǵ���ͼSRV��ͼ
	void BuildTextureConstantBuffer();

	//�������ǵ��ӿڳ�����������ͼ
	void BuildViewportConstantBufferView(UINT InViewportOffset = 0);

public:
	UINT GetDynamicReflectionMeshComponentsSize();
	CMeshComponent* GetDynamicReflectionMeshComponents(int Index);
public:
	UINT GetViewportConstantBufferByteSize();
public:
	D3D12_GPU_VIRTUAL_ADDRESS ViewportGPUVirtualAddress();
public:
	bool IsStartUPFog();

public:
	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey);
public:
	void DrawShadow(float DeltaTime);
	void DrawLight(float DeltaTime);
	void DrawViewport(float DeltaTime);
	void DrawMesh(float DeltaTime);
	void DrawMaterial(float DeltaTime);
	void Draw2DTexture(float DeltaTime);
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


	FConstantBufferViews MeshConstantBufferViews;
	FConstantBufferViews MaterialConstantBufferViews;
	FConstantBufferViews ViewportConstantBufferViews;
	FConstantBufferViews FogConstantBufferViews;
	FConstantBufferViews LightConstantBufferViews;
	FConstantBufferViews SkinnedConstantBufferViews;

	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTexture2DResources;
	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingCubeMapResources;
	std::vector<CMaterial*> Materials;
	std::vector<CMeshComponent*> DynamicReflectionMeshComponents;
	CFogComponent* Fog;

	FDynamicShadowMap DynamicShadowMap;
	FDynamicShadowCubeMap DynamicShadowCubeMap;
};