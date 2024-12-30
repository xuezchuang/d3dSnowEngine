#include "../../../../../Interface/DirectXDeviceInterfece.h"

struct FIMGUIPipeline :public IDirectXDeviceInterfece_Struct
{
	FIMGUIPipeline();

	void BuildDesciptorHeap();

	void Init();

	void Draw(float DeltaTime);

	void Exit();

protected:
	

	void Tick(float DeltaTime);

	ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
};