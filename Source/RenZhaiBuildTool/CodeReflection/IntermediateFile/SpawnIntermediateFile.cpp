#include "SpawnIntermediateFile.h"

namespace IntermediateFile
{
	string GetTable(int Count = 1)
	{
		std::string Table;
		for (int i = 0; i < Count; i++)
		{
			Table += "\t";
		}

		return Table;
	}

	void ParsingVariables(
		std::vector<std::string>& AnalysisRaw, 
		const FParamElement* InParam,
		const std::string& InSupplementaryDesc = "NativeClass",
		bool bEmpty = false,
		int CounTable = 1)
	{
		if (string_equal(InParam->Type.c_str(), "void"))
		{
			return;
		}

		std::string Table = GetTable(CounTable);

		string PropertyStringType = "INPUT_PROPERTY";
		if (InParam->bReference)
		{
			if (!InParam->bConst)
			{
				PropertyStringType = "OUT_PROPERTY";
			}
		}
		else if (InParam->Name == "ReturnValue")
		{
			PropertyStringType = "RETURN_PROPERTY";
			if (InParam->Type == "void")
			{
				return;
			}
		}

		if (InParam->Type == "map")
		{
			if (InParam->InternalType.size() >= 2)
			{
				AnalysisRaw.push_back(
					simple_cpp_string_algorithm::printf(
						"%s%s.AddMapProperty(\
\"%s\",\"%s\",\"%s\",sizeof(%s),\"%s\",sizeof(%s)\
,0,sizeof(std::%s<%s,%s>),NULL,%s,\"%s\");",
Table.c_str(),
InSupplementaryDesc.c_str(),
InParam->Name.c_str(),
InParam->Type.c_str(),
InParam->InternalType[0].Type.c_str(),
InParam->InternalType[0].Type.c_str(),
InParam->InternalType[1].Type.c_str(),
InParam->InternalType[1].Type.c_str(),
InParam->Type.c_str(),
InParam->InternalType[0].Type.c_str(),
InParam->InternalType[1].Type.c_str(),
PropertyStringType.c_str(),
InParam->DefaultValue.c_str()));
			}
		}
		else if (InParam->Type == "vector")
		{
			if (InParam->InternalType.size() >= 1)
			{
				AnalysisRaw.push_back(
					simple_cpp_string_algorithm::printf(
						"%s%s.AddArrayProperty(\"%s\",\"%s\",\"%s\",sizeof(%s),0,sizeof(std::vector<%s>),NULL,%s,\"%s\");",
						Table.c_str(),
						InSupplementaryDesc.c_str(),
						InParam->Name.c_str(),
						InParam->Type.c_str(),
						InParam->InternalType[0].Type.c_str(),
						InParam->InternalType[0].Type.c_str(),
						InParam->InternalType[0].Type.c_str(),
						PropertyStringType.c_str(),
						InParam->DefaultValue.c_str()));
			}
		}
		else
		{
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"%s%s.AddProperty(\"%s\",\"%s\",sizeof(%s),%s,%s,\"%s\");",
					Table.c_str(),
					InSupplementaryDesc.c_str(),
					InParam->Name.c_str(),
					InParam->Type.c_str(),
					InParam->Type.c_str(),
					bEmpty ? "NULL" : ("&" + InParam->Name).c_str(),
					PropertyStringType.c_str(),
					InParam->DefaultValue.c_str()));
		}
	}

	void ParsingMeta(
		std::vector<std::string>& AnalysisRaw, 
		const FBuildElement* InParam, 
		const std::string& InSupplementaryDesc = "NativeClass",
		int CounTable = 1)
	{
		if (string_equal(InParam->Name.c_str(), "ReturnValue"))
		{
			return;
		}

		std::string Table = GetTable(CounTable);

		auto It = InParam->Metas.find("Category");
		if (It == InParam->Metas.end())
		{
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"%s%s.AddMetas(\"%s\",\"Category\",\"Default\");",
					Table.c_str(),
					InSupplementaryDesc.c_str(),
					InParam->Name.c_str()));
		}

		for (auto& SubTmp : InParam->Metas)
		{
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"%s%s.AddMetas(\"%s\",\"%s\",\"%s\");",
					Table.c_str(),
					InSupplementaryDesc.c_str(),
					InParam->Name.c_str(),
					SubTmp.first.c_str(),
					SubTmp.second.c_str()));
		}
	}

	void ParsingField(
		std::vector<std::string>& AnalysisRaw,
		const FBuildElement* InParam,
		const std::string& InSupplementaryDesc = "NativeClass",
		int CounTable = 1)
	{
		std::string Table = GetTable(CounTable);

		for (auto& SubTmp : InParam->Fields)
		{
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"%s%s.AddFields(\"%s\",\"%s\");",
					Table.c_str(),
					InSupplementaryDesc.c_str(),
					InParam->Name.c_str(), SubTmp.c_str()));
		}
	}

	void RemoveEnd(std::vector<std::string>& ParamArray,char EndChar)
	{
		if (simple_cpp_string_algorithm::index_valid(
			ParamArray.size(),
			int(ParamArray.size() - 1)))
		{
			remove_char_end(
				const_cast<char*>(ParamArray[ParamArray.size() - 1].c_str()),
				EndChar);
		}
	}

	const string* GetGOrCInheritName(const std::vector<string> &InheritName)
	{
		for (auto &Tmp: InheritName)
		{
			if (!Tmp.empty())
			{
				if (Tmp.c_str()[0] == 'C' || Tmp.c_str()[0] == 'G')
				{
					return &Tmp;
				}
			}
		}

		return NULL;
	}

	std::string GetCombineParmString(
		const FFunctionAnalysis& Function,
		std::vector<std::string>& ParamArray)
	{
		if (Function.ParamArray.size() == 0)
		{
			return "";
		}
		else
		{
			std::string ParamString;
			for (auto& Param : Function.ParamArray)
			{
				//int   c, float   b
				std::string NewParam =
					simple_cpp_string_algorithm::printf(
						"%s %s%s %s",
						string((Param.bConst ? ("const") : (""))).c_str(),
						Param.Type.c_str(),
						string((Param.bPointer ? ("* ") : (Param.bReference ? ("& ") : (" ")))).c_str(),
						Param.Name.c_str());

				//int32 A,
				ParamString += NewParam + (",");

				//int32 A,int32 B.int32 c
				ParamArray.push_back(NewParam);
			}

			remove_char_end(const_cast<char*>(ParamString.c_str()), ',');

			return ParamString;
		}	
	}

	void GeneratePointH(
		std::vector<std::string>& AnalysisRaw,
		const FClassAnalysis& ClassAnalysis, 
		std::vector<std::string>& StaticRegistration)
	{
		AnalysisRaw.push_back("#pragma once");
		AnalysisRaw.push_back("");

		AnalysisRaw.push_back("#include \"CoreObject/CoreMinimalObject.h\"");
		AnalysisRaw.push_back("#include \"CodeReflection/Frame.h\"");
		AnalysisRaw.push_back("#include \"CodeReflection/ScriptMacro.h\"");
		AnalysisRaw.push_back("");

		std::string MClassName = 
			simple_cpp_string_algorithm::printf(
		" Z_BT_%s", ClassAnalysis.ClassName.c_str());

		//#define  Z_BT_GActorObject
		AnalysisRaw.push_back(
			simple_cpp_string_algorithm::printf("#define %s \\", 
				MClassName.c_str()));

		AnalysisRaw.push_back("public: \\");

		//����
		std::string ClearClassName = ClassAnalysis.ClassName;
		{
			char* ClearClassNamePtr = const_cast<char*>(ClearClassName.c_str());
			trim_start_and_end_inline(ClearClassNamePtr);
			
			//�Ƴ�ͷ��C��ͷ����G��ͷ
			if (!remove_char_start(ClearClassNamePtr, 'G'))
			{
				remove_char_start(ClearClassNamePtr, 'C');
			}
		}

		if (ClassAnalysis.Function.size() > 0)
		{
			std::vector<FFunctionAnalysis>* InFunctionAnalysis = const_cast<std::vector<FFunctionAnalysis>*>(&ClassAnalysis.Function);
			for (FFunctionAnalysis& Function : *InFunctionAnalysis)
			{
				if (Function.Metas["CodeType"] == "Function" ||
					Function.Metas["CodeType"] == "PureFunction")
				{
					//Script_Hello1
					std::string VMString =
						simple_cpp_string_algorithm::printf(
							"Script_%s",
							Function.Name.c_str());

					//FUNCTION_DEFINITION(Script_Hello1) 
					AnalysisRaw.push_back(
						simple_cpp_string_algorithm::printf("FUNCTION_DEFINITION(%s) \\", VMString.c_str()));
				
					AnalysisRaw.push_back("{ \\");
					{
						//a,b,c,d
						std::string VariableAdd;
						//��ƴ�Ӻ����Ĳ���
						for (const FParamElement& Variable : Function.ParamArray)
						{
							std::string StackString = " Stack.GetParmAddr(); \\";

							std::string VariableName =
								simple_cpp_string_algorithm::printf(
									" Z_%s_Name",
									Variable.Name.c_str());

							//float Z_a = *(float*)Stack.GetParmAddr();
							AnalysisRaw.push_back(
								simple_cpp_string_algorithm::printf(
									"\t%s%s %s = %s(%s*)%s",
									Variable.Type.c_str(),
									string(Variable.bPointer ? "*" : "").c_str(),
									VariableName.c_str(),
									std::string(Variable.bPointer ? "" : "*").c_str(),
									Variable.Type.c_str(),
									StackString.c_str()));

							//,Z_Context_Name, Z_A_Name, Z_b_Name, Z_C_Name
							VariableAdd += (","+ VariableName);
						}
						//�Ƴ���ǰ����ַ���
						char* VariableAddPtr = const_cast<char*>(VariableAdd.c_str());
						remove_char_start(VariableAddPtr, ',');

						//������
						if (Function.bStatic)
						{
							if (Function.Return.Type == "void")
							{
								//GActorObject::Hello1(Z_a,Z_b); 
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\t%s::%s(%s); \\",
										ClassAnalysis.ClassName.c_str(),
										Function.Name.c_str(),
										VariableAdd.c_str()));
							}
							else
							{
								/**(std::string*)RefData = GActorObject::Hello3( Z_Context_Name); \*/
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\t%s(%s*) RefData = %s::%s(%s); \\",
										string(Function.Return.bPointer ? "" : "*").c_str(),
										Function.Return.Type.c_str(),
										ClassAnalysis.ClassName.c_str(),
										Function.Name.c_str(),
										VariableAdd.c_str()));
							}
						}
						else //�����Ա����
						{
							if (Function.Return.Type == "void")
							{
								//GActorObject::Hello1(Z_a,Z_b); 
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\t((%s*)Context)->%s(%s); \\",
										ClassAnalysis.ClassName.c_str(),
										Function.Name.c_str(),
										VariableAdd.c_str()));
							}
							else
							{
								/**(std::string*)RefData = GActorObject::Hello3( Z_Context_Name); \*/
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\t%s(%s*) RefData = ((%s*)Context)->%s(%s); \\",
										string(Function.Return.bPointer ? "" : "*").c_str(),
										Function.Return.Type.c_str(),
										ClassAnalysis.ClassName.c_str(),
										Function.Name.c_str(),
										VariableAdd.c_str()));
							}
						}
					}
					AnalysisRaw.push_back("} \\");

					//�ռ���̬ע��
					//FFuntionManage::SetNativeFuncPtr(FFuntionID(("ActorObject"),("Hello1"),GActorObject::Script_Hello1));
					StaticRegistration.push_back(
						simple_cpp_string_algorithm::printf(
							"\tFFuntionManage::Add(FFuntionID((\"%s\"),(\"%s\"),%s::%s));",
							ClearClassName.c_str(),
							Function.Name.c_str(),
							ClassAnalysis.ClassName.c_str(),
							VMString.c_str()));
				}
			}

			//�Ƴ�����ƴ�ӵ� "\"
			//RemoveEnd(AnalysisRaw,'\\');
		}
		AnalysisRaw.push_back("private:");

		AnalysisRaw.push_back((""));

		//�ϲ��ڲ��ĺ���
		//GActorObject_10_Internal_Fun
		std::string InternalFunMacro = simple_cpp_string_algorithm::printf(
			"%s_%i_Internal_Fun",
			ClassAnalysis.ClassName.c_str(),
			ClassAnalysis.CodeLine);

		//#define GActorObject_10_Internal_Fun 
		AnalysisRaw.push_back(
			simple_cpp_string_algorithm::printf(
				"#define %s \\",
				InternalFunMacro.c_str()));

		if (const string* InheritName = GetGOrCInheritName(ClassAnalysis.InheritName))
		{
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"typedef %s Super; \\",
					InheritName->c_str()));
		}
		AnalysisRaw.push_back("public: \\");

		AnalysisRaw.push_back(
			simple_cpp_string_algorithm::printf(
				"%s(int InParamA):Super(InParamA){} \\",
				ClassAnalysis.ClassName.c_str()));

		AnalysisRaw.push_back("virtual void ExecutionEventScript(const string &EventName,void *Param = nullptr); \\");
		AnalysisRaw.push_back("virtual bool UpdateEditorContainerPropertyDetails(class CPropertyObject* InProperty); \\");
		AnalysisRaw.push_back("static class CClassObject* GetClass(); \\");	
		AnalysisRaw.push_back("virtual CClassObject* Class();\\");
		AnalysisRaw.push_back("protected: \\");
		AnalysisRaw.push_back("virtual void InitReflectionContent(); \\");
		AnalysisRaw.push_back("private:");

		AnalysisRaw.push_back("");

		//#define GActorObject_10_GENERATED_BODY_BT 
		AnalysisRaw.push_back(
			simple_cpp_string_algorithm::printf(
				"#define %s_%i_GENERATED_BODY_BT \\",
				ClassAnalysis.ClassName.c_str(),
				ClassAnalysis.CodeLine));

		// Z_BT_GActorObject
		AnalysisRaw.push_back(simple_cpp_string_algorithm::printf(
			"%s \\", MClassName.c_str()));
		AnalysisRaw.push_back(simple_cpp_string_algorithm::printf(
			"%s ", InternalFunMacro.c_str()));

		AnalysisRaw.push_back((""));

		//���ǰ��Ķ���
		/*
		#ifdef CURRENT_FILE_ID_BT
		#undef CURRENT_FILE_ID_BT
		#endif // CURRENT_FILE_ID_BT

		#ifdef NewLine
		#undef NewLine
		#endif
		*/
		AnalysisRaw.push_back("#ifdef CURRENT_FILE_ID_BT");
		AnalysisRaw.push_back("#undef CURRENT_FILE_ID_BT");
		AnalysisRaw.push_back("#endif // CURRENT_FILE_ID_BT");

		AnalysisRaw.push_back("");

		AnalysisRaw.push_back("#ifdef CodeReflectionTagLine");
		AnalysisRaw.push_back("#undef CodeReflectionTagLine");
		AnalysisRaw.push_back("#endif // NewLine");

		AnalysisRaw.push_back("");

		//#define CURRENT_FILE_ID_BT  GActorObject
		AnalysisRaw.push_back(
			simple_cpp_string_algorithm::printf(
				"#define %s %s",
				std::string("CURRENT_FILE_ID_BT ").c_str(),
				ClassAnalysis.ClassName.c_str()));

		//#define NewLine 10
		AnalysisRaw.push_back(
			simple_cpp_string_algorithm::printf(
				"#define %s %i",
				std::string("CodeReflectionTagLine").c_str(),
				ClassAnalysis.CodeLine));
	}

	void GeneratePointCpp(
		std::vector<std::string>& AnalysisRaw, 
		const FClassAnalysis& ClassAnalysis,
		std::vector<std::string>& StaticRegistration)
	{
		AnalysisRaw.push_back("// Copyright (C) RenZhai.2022.All Rights Reserved..");
		AnalysisRaw.push_back("/*===========================================================================");
		AnalysisRaw.push_back("	Generated code exported from c f.");
		AnalysisRaw.push_back("===========================================================================*/");
		
		//�ļ��ľ���·��
		AnalysisRaw.push_back(simple_cpp_string_algorithm::printf(
			"#include \"%s\"",
			ClassAnalysis.Filename.c_str()));

		//�����.h
		//AnalysisRaw.push_back(simple_cpp_string_algorithm::printf(
		//	"#include \"%s.CodeReflection.h\"",
		//	ClassAnalysis.CodeCPPName.c_str()));

		AnalysisRaw.push_back("#include \"CodeReflection/FunctionManage.h\"");
		AnalysisRaw.push_back("#include \"CoreMacro.h\"");
		AnalysisRaw.push_back("#include \"CoreObject/PropertyObject.h\"");

		AnalysisRaw.push_back("#include \"CoreObject/FunctionObject.h\"");
		AnalysisRaw.push_back("#include \"CoreObject/ClassObject.h\"");
		AnalysisRaw.push_back("#include \"CodeReflection/ClassManage.h\"");
		AnalysisRaw.push_back("#include \"Construction/ObjectConstruction.h\"");

		AnalysisRaw.push_back("");
		AnalysisRaw.push_back("#ifdef _MSC_VER");
		AnalysisRaw.push_back("#pragma warning (push)");
		AnalysisRaw.push_back("#pragma warning (disable : 4883)");
		AnalysisRaw.push_back("#endif");
	
		AnalysisRaw.push_back("");
		//���붨����
		{
			//������
			if (ClassAnalysis.Function.size() > 0)
			{
				std::vector<FFunctionAnalysis>* InFunctionAnalysis = const_cast<std::vector<FFunctionAnalysis>*>(&ClassAnalysis.Function);
				for (FFunctionAnalysis& Function : *InFunctionAnalysis)
				{
					if (Function.Metas["CodeType"] == "Event")
					{
						//Name_Hello123
						std::string FunctionName =
							simple_cpp_string_algorithm::printf("Name_%s",
								Function.Name.c_str());

						std::vector<std::string> ParamStr;
						//static std::string Name_Hello123 = std::string(("Hello123"));v
						AnalysisRaw.push_back(
							simple_cpp_string_algorithm::printf(
								"static std::string %s = std::string((\"%s\"));",
								FunctionName.c_str(),
								Function.Name.c_str()
							));

						//void   ActorObject::Hello123( int   c, float   b)
						AnalysisRaw.push_back(
							simple_cpp_string_algorithm::printf(
								"%s %s %s::%s(%s)",
								Function.Return.Type.c_str(),
								string((Function.Return.bPointer ? ("*") : (Function.Return.bReference ? ("&") : (" ")))).c_str(),
								ClassAnalysis.ClassName.c_str(),
								Function.Name.c_str(),
								GetCombineParmString(Function,ParamStr).c_str()
							));

						AnalysisRaw.push_back(("{"));
						{
							//Parm_Hello123
							std::string StructName =
								simple_cpp_string_algorithm::printf(
									"Parm_%s",
									Function.Name.c_str());

							if (Function.ParamArray.size() > 0)
							{
								//struct FParm_Hello123
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf("\tstruct F%s",
										StructName.c_str()));
								AnalysisRaw.push_back(std::string(("\t{")));//{
								{
									for (auto& Param : ParamStr)
									{
										AnalysisRaw.push_back(
											simple_cpp_string_algorithm::printf("\t\t%s;",
												Param.c_str()));
									}
								}
								AnalysisRaw.push_back(std::string(("\t};")));//};

								//FParm_Hello123 Parm_Hello123;
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\tF%s %s;",
										StructName.c_str(),
										StructName.c_str()));

								//��ֵ
								{
									//Parm_Hello123.c = c;
									for (auto& Param : Function.ParamArray)
									{
										AnalysisRaw.push_back(
											simple_cpp_string_algorithm::printf(
												"\t%s.%s = %s;",
												StructName.c_str(),
												Param.Name.c_str(),
												Param.Name.c_str()));
									}
								}
							}

							//ExecutionScript(FindScriptFuntion(Name_Hello123), &Parm_Hello123);
							AnalysisRaw.push_back(
								simple_cpp_string_algorithm::printf(
									"\tExecutionScript(%s(%s),%s);",
									Function.bStatic ? "FindScriptStaticFuntion" : "FindScriptFuntion",
									FunctionName.c_str(),
									string(Function.ParamArray.size() == 0 ? ("NULL") : (("&") + StructName)).c_str()
								));
						}
						AnalysisRaw.push_back(("} "));
					}
				}
			}

			AnalysisRaw.push_back((""));

			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"bool %s::UpdateEditorContainerPropertyDetails(CPropertyObject* InProperty)",
					ClassAnalysis.ClassName.c_str()));
			AnalysisRaw.push_back(("{"));
			{
				AnalysisRaw.push_back("\tSuper::UpdateEditorContainerPropertyDetails(InProperty);");
			
				AnalysisRaw.push_back((""));
				AnalysisRaw.push_back(("\tif (InProperty)"));
				AnalysisRaw.push_back(("\t{"));
				{
					bool bSpawnArray = false;
					for (auto& Tmp : ClassAnalysis.Variable)
					{
						if (Tmp.Type == "vector")
						{
							if (Tmp.InternalType.size() >= 1)
							{
								bSpawnArray = true;

								AnalysisRaw.push_back(("\t\t//Let's start working with the vector container."));

								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\t\tif (InProperty->GetName() == \"%s\")",
										Tmp.Name.c_str()));

								AnalysisRaw.push_back(("\t\t{"));
								{
									//	TTTT.clear();
									AnalysisRaw.push_back(
										simple_cpp_string_algorithm::printf(
											"\t\t\t%s.clear();",
											Tmp.Name.c_str()));

									AnalysisRaw.push_back((""));
									AnalysisRaw.push_back(("\t\t\tCPropertyObject *ValueObject = InProperty->GetNativeClass().Property;"));
									AnalysisRaw.push_back(("\t\t\twhile (ValueObject)"));
									AnalysisRaw.push_back(("\t\t\t{"));
									{
										AnalysisRaw.push_back(
											simple_cpp_string_algorithm::printf(
												"\t\t\t\t%s.push_back(*ValueObject->GetData<%s>());",
												Tmp.Name.c_str(),
												Tmp.InternalType[0].Type.c_str()));

										AnalysisRaw.push_back(("\t\t\t\tValueObject = dynamic_cast<CPropertyObject*>(ValueObject->Nest);"));
									}
									AnalysisRaw.push_back(("\t\t\t}"));
								}
								AnalysisRaw.push_back(("\t\t}"));
								AnalysisRaw.push_back(("\t\telse"));
							}
						}
						else if (Tmp.Type == "map")
						{
							if (Tmp.InternalType.size() >= 2)
							{
								AnalysisRaw.push_back(("\t\t//Let's start working with the map container."));
							
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\t\tif (InProperty->GetName() == \"%s\")",
										Tmp.Name.c_str()));
								AnalysisRaw.push_back(("\t\t{"));
								{
									AnalysisRaw.push_back(
										simple_cpp_string_algorithm::printf(
											"\t\t\t%s.clear();",
											Tmp.Name.c_str()));

									AnalysisRaw.push_back((""));

									AnalysisRaw.push_back(("\t\t\tCPropertyObject* KeyObject = InProperty->GetNativeClass().Property;"));
									AnalysisRaw.push_back(("\t\t\twhile (KeyObject)"));
									AnalysisRaw.push_back(("\t\t\t{"));
									{
										AnalysisRaw.push_back(("\t\t\t\tCPropertyObject *ValueObject = dynamic_cast<CPropertyObject*>(KeyObject->Nest);"));
									
										AnalysisRaw.push_back(
											simple_cpp_string_algorithm::printf(
												"\t\t\t\t%s.insert({*KeyObject->GetData<%s>(),*ValueObject->GetData<%s>()});",
												Tmp.Name.c_str(),
												Tmp.InternalType[0].Type.c_str(),
												Tmp.InternalType[1].Type.c_str()));
									
										AnalysisRaw.push_back((""));

										AnalysisRaw.push_back(("\t\t\t\tKeyObject = dynamic_cast<CPropertyObject*>(ValueObject->Nest);"));
									}
									AnalysisRaw.push_back(("\t\t\t}"));
								}
								AnalysisRaw.push_back(("\t\t}"));
								AnalysisRaw.push_back(("\t\telse"));
							}
						}
					}

					if (bSpawnArray)
					{
						AnalysisRaw.pop_back();
					}

					AnalysisRaw.push_back((""));
					AnalysisRaw.push_back(("\t\treturn true;"));
				}

				AnalysisRaw.push_back(("\t}"));
				AnalysisRaw.push_back((""));
				AnalysisRaw.push_back(("\treturn false;"));
			}
			AnalysisRaw.push_back(("}"));
			AnalysisRaw.push_back((""));

			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"void %s::ExecutionEventScript(const string& EventName, void* Param)",
					ClassAnalysis.ClassName.c_str()));
			AnalysisRaw.push_back(("{"));
			{
				AnalysisRaw.push_back("\tSuper::ExecutionEventScript(EventName,Param);");
				AnalysisRaw.push_back((""));

				std::vector<FFunctionAnalysis>* InFunctionAnalysis = const_cast<std::vector<FFunctionAnalysis>*>(&ClassAnalysis.Function);
				
				bool SpawnEnvent = false;
				for (auto& Tmp : *InFunctionAnalysis)
				{
					if (Tmp.Metas["CodeType"] == "Event")
					{
						if (Tmp.Metas.find("DisplayName") != Tmp.Metas.end())
						{
							AnalysisRaw.push_back(
								simple_cpp_string_algorithm::printf(
									"\tif (EventName == \"%s\" || EventName == \"%s\")",
									Tmp.Name.c_str(),
									Tmp.Metas["DisplayName"].c_str()));
						}
						else
						{
							AnalysisRaw.push_back(
								simple_cpp_string_algorithm::printf(
									"\tif (EventName == \"%s\")",
									Tmp.Name.c_str()));
						}

						AnalysisRaw.push_back(("\t{"));
						{
							AnalysisRaw.push_back(("\t\tstruct FEventParamStruct"));
							AnalysisRaw.push_back(("\t\t{"));
							for (auto& TmpParam : Tmp.ParamArray)
							{
								AnalysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\t\t\t%s%s%s;",
										TmpParam.Type.c_str(),
										TmpParam.bPointer ? "*" : " ",
										TmpParam.Name.c_str()));
							}
							AnalysisRaw.push_back(("\t\t};"));
							AnalysisRaw.push_back((""));
							AnalysisRaw.push_back(("\t\tFEventParamStruct EventParam;"));
							AnalysisRaw.push_back((""));
							AnalysisRaw.push_back(("\t\tif(Param)"));
							AnalysisRaw.push_back(("\t\t{"));
							{
								AnalysisRaw.push_back(("\t\t\tEventParam = *(FEventParamStruct*)Param;"));
							}

							string InParamString;
							for (auto& TmpParam : Tmp.ParamArray)
							{
								InParamString += simple_cpp_string_algorithm::printf(
									"EventParam.%s,",
									TmpParam.Name.c_str());
							}

							if (!InParamString.empty())
							{
								remove_char_end(const_cast<char*>(InParamString.c_str()), ',');
							}

							AnalysisRaw.push_back(("\t\t}"));

							AnalysisRaw.push_back((""));

							AnalysisRaw.push_back(
								simple_cpp_string_algorithm::printf(
									"\t\t%s(%s);",
									Tmp.Name.c_str(),
									InParamString.c_str()));

						}
						AnalysisRaw.push_back(("\t}"));
					}
				}
			}
			AnalysisRaw.push_back(("}"));

			AnalysisRaw.push_back((""));

			//void GActorObject::InitReflectionContent()
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
				"void %s::InitReflectionContent()",
					ClassAnalysis.ClassName.c_str()));
			AnalysisRaw.push_back("{");
			{
				AnalysisRaw.push_back("\tSuper::InitReflectionContent();");

				AnalysisRaw.push_back("");

				//Rename("ActorObject");
				AnalysisRaw.push_back(
					simple_cpp_string_algorithm::printf(
						"\tRename(\"%s\");",
						ClassAnalysis.CodeCPPName.c_str()));

				if (ClassAnalysis.Variable.size() > 0)
				{
					AnalysisRaw.push_back("");

					//��ӱ���
					//AddProperty
					for (auto& Tmp : ClassAnalysis.Variable)
					{
						ParsingVariables(AnalysisRaw, &Tmp);
					}

					AnalysisRaw.push_back("");

					//����Ԫ����
					//NativeClass.AddMetas
					AnalysisRaw.push_back("#if EDITOR_ENGINE");
					for (auto& Tmp : ClassAnalysis.Variable)
					{
						ParsingMeta(AnalysisRaw, &Tmp);
					}

					AnalysisRaw.push_back("");

					//�����ֶ�
					for (auto& Tmp : ClassAnalysis.Variable)
					{
						ParsingField(AnalysisRaw,&Tmp);
					}				
					AnalysisRaw.push_back("#endif // EDITOR_ENGINE");

					AnalysisRaw.push_back("");
				}
			}

			AnalysisRaw.push_back("#if EDITOR_ENGINE");
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"\tNativeClass.AddClassType(\"%s\");",
					ClassAnalysis.ClassName.c_str()));
			AnalysisRaw.push_back("#endif // EDITOR_ENGINE");

			AnalysisRaw.push_back((""));

			for (auto& Tmp : ClassAnalysis.Function)
			{
				std::string FuncType = Tmp.bStatic ? "AddStaticFunction" : "AddFunction";
			
				AnalysisRaw.push_back(
					simple_cpp_string_algorithm::printf(
						"\tif (CFunctionObject *InFunction = NativeClass.%s(\"%s\"))",
						FuncType.c_str(),
						Tmp.Name.c_str()));
				AnalysisRaw.push_back("\t{");
				{
					string GetNativeClassString = "InFunction->GetNativeClass()";

					ParsingMeta(AnalysisRaw, &Tmp, GetNativeClassString, 2);
					ParsingField(AnalysisRaw, &Tmp, GetNativeClassString, 2);

					AnalysisRaw.push_back("");

					//������ؽ���
					for (auto& ParamTmp : Tmp.ParamArray)
					{
						ParsingVariables(
							AnalysisRaw,
							&ParamTmp, GetNativeClassString, true, 2);

						ParsingMeta(AnalysisRaw, &ParamTmp, GetNativeClassString, 2);
						ParsingField(AnalysisRaw, &ParamTmp, GetNativeClassString, 2);

						AnalysisRaw.push_back("");
					}

					//����ֵ
					ParsingVariables(
						AnalysisRaw,
						&Tmp.Return,GetNativeClassString, true, 2);

					ParsingMeta(AnalysisRaw, &Tmp.Return, GetNativeClassString, 2);
					ParsingField(AnalysisRaw, &Tmp.Return, GetNativeClassString, 2);
				}
				AnalysisRaw.push_back("\t}");
			}

			AnalysisRaw.push_back("}");

			AnalysisRaw.push_back((""));

			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf("CClassObject* %s::Class()",
					ClassAnalysis.ClassName.c_str()));
			AnalysisRaw.push_back(("{"));
			{
				AnalysisRaw.push_back(
					simple_cpp_string_algorithm::printf("\treturn %s::GetClass();",
						ClassAnalysis.ClassName.c_str()));
			}
			AnalysisRaw.push_back(("}"));
			AnalysisRaw.push_back((""));

			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf("CClassObject* %s::GetClass()",
					ClassAnalysis.ClassName.c_str()));
			AnalysisRaw.push_back(("{"));
			{
				AnalysisRaw.push_back(("\tstatic CClassObject* ClassObject = nullptr;"));
				AnalysisRaw.push_back(("\tif (!ClassObject)"));
				AnalysisRaw.push_back(("\t{"));
				{
					AnalysisRaw.push_back(("\t\tFCreateObjectParam ClassParam;"));
					AnalysisRaw.push_back(("\t\tClassObject = CreateObject<CClassObject>(ClassParam, new CClassObject(0));"));
					AnalysisRaw.push_back(
						simple_cpp_string_algorithm::printf("\t\tClassObject->Rename(\"%s\");",
							ClassAnalysis.CodeCPPName.c_str()));

					AnalysisRaw.push_back((""));

					AnalysisRaw.push_back(("\t\tClassObject->RegisterClassObjectDelegate.BindLambda([&]()->CCoreMinimalObject*"));
					AnalysisRaw.push_back(("\t\t{"));
					{
						AnalysisRaw.push_back(
							simple_cpp_string_algorithm::printf("\t\t\treturn CreateObject<%s>(FCreateObjectParam(), new %s(0));",
								ClassAnalysis.ClassName.c_str(),
								ClassAnalysis.ClassName.c_str()));
					}
					AnalysisRaw.push_back(("\t\t});"));
				}
				AnalysisRaw.push_back(("\t}"));
				AnalysisRaw.push_back((""));
				AnalysisRaw.push_back(("\treturn ClassObject;"));
			}
			AnalysisRaw.push_back(("}"));

			AnalysisRaw.push_back((""));

			AnalysisRaw.push_back(("/* 1xxxx xxxx "));
			AnalysisRaw.push_back((" 2xxxx xxxx */"));
			//Register_ActorObject
			std::string Register_Func =
				simple_cpp_string_algorithm::printf("Register_%s()",
					ClassAnalysis.ClassName.c_str());
			//int Register_ActorObject()
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf("int %s", Register_Func.c_str()));
			AnalysisRaw.push_back(("{"));
			{
				//�ϲ�����vector
				AnalysisRaw.insert(
					AnalysisRaw.end(),
					StaticRegistration.begin(),
					StaticRegistration.end());

				AnalysisRaw.push_back((""));

				AnalysisRaw.push_back(
					simple_cpp_string_algorithm::printf(
						"\tFClassManage::Add(FClassID(\"%s\",%s::GetClass()));",
						ClassAnalysis.CodeCPPName.c_str(),
						ClassAnalysis.ClassName.c_str()));

				AnalysisRaw.push_back((""));
				AnalysisRaw.push_back(("\treturn 0;"));
			}
			AnalysisRaw.push_back(("}"));

			//static int ActorObject_Index = Register_ActorObject();
			AnalysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"static int %s_Index = %s;",
					ClassAnalysis.ClassName.c_str(),
					Register_Func.c_str()));
		}

		AnalysisRaw.push_back("");

		AnalysisRaw.push_back("#ifdef _MSC_VER");
		AnalysisRaw.push_back("#pragma warning (pop)");
		AnalysisRaw.push_back("#endif");
	}

	bool Builder(
		const FClassAnalysis& InClassAnalysis,
		std::vector<std::string>& OutAnalysisRawH,
		std::vector<std::string>& OutAnalysisRawCPP)
	{
		vector<string> StaticRegistration;

		//���ɷ�������
		GeneratePointH(OutAnalysisRawH, InClassAnalysis, StaticRegistration);
		GeneratePointCpp(OutAnalysisRawCPP, InClassAnalysis, StaticRegistration);

		return OutAnalysisRawH.size() && OutAnalysisRawH.size();
	}
}

/*
* .h���ɵĴ���
#pragma once

#include "CoreObject/CoreMinimalObject.h"
#include "CodeReflection/Frame.h"
#include "CodeReflection/ScriptMacro.h"

#define  Z_BT_GActorObject \
FUNCTION_DEFINITION(Script_Hello1) \
{ \
	float Z_a = *(float*)Stack.GetParmAddr();\
	float Z_b = *(float*)Stack.GetParmAddr();\
	GActorObject::Hello1(Z_a,Z_b); \
} \
FUNCTION_DEFINITION(Script_Hello2) \
{ \
	GActorObject::Hello2(); \
}

#define GActorObject_10_GENERATED_BODY_BT \
 Z_BT_GActorObject

#define CURRENT_FILE_ID_BT  GActorObject
#define NewLine 10
*/