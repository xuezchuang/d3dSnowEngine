#include "DirectXDeviceInterfece.h"
#include "../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "../Core/World.h"
#include "../Core/Camera.h"

void IDirectXDeviceInterfece::StartSetMainViewportRenderTarget()
{
	GetEngine()->GetRenderingEngine()->StartSetMainViewportRenderTarget();
}

void IDirectXDeviceInterfece::EndSetMainViewportRenderTarget()
{
	GetEngine()->GetRenderingEngine()->EndSetMainViewportRenderTarget();
}

void IDirectXDeviceInterfece::ClearMainSwapChainCanvas()
{
	GetEngine()->GetRenderingEngine()->ClearMainSwapChainCanvas();
}
//
//ComPtr<ID3D12Fence> IDirectXDeviceInterfece::GetFence() const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->Fence;
//		}
//	}
//
//	return NULL;
//}
//
//ComPtr<ID3D12Device> IDirectXDeviceInterfece::GetD3dDevice()const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->D3dDevice;
//		}
//	}
//
//	return NULL;
//}

CLightManage* IDirectXDeviceInterfece::GetLightManage() const
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GetLightManage();
		}
	}

	return NULL;
}

CMeshManage* IDirectXDeviceInterfece::GetMeshManage() const
{
	return GetEngine()->GetMeshManage();
}

CWorld* IDirectXDeviceInterfece::GetWorld()const
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->GetWorld();
	}
	
	return NULL;
}

GCamera* IDirectXDeviceInterfece::GetCamera() const
{
	if (GetWorld())
	{
		return GetWorld()->GetCamera();
	}

	return nullptr;
}
//
//ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterfece::GetGraphicsCommandList()const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->GraphicsCommandList;
//		}
//	}
//
//	return NULL;
//}
//
//ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterfece::GetCommandAllocator()const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->CommandAllocator;
//		}
//	}
//
//	return NULL;
//}
//
//ComPtr<ID3D12CommandQueue> IDirectXDeviceInterfece::GetCommandQueue()const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->CommandQueue;
//		}
//	}
//
//	return NULL;
//}
//
//ID3D12DescriptorHeap* IDirectXDeviceInterfece::GetRTVHeap()const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->RTVHeap.Get();
//		}
//	}
//
//	return NULL;
//}
//
//ID3D12DescriptorHeap* IDirectXDeviceInterfece::GetDSVHeap()const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->DSVHeap.Get();
//		}
//	}
//
//	return NULL;
//}

int IDirectXDeviceInterfece::GetVieportWidth() const
{
	if (GCamera *InCamera = GetCamera())
	{
		return InCamera->GetWidth();
	}

	return 0;
}

int IDirectXDeviceInterfece::GetVieportHeight() const
{
	if (GCamera* InCamera = GetCamera())
	{
		return InCamera->GetHeight();
	}

	return 0;
}

//UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByDSV()const
//{
//	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
//}
//
//UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByRTV()const
//{
//	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//}
//
//UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()const
//{
//	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//}

//UINT64 IDirectXDeviceInterfece::GetCurrentFenceIndex()const
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->CurrentFenceIndex;
//		}
//	}
//
//	return NULL;
//}

HWND IDirectXDeviceInterfece::GetMianWindowsHandle()const
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->MianWindowsHandle;
		}
	}
	return HWND();
}

//ID3D12Resource* IDirectXDeviceInterfece::GetDepthStencilBuffer()
//{
//	if (CWindowsEngine* InEngine = GetEngine())
//	{
//		if (InEngine->GetRenderingEngine())
//		{
//			return InEngine->GetRenderingEngine()->DepthStencilBuffer.Get();
//		}
//	}
//
//	return NULL;
//}

FRenderingPipeline* IDirectXDeviceInterfece::GetRenderingPipeline() const
{
	if (CMeshManage *MeshManage = GetMeshManage())
	{
		return MeshManage->GetRenderingPipeline();
	}

	return NULL;
}

FRenderLayerManage* IDirectXDeviceInterfece::GetRenderLayerManage() const
{
	if (FRenderingPipeline *InRenderingPipeline = GetRenderingPipeline())
	{
		return InRenderingPipeline->GetRenderLayer();
	}

	return nullptr;
}


#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterfece::GetEngine() const
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
//CEngine* IDirectXDeviceInterfece::GetEngine()
//{
//	return Engine;
//}

#endif


//ComPtr<ID3D12Fence> IDirectXDeviceInterfece_Struct::GetFence() const
//{
//	return Interfece.GetFence();
//}
//
//ComPtr<ID3D12Device> IDirectXDeviceInterfece_Struct::GetD3dDevice()const
//{
//	return Interfece.GetD3dDevice();
//}
//
CLightManage* IDirectXDeviceInterfece_Struct::GetLightManage()const
{
	return Interfece.GetLightManage();
}
//
//CMeshManage* IDirectXDeviceInterfece_Struct::GetMeshManage()const
//{
//	return Interfece.GetMeshManage();
//}
//
//CWorld* IDirectXDeviceInterfece_Struct::GetWorld()const
//{
//	return Interfece.GetWorld();
//}
//
//ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterfece_Struct::GetGraphicsCommandList()const
//{
//	return Interfece.GetGraphicsCommandList();
//}
//
//ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterfece_Struct::GetCommandAllocator()const
//{
//	return Interfece.GetCommandAllocator();
//}
//
//ComPtr<ID3D12CommandQueue> IDirectXDeviceInterfece_Struct::GetCommandQueue()const
//{
//	return Interfece.GetCommandQueue();
//}
//
//ID3D12DescriptorHeap* IDirectXDeviceInterfece_Struct::GetRTVHeap()const
//{
//	return Interfece.GetRTVHeap();
//}
//
//ID3D12DescriptorHeap* IDirectXDeviceInterfece_Struct::GetDSVHeap()const
//{
//	return Interfece.GetDSVHeap();
//}
//
//UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByDSV()const
//{
//	return Interfece.GetDescriptorHandleIncrementSizeByDSV();
//}
//
//UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByRTV()const
//{
//	return Interfece.GetDescriptorHandleIncrementSizeByRTV();
//}
//
//UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()const
//{
//	return Interfece.GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
//}

//UINT64 IDirectXDeviceInterfece_Struct::GetCurrentFenceIndex()const
//{
//	return Interfece.GetCurrentFenceIndex();
//}
//
HWND IDirectXDeviceInterfece_Struct::GetMianWindowsHandle()const
{
	return Interfece.GetMianWindowsHandle();
}
//
//FRenderingPipeline* IDirectXDeviceInterfece_Struct::GetRenderingPipeline() const
//{
//	return Interfece.GetRenderingPipeline();
//}
//
//FRenderLayerManage* IDirectXDeviceInterfece_Struct::GetRenderLayerManage() const
//{
//	return Interfece.GetRenderLayerManage();
//}
//
//#if defined(_WIN32)
//CWindowsEngine* IDirectXDeviceInterfece_Struct::GetEngine()const
//{
//	return Interfece.GetEngine();
//}
//#else
//CEngine* IDirectXDeviceInterfece_Struct::GetEngine()const
//{
//	return Interfece.GetEngine();
//}
//
//#endif
//
#if EDITOR_ENGINE
#include "../../EditorEngine/EditorEngine.h"
CEditorEngine* IDirectXDeviceInterfece::GetEditorEngine()const
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->EditorEngine;
	}

	return NULL;
}

//ID3D12Resource* IDirectXDeviceInterfece_Struct::GetDepthStencilBuffer()
//{
//	return Interfece.GetDepthStencilBuffer();
//}
//
CEditorEngine* IDirectXDeviceInterfece_Struct::GetEditorEngine()const
{
	return Interfece.GetEditorEngine();
}
#endif