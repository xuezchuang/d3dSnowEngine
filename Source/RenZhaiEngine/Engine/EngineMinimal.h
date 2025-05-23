#pragma once

#include "simple_library/public/simple_library.h"
//#pragma comment(lib, "SimpleBaseLibrary.lib")
//
////链接EngineCore
#include "Core/Path/EnginePath.h"
//#pragma comment(lib, "RenZhaiEngineCore.lib")
//#pragma comment(lib, "RenZhaiCoreObject.lib")
//
////资源处理
//#pragma comment(lib, "RenZhaiAssetHandle.lib")
//
#include "Debug/Log/SimpleLog.h"

#include "MeshImportExportToolType.h"
//
#include "imgui.h"
////#include "../imgui/backends/imgui_impl_win32.h"
////#include "../imgui/backends/imgui_impl_dx12.h"
//#include <d3d12.h>
//#include <tchar.h>

//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "D3D12.lib")
//#pragma comment(lib, "dxgi.lib")

//#include <wrl.h>//
#include "d3dx12.h"
//#include "include/D3D12RHI.h"
//#include "Rendering/DirectX12/DDSTextureLoader.h"//
#include <D3Dcompiler.h>//
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "Debug/EngineDebug.h"

using namespace Microsoft::WRL;
//using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class CEngine* Engine;
