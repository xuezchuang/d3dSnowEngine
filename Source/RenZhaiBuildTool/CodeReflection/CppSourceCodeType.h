#pragma once

#include "../RenZhaiBuildTool.h"

struct FBuildElement
{
	string Name; //名字

	std::map<string, string> Metas;//
	std::vector<string> Fields;
};

//参数
struct FParamElement :public FBuildElement
{
	string DefaultValue;//如果参数有默认值 就是这个

	bool bConst;//是否是const
	bool bPointer;//是否是指针*
	bool bReference;//是否是引用&
	string Type;//float int ...

	//有可能是内部类型 类型可能是模板
	vector<FParamElement> InternalType;//类似模板 vector<float> or vector123<float,int,float>...

	FParamElement()
		: bPointer(false)
		, bReference(false)
		, bConst(false)
	{}
};

//变量
struct FVariableAnalysis :public FParamElement
{
	string CodeType;
	bool bStatic; //是否是静态

	FVariableAnalysis()
		:bStatic(false)
	{}
};

//函数解析
struct FFunctionAnalysis :public FBuildElement
{
	std::vector<FParamElement> ParamArray;//函数的静态值
	FParamElement Return;//返回类型

	bool bStatic;//静态
	bool bVirtual;//虚拟

	FFunctionAnalysis()
		:bStatic(false)
		, bVirtual(false)
	{
		Metas.insert({ "Category","Default" });
		Metas.insert({ "CodeType","" });
	}
};

//类解析成员
struct FClassAnalysis
{
	std::vector<FFunctionAnalysis> Function;//函数
	std::vector<FVariableAnalysis> Variable;//多少变量

	string APIName;//_API的名称
	string ClassName;//类名称

	std::vector<string> InheritName;//继承者名字

	string CodeCPPName;//CPP名字
	string Filename;//.h文件路径
	int CodeLine;//反射宏标记在哪行？

	string ModularPath;//E:\RenZhaiEngine\Source\RenZhaiBuildTool
};

enum ECollectionParmType
{
	Type_Return,//是返回
	Type_Parm,//参数
};

bool IsCheckAllowCodeReflection(const std::vector<std::string>& InContent);