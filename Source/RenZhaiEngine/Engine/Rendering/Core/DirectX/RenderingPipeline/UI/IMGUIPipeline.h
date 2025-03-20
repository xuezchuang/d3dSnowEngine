#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "CommandContext.h"

struct FIMGUIPipeline :public IDirectXDeviceInterfece_Struct
{
	FIMGUIPipeline();

	

	void Init();

	void Draw(GraphicsContext& context,float DeltaTime);

	void Exit();

protected:
	
	void BuildDesciptorHeap();

	void Tick(float DeltaTime);

	ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
};