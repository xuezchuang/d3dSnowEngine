#include "../../../../../Interface/DirectXDeviceInterfece.h"

struct FIMGUIPipeline :public IDirectXDeviceInterfece_Struct
{
	FIMGUIPipeline();

	void Init(ID3D12DescriptorHeap* InHeap, UINT InOffset);

	void Draw(float DeltaTime);

	void Exit();

protected:
	void Tick(float DeltaTime);
};