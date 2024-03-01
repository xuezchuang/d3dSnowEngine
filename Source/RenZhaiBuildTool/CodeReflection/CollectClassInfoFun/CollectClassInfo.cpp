#include "CollectClassInfo.h"

namespace CollectClassInfo
{
	const char SpaceString[] = " ";
	const char ColonString[] = ":";
	const char CommaString[] = ",";
	const char LeftParenthesisString[] = "(";
	const char StarString[] = "*";
	const char FetchAddressString[] = "&";
	const char CodeType[] = "CodeType";
	const char EqualSign[] = "=";
	const char Colon[] = "\"";

	//CVARIABLE(CodeType = Resources,aaaa="asdasdas",www=dasdasd,xxxxx,wwww,ddd)
	void ProcessingReflexParameters(
		const string& InRowStrong,
		FBuildElement* InBuildElement)
	{
		//InRowStrong = "CVARIABLE(CodeType = Resources,aaaa="asdasdas",www=dasdasd,xxxxx,wwww,ddd)"
		char* Ptr = const_cast<char*>(InRowStrong.c_str());

		char R[1024] = { 0 };
		char L[1024] = { 0 };

		//L = CVARIABLE
		//  CodeType = Resources,aaaa="asdasdas",www=dasdasd,xxxxx,wwww,ddd)  
		split(Ptr, LeftParenthesisString, L, R, false);

		//�޼��ո�
		//CodeType = Resources,aaaa="asdasdas",www=dasdasd,xxxxx,wwww,ddd)
		trim_start_and_end_inline(R);

		//CodeType = Resources,aaaa="asdasdas",www=dasdasd,xxxxx,wwww,ddd
		remove_char_end(R, ')');

		//�ö��Ű������п�
		vector<string> ElementStr;
		simple_cpp_string_algorithm::parse_into_vector_array(R, ElementStr, CommaString);

		/*
		CodeType = Resources
		aaaa="asdasdas"
		www=dasdasd
		xxxxx  
		wwww
		ddd
		*/
		for (auto& Tmp : ElementStr)
		{
			char* RowPtr = const_cast<char*>(Tmp.c_str());
			//�ж��Ƿ�ΪԪ����
			if (simple_cpp_string_algorithm::string_contain(Tmp, EqualSign))
			{
				//CodeType = Resources
				char EqualR[1024] = { 0 };
				char EqualL[1024] = { 0 };

				split(RowPtr, EqualSign, EqualL, EqualR, false);

				//EqualL = CodeType
				//EqualR = Resources
				trim_start_and_end_inline(EqualL);

				if (simple_cpp_string_algorithm::string_contain(EqualR, Colon))
				{
					remove_all_char_end(EqualR,'\"');
				}
				// Resources
				trim_start_and_end_inline(EqualR);
				InBuildElement->Metas[EqualL] = EqualR;
			}
			else //���ֶ�
			{
				trim_start_and_end_inline(RowPtr);
				InBuildElement->Fields.push_back(RowPtr);
			}
		}
	}

	bool GetCodeTypeByFunc(
		const string& RowStrong, 
		FFunctionAnalysis& FunctionAnalysis)
	{
		char* Ptr = const_cast<char*>(RowStrong.c_str());

		char R[1024] = { 0 };
		char L[1024] = { 0 };
		//UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
		//L =  "Describe", Group = "SimpleCodeLibrary"))
		//R = UFUNCTION(Meta = (

		split(Ptr, CodeType, R, L, false);

		vector<string> ElementStr;
		simple_cpp_string_algorithm::parse_into_vector_array(L, ElementStr, ",");

		if (simple_cpp_string_algorithm::string_contain(ElementStr[0],"Event"))
		{
			FunctionAnalysis.Metas["CodeType"] = "Event";

			return true;
		}
		else if (simple_cpp_string_algorithm::string_contain(ElementStr[0], "PureFunction"))
		{
			FunctionAnalysis.Metas["CodeType"] = "PureFunction";

			return true;
		}
		else if (simple_cpp_string_algorithm::string_contain(ElementStr[0], "Function"))
		{
			FunctionAnalysis.Metas["CodeType"] = "Function";

			return true;
		}
		
		return false;
	}

	FParamElement CollectionVariableType(
		std::string Variable,
		ECollectionParmType CollectionParmType = ECollectionParmType::Type_Parm)
	{
		char* Ptr = const_cast<char*>(Variable.c_str());

		//int & 
		FParamElement ParamElement;
		ParamElement.Name = "ReturnValue";

		if (simple_cpp_string_algorithm::string_contain(Variable, StarString))
		{
			remove_char_end(Ptr, '*');
			ParamElement.bPointer = true;
		}

		if (simple_cpp_string_algorithm::string_contain(Variable, FetchAddressString))
		{
			remove_char_end(Ptr, '&');
			ParamElement.bReference = true;
		}

		//int  
		if (simple_cpp_string_algorithm::string_contain(Variable, "const"))
		{
			trim_start_and_end_inline(Ptr);

			remove_string_start(Ptr, "const");
			ParamElement.bConst = true;
		}

		trim_start_and_end_inline(Ptr);

		remove_char_start(const_cast<char*>(Variable.c_str()), '\t');
		ParamElement.Type = Variable;

		return ParamElement;
	}

	bool GetCodeTypeByProp(
		const string& RowStrong,
		FVariableAnalysis* VariableAnalysis)
	{
		//RowStrong = UPROPERTY(Meta = (CodeType = "Resources", Group = "SimpleCodeLibrary"))
	
		char R[1024] = { 0 };
		char L[1024] = { 0 };

		char* Ptr = const_cast<char*>(RowStrong.c_str());
		split(Ptr, CodeType, R, L, false);

		//R =  UPROPERTY(Meta = (
		//L =  = "Resources", Group = "SimpleCodeLibrary"))

		vector<string> ElementStr;
		simple_cpp_string_algorithm::parse_into_vector_array(L, ElementStr, CommaString);
	
		// "Resources" 0 
		// Group = "SimpleCodeLibrary")) 1
		if (simple_cpp_string_algorithm::string_contain(ElementStr[0], "Resources"))
		{
			VariableAnalysis->CodeType = "Resources";
			return true;
		}

		return false;
	}

	//Vector123124<float,int,float,int> float,int,float,int xx,xx,xx,xx
	//void XXXX(xx,xx,xx,xx)?
	void AnalyticParameters(char* LStr, std::vector<FParamElement>& OutParam)
	{
		//���������Ͳ�����
		vector<string> ElementStr;
		simple_cpp_string_algorithm::parse_into_vector_array(LStr, ElementStr, CommaString);

		//UObject *Context
		//int32 &A
		//float b
		//bool C

		//�ռ�����
		for (std::string& Ele : ElementStr)
		{
			char* ElePtr = const_cast<char*>(Ele.c_str());

			if (Ele == "")
			{
				continue;
			}

			//int32 &A
			//�Ƴ�ǰ��ո�
			trim_start_and_end_inline(ElePtr);

			OutParam.push_back(FParamElement());
			FParamElement &ParamElement = OutParam[OutParam.size() - 1];

			char R[1024] = { 0 };
			char L[1024] = { 0 };
			if (simple_cpp_string_algorithm::string_contain(Ele, StarString))
			{
				ParamElement.bPointer = true;
				//GObject *Context
				split(ElePtr, StarString, R, L, false);
				//R = GObject
				//L = Context
			}
			else if (simple_cpp_string_algorithm::string_contain(Ele, FetchAddressString))
			{
				ParamElement.bReference = true;
				split(ElePtr, FetchAddressString, R, L, false);
			}
			else
			{
				//  int a
				split(ElePtr, SpaceString, R, L, false);
				if (R[0] == '\0')
				{
					strcpy(R, ElePtr);
				}
			}

			if (c_str_contain(R, "const"))
			{
				//const GObject *Context
				ParamElement.bConst = true;

				remove_string_start(R, "const");
			}

			//�п�����Ĭ�ϲ���
			if (c_str_contain(L, EqualSign))
			{
				char R1[1024] = { 0 };
				char L1[1024] = { 0 };
				split(L, EqualSign, L1, R1, false);

				trim_start_and_end_inline(L1);
				trim_start_and_end_inline(R1);

				ParamElement.Name = L1;
				ParamElement.DefaultValue = R1;
			}
			else
			{
				trim_start_and_end_inline(L);
				ParamElement.Name = L;
			}

			trim_start_and_end_inline(R);

			ParamElement.Type = R;
		}
	}

	bool Collection(const string& Paths, FClassAnalysis& ClassAnalysis)
	{
		std::vector<std::string> StringArray;
		simple_cpp_helper_file::load_file_to_strings(Paths, StringArray);

		//�ռ�filname
		ClassAnalysis.Filename = Paths;

		//����ÿһ�д���
		for (int i = 0; i < StringArray.size(); i++)
		{
			string& Row = StringArray[i];
			char* RowPtr = const_cast<char*>(Row.c_str());

			//����
			auto Contain = [&](const char* InSubString)->bool
			{
				return simple_cpp_string_algorithm::string_contain(Row, InSubString);
			};

			if (Contain("CODEREFLECTION"))
			{
				ClassAnalysis.CodeLine = i + 1;
			}

			//��ȡ�����ͼ̳���
			if ((Contain("\tclass") || Contain("class")) &&
				Contain(":") &&
				(Contain("protected") || Contain("private") || Contain("public")))
			{
				//class FRenderingPipeline :public IDirectXDeviceInterfece
				//FRenderingPipeline :public IDirectXDeviceInterfece
				remove_string_start(RowPtr, "class ");
				remove_string_start(RowPtr, "\tclass ");

				if (Contain("_API"))
				{
					//XXX_API FRenderingPipeline :public IDirectXDeviceInterfece
					//L ="XXX_API" R = " FRenderingPipeline :public IDirectXDeviceInterfece"
					trim_start_inline(RowPtr);

					char R[1024] = { 0 };
					char L[1024] = { 0 };

					split(RowPtr, SpaceString, L, R, false);

					//API����
					ClassAnalysis.APIName = L;

					Row = R;
				}

				vector<string> ElementStr;
				simple_cpp_string_algorithm::parse_into_vector_array(RowPtr, ElementStr, ColonString);
			
				//���ǰ���пո� �Ͱ����޼���
				trim_start_and_end_inline(const_cast<char*>(ElementStr[0].c_str()));
			
				ClassAnalysis.ClassName = ElementStr[0];
				ClassAnalysis.CodeCPPName = ElementStr[0];
				//ȥ��C��Gǰ׺
				{
					char* ClearClassNamePtr = const_cast<char*>(ClassAnalysis.CodeCPPName.c_str());

					trim_start_and_end_inline(ClearClassNamePtr);

					// �Ƴ�ͷ��C��ͷ����G��ͷ
					remove_char_start(ClearClassNamePtr, 'C');
					remove_char_start(ClearClassNamePtr, 'G');
				}

				//���ǵ���̳�����
				//public GObject ,public Interxx
				if (ElementStr.size() >= 2)
				{
					vector<std::string> InheritElement;

					simple_cpp_string_algorithm::parse_into_vector_array(
						ElementStr[1].c_str(), InheritElement, CommaString);

					for (auto& Tmp : InheritElement)
					{
						char* TmpPtr = const_cast<char*>(Tmp.c_str());
						trim_start_inline(TmpPtr);

						remove_string_start(TmpPtr, "public");
						remove_string_start(TmpPtr, "protected");
						remove_string_start(TmpPtr, "private");

						trim_start_inline(TmpPtr);

						ClassAnalysis.InheritName.push_back(Tmp);
					}
				}
			}
		
			//��ȡ��ǵĳ�Ա����
			if (Contain("CDIAPER"))
			{
				FFunctionAnalysis FunctionAnalysis;
				if (GetCodeTypeByFunc(Row, FunctionAnalysis))
				{
					ProcessingReflexParameters(Row,&FunctionAnalysis);

					Row = StringArray[i + 1];
					RowPtr = const_cast<char*>(Row.c_str());
					//static void Hello1(GObject *Context, int32 &A,float b,bool C);
				
					if (Contain("\tstatic") || Contain("static "))
					{
						FunctionAnalysis.bStatic = true;

						char R[1024] = { 0 };
						char L[1024] = { 0 };
						//remove_string_start();
						split(RowPtr, SpaceString, R, L, false);

						Row = L;
					}
					else if (Contain("virtual "))
					{
						FunctionAnalysis.bVirtual = true;

						char R[1024] = { 0 };
						char L[1024] = { 0 };

						split(RowPtr, SpaceString, R, L, false);

						Row = L;
					}

					//ȷ�����Ǻ����ķ�������
					char Tmp[1024] = { 0 };
					{
						//Row =  void Hello1(GObject *Context, int32 &A,float b,bool C);
					
						char R[1024] = { 0 };
						trim_start_inline(RowPtr);

						split(RowPtr, SpaceString, R, Tmp, false);

						////Tmp = Hello1(UObject *Context, int32 &A,float b,bool C);  {}
						FunctionAnalysis.Return = CollectionVariableType(R, ECollectionParmType::Type_Return);
						
						{
							//GetXXX()const {return asdasd}
							//void Hello1(UObject *Context, int32 &A,float b,bool C
							trim_end_inline(Tmp);

							char RStr[1024] = { 0 };
							char LStr[1024] = { 0 };

							split(Tmp, ")", RStr, LStr, false);

							memset(Tmp, 0, 1024);
							strcpy(Tmp, RStr);

							//Tmp = Hello1(UObject *Context, int32 &A,float b,bool C
						}

						char RStr[1024] = { 0 };
						char LStr[1024] = { 0 };

						split(Tmp, LeftParenthesisString, RStr, LStr, false);

						//������
						FunctionAnalysis.Name = RStr;

						AnalyticParameters(LStr,FunctionAnalysis.ParamArray);

						ClassAnalysis.Function.push_back(FunctionAnalysis);
					}
				}
			}	

			////��ȡ��ǵĳ�Ա����
			if (Contain("CVARIABLE"))
			{
				if (Contain("CodeType"))
				{
					FVariableAnalysis VariableAnalysis;
					if (GetCodeTypeByProp(Row, &VariableAnalysis))
					{
						ProcessingReflexParameters(Row,&VariableAnalysis);

						char R[1024] = { 0 };
						char L[1024] = { 0 };

						Row = StringArray[i + 1];
						RowPtr = const_cast<char*>(Row.c_str());

						//Row = \tTSubclassOf<UStaticMesh> Mesh;
						remove_char_start(RowPtr, '\t');
						remove_char_end(RowPtr, ';');

						//Row = TSubclassOf<UStaticMesh> Mesh
						if (Contain(StarString))
						{
							VariableAnalysis.bPointer = true;
							split(RowPtr, StarString, R, L, false);
						}

						if (Contain(FetchAddressString))
						{
							VariableAnalysis.bReference = true;
							split(RowPtr, FetchAddressString, R, L, false);
						}

						if (Contain(SpaceString))
						{
							split(RowPtr, SpaceString, R, L, false);
						}

						if (c_str_contain(R, "const"))
						{
							VariableAnalysis.bConst = true;
							remove_string_start(R, "const");
						}

						trim_start_and_end_inline(R);
						trim_start_and_end_inline(L);

						//�ǲ���ģ��
						if (c_str_contain(R, "<") && c_str_contain(R, ">"))
						{
							//V<a,b,c,d,e,f,g,....>
							char TempR[1024] = { 0 };
							char TempL[1024] = { 0 };
							split(R, "<", TempR, TempL, false);

							VariableAnalysis.Type = TempR;

							//a,b,c,d,e,f,g,....>
							remove_char_end(TempL, '>');
							//a,b,c,d,e,f,g,....

							AnalyticParameters(TempL, VariableAnalysis.InternalType);
						}
						else
						{
							VariableAnalysis.Type = R;
						}
	
						VariableAnalysis.Name = L;

						ClassAnalysis.Variable.push_back(VariableAnalysis);
					}
				}
			}
		}

		return true;
	}
}