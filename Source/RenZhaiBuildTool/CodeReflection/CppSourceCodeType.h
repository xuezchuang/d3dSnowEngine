#pragma once

#include "../RenZhaiBuildTool.h"

struct FBuildElement
{
	string Name; //����

	std::map<string, string> Metas;//
	std::vector<string> Fields;
};

//����
struct FParamElement :public FBuildElement
{
	string DefaultValue;//���������Ĭ��ֵ �������

	bool bConst;//�Ƿ���const
	bool bPointer;//�Ƿ���ָ��*
	bool bReference;//�Ƿ�������&
	string Type;//float int ...

	//�п������ڲ����� ���Ϳ�����ģ��
	vector<FParamElement> InternalType;//����ģ�� vector<float> or vector123<float,int,float>...

	FParamElement()
		: bPointer(false)
		, bReference(false)
		, bConst(false)
	{}
};

//����
struct FVariableAnalysis :public FParamElement
{
	string CodeType;
	bool bStatic; //�Ƿ��Ǿ�̬

	FVariableAnalysis()
		:bStatic(false)
	{}
};

//��������
struct FFunctionAnalysis :public FBuildElement
{
	std::vector<FParamElement> ParamArray;//�����ľ�ֵ̬
	FParamElement Return;//��������

	bool bStatic;//��̬
	bool bVirtual;//����

	FFunctionAnalysis()
		:bStatic(false)
		, bVirtual(false)
	{
		Metas.insert({ "Category","Default" });
		Metas.insert({ "CodeType","" });
	}
};

//�������Ա
struct FClassAnalysis
{
	std::vector<FFunctionAnalysis> Function;//����
	std::vector<FVariableAnalysis> Variable;//���ٱ���

	string APIName;//_API������
	string ClassName;//������

	std::vector<string> InheritName;//�̳�������

	string CodeCPPName;//CPP����
	string Filename;//.h�ļ�·��
	int CodeLine;//������������У�

	string ModularPath;//E:\RenZhaiEngine\Source\RenZhaiBuildTool
};

enum ECollectionParmType
{
	Type_Return,//�Ƿ���
	Type_Parm,//����
};

bool IsCheckAllowCodeReflection(const std::vector<std::string>& InContent);