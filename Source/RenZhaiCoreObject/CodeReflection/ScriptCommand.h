#pragma once
#include "../CoreObjectMinimal.h"

//�ֽ����
enum EScriptCommand
{
	Script_Undefined = 0x0B,//Ĭ�ϵ�
	Script_Int = 0x1D,//int
	Script_Float = 0x1E,//float
	Script_ConstString = 0x1F,	// String constant.
	Script_True = 0x27,	// Bool True.
	Script_False = 0x28,	// Bool False.
	Script_EndThread = 0x53,	//Nest funtion
	Script_Vector3D = 0x63,	//Nest funtion

	Script_Funtion = 0x77,//���к���
	Script_NestFunction = 0x88,	//Nest funtion

	Script_Max = 0x100
};