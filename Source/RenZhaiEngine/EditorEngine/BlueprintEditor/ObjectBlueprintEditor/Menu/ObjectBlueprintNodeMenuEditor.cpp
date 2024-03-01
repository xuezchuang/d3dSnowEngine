#include "ObjectBlueprintNodeMenuEditor.h"
#include "CodeReflection/FunctionManage.h"
#include "CoreObject/ClassObject.h"
#include "Core/AssetPackageManage.h"
#include "../Graphic/ObjectGraphicBlueprintEditor.h"
#include "../../Core/BlueprintNode/BlueprintNode.h"
#include "CoreObject/FunctionObject.h"
#include "../../BlueprintFunctionLibrary.h"

void FObjectBlueprintNodeMenuEditor::BuildEditor()
{

}

void FObjectBlueprintNodeMenuEditor::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);

	ImVec2 DragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	ImGui::InvisibleButton("Context", ImGui::GetContentRegionAvail(), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

	if (DragDelta.x == 0.f && DragDelta.y == 0.f)
	{
		ImGui::OpenPopupOnItemClick("Context", ImGuiPopupFlags_MouseButtonRight);
	}

	if (ImGui::BeginPopup("Context"))
	{
		ImGui::BeginChild("Blueprint Node List", ImVec2(400, 619), true);
		if (!GraphicBlueprintEditor.expired())
		{
			static ImGuiTextFilter BlueprintNodeFilter;

			BlueprintNodeFilter.Draw("BP Filter");

			if (auto GraphicBlueprintObjEditor = GraphicBlueprintEditor.lock())
			{
				if (CAssetPackage* InPackage = FAssetPackageManage::Get()->FindPackage(GraphicBlueprintObjEditor->GetBlueprintAssetName()))
				{
					if (CClassObject* InClassObject = InPackage->GetClassObject())
					{
						if (CCoreMinimalObject* InObject = InClassObject->GetDefaultObject())
						{
							map<string, vector<CFunctionObject*>> FunCategorys;
							auto Add = [&](const string& InKey, CFunctionObject* InFunObject)
							{
								if (!InKey.empty())
								{
									FunCategorys[InKey].push_back(InFunObject);
								}
							};

							//1.成员函数
							CFunctionObject* StartPtr = InObject->GetNativeClass().FunctionList;
							while (StartPtr)
							{
								if (BlueprintNodeFilter.PassFilter(StartPtr->GetName().c_str()))
								{
									Add(StartPtr->GetNativeClass().Metas["Category"], StartPtr);
								}

								StartPtr = dynamic_cast<CFunctionObject*>(StartPtr->Nest);
							}

							//2.静态函数
							StartPtr = InObject->GetNativeClass().StaticFunctionList;
							while (StartPtr)
							{
								if (BlueprintNodeFilter.PassFilter(StartPtr->GetName().c_str()))
								{
									Add(StartPtr->GetNativeClass().Metas["Category"], StartPtr);
								}

								StartPtr = dynamic_cast<CFunctionObject*>(StartPtr->Nest);
							}

							//3.按照分类生成函数列表
							for (auto& Tmp : FunCategorys)
							{
								if (ImGui::CollapsingHeader(Tmp.first.c_str()/*, ImGuiTreeNodeFlags_DefaultOpen*/))
								{
									bool bCreate = false;

									ImGui::Separator();
									for (auto& Func : Tmp.second)
									{
										//优先获取displayname
										string Name = GBlueprintFunctionLibrary::GetFunction(Func);

										if (ImGui::MenuItem(Name.c_str(), NULL, false, true))
										{
											GBlueprintFunctionLibrary::CreateBulueprintNodeByName(Func, GraphicBlueprintObjEditor.get(),Name.c_str());
											ImGui::CloseCurrentPopup();

											bCreate = true;
											break;
										}
									}
									ImGui::Separator();

									if (bCreate)
									{
										break;
									}
								}
							}
						}
					}
				}
			}	
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}
}

void FObjectBlueprintNodeMenuEditor::ExitEditor()
{

}