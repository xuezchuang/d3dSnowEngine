#pragma once
#include "../Engine/EngineMacro.h"
#include "CoreMacro.h"

#if EDITOR_ENGINE
class GActorObject;

class FOperationHandleSelectManage
{
public:
	FOperationHandleSelectManage();

	static FOperationHandleSelectManage* Get();
	static void Destroy();

	bool IsCaptureMouseNotOnUI();
public:
	void AllOperationHandleHide();

public:
	void DisplaySelectedOperationHandle(GActorObject* InNewSelectedObject);
	void DisplaySelectedOperationHandle();
	void HideSelectedOperationHandle();

public:
	//��ʾ�µ��ֱ�
	void SetNewSelectedOperationHandle(GActorObject* InNewSelectedObject);
	//����ѡ�����
	void SetNewSelectedObject(GActorObject* InNewSelectedObject);

public:
	GActorObject* GetSelectedOperationHandle();
private:
	static FOperationHandleSelectManage* InManage;

	//��¼��ǰ�Ĳ����ֱ� �����´�����ʾ����
	GActorObject* SelectedOperationHandle;
};

#endif