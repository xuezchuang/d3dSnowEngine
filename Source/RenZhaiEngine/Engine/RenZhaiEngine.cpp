#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"
#include "Core/AssetPackageManage.h"
#include "CoreObject/ClassObject.h"
#include "CodeReflection/ClassManage.h"
#include "Timer.h"

int Init(CEngine* InEngine,HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(_WIN32)
	FWinMainCommandParameters WinMainParameters(hInstance, prevInstance, cmdLine, showCmd);
#endif 

	//��������Class Ϊ����ͼ����
	for (int i = 0; i < FClassManage::GetNum(); i++)
	{
		if (CClassObject* InObjectClass = FClassManage::GetClassByIndex(i))
		{
			InObjectClass->GetDefaultObject();
		}
	}

	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		WinMainParameters
#endif 
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Init(
#if defined(_WIN32)
		WinMainParameters
#endif 
	);
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error, please check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	//��ʼ�����ǵ��ʲ�
	//FAssetPackageManage::Get()->Init();

	ReturnValue = InEngine->PostInit();
	if(ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initialization error, please check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

void Tick(CEngine* InEngine, float DeltaTime)
{
	FAssetPackageManage::Tick(DeltaTime);

	InEngine->Tick(DeltaTime);

	//Sleep(30);
}

int Exit(CEngine* InEngine)
{
	int ReturnValue = InEngine->PreExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit failed.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

CEngine* Engine = NULL;
std::unique_ptr<FTimer> Timer = std::make_unique<FTimer>();

//hInstance �Լ���ʵ��
//prevInstance �ϴε�ʵ��
//cmdLine ��������
//showCmd ������cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;

	Engine = FEngineFactory::CreateEngine();
	if (Engine)
	{
		//��ʼ��
		Init(Engine, hInstance, prevInstance, cmdLine, showCmd);

		MSG EngineMsg = { 0 };

		Timer->Reset();

		//��Ⱦ��ͼ
		while (EngineMsg.message != WM_QUIT)
		{
			//PM_NOREMOVE ��Ϣ���Ӷ����������
			//PM_REMOVE   ��Ϣ�Ӷ����������
			//PM_NOYIELD  �˱�־ʹϵͳ���ͷŵȴ����ó�����е��߳�
			// 
			//PM_QS_INPUT �������ͼ�����Ϣ��
			//PM_QS_PAINT ����ͼ��Ϣ��
			//PM_QS_POSTMESSAGE �������б����͵���Ϣ��������ʱ�����ȼ���
			//PM_QS_SENDMESSAGE �������з�����Ϣ��
			if (PeekMessage(&EngineMsg,0,0,0, PM_REMOVE))
			{
				TranslateMessage(&EngineMsg);
				DispatchMessage(&EngineMsg);
			}
			else
			{
				Timer->Tick();

				Tick(Engine,Timer->GetDeltaTime());

				//ģ����Թ̶�����
			//	Sleep(1);
			}
		}

		//�˳�
		ReturnValue = Exit(Engine);
	}
	else
	{
		ReturnValue = 1;
	}

	Engine_Log("[%i]The engine has exited.", ReturnValue);
	return ReturnValue;
}