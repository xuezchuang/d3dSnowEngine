#include "BlueprintNode.h"
#include "BlueprintPin.h"
#include "../../BlueprintConfigInfo.h"
#include "CoreObject/PropertyObject.h"

FBlueprintNode::FBlueprintNode()
{
    NodeName = "New Node";

    OutputNumber = 0;
    InputNumber = 0;

    InputNameMaxLen = 0;
    OutputNameMaxLen = 0;

    Function = nullptr;
}

void FBlueprintNode::BuildEditor(const FCanvasGeometry& InGeometry)
{
    int GridStep = FBlueprintConfigInfo::Get()->StandardGridStep * InGeometry.ZoomRatio;
    NodeSize.y = GridStep;
}

void FBlueprintNode::DrawEditor(float DeltaTime)
{
    ImDrawList* DrawList = ImGui::GetWindowDrawList();

    fvector_2d CurrentPosition = GetPosition();

    //绘制身体
    DrawList->AddRectFilledMultiColor(
        ImVec2(CurrentPosition.x, CurrentPosition.y),
        ImVec2(CurrentPosition.x + NodeSize.x, CurrentPosition.y + NodeSize.y),
        IM_COL32(10, 10, 10, 255.0f),
        IM_COL32(10, 10, 10, 50.f),
        IM_COL32(10, 10, 10, 50.f),
        IM_COL32(10, 10, 10, 255.0f));

    //绘制顶部
    DrawList->AddRectFilledMultiColor(
        ImVec2(CurrentPosition.x, CurrentPosition.y),
        ImVec2(CurrentPosition.x + NodeSize.x, CurrentPosition.y + 32.f * GetZoomRatio()),
        GetTopFrontColor(),
        GetTopBackColor(),
        GetTopBackColor(),
        GetTopFrontColor());

    //黑色描边
    DrawList->AddRect(
        ImVec2(CurrentPosition.x, CurrentPosition.y),
        ImVec2(CurrentPosition.x + NodeSize.x, CurrentPosition.y + NodeSize.y),
        IM_COL32(0, 0, 0, 255), 
        10, 0, 4);

    int TextLen = NodeName.length();

    int TextSize = FBlueprintConfigInfo::Get()->BlueprintNodeFontSize * GetZoomRatio();

    DrawList->AddText(
        NULL,
        TextSize,
        ImVec2(CurrentPosition.x + 10.f * GetZoomRatio(),
        CurrentPosition.y + 2 * GetZoomRatio()),
        GetTextColor(),
        NodeName.c_str());

    DrawPins(DeltaTime);
}

void FBlueprintNode::ExitEditor()
{

}

void FBlueprintNode::OnMouseHovered(const FCanvasGeometry& InGeometry)
{
    BlueprintPinInteraction(InGeometry);
}

void FBlueprintNode::OnMouseClicked(const FCanvasGeometry& InGeometry)
{

}

void FBlueprintNode::OnMouseDrag(const FCanvasGeometry& InGeometry)
{
    ImGuiIO& IO = ImGui::GetIO();
    OffsetPosition += fvector_2d(IO.MouseDelta.x, IO.MouseDelta.y);

    StandardOffsetPosition = OffsetPosition;
    StandardOffsetPosition /= InGeometry.ZoomRatio;

    for (auto& Tmp : Pins)
    {
        Tmp->OnMouseDrag(InGeometry);
    }
}

void FBlueprintNode::OnHighlight()
{
    fvector_2d CurrentPosition = GetPosition();
    fvector_2d EndPosition = CurrentPosition + NodeSize;

    ImDrawList* DrawList = ImGui::GetWindowDrawList();
    DrawList->AddRect(
        ImVec2(CurrentPosition.x, CurrentPosition.y),
        ImVec2(EndPosition.x, EndPosition.y), IM_COL32(255, 255, 50, 255), 10, 0, 5);
}

void FBlueprintNode::ZoomPinsSize(float InRatio)
{
    for (auto &Tmp :Pins)
    {
        Tmp->ZoomSize(InRatio);
    }
}

void FBlueprintNode::ZoomPinsOffset(float InRatio)
{
    for (auto& Tmp : Pins)
    {
        Tmp->ZoomOffset(InRatio);
    }
}

float FBlueprintNode::GetZoomRatio() const
{
    return NodeSize.x / StandardNodeSize.x;
}

std::shared_ptr<FNode> FBlueprintNode::IsInPinRange(const fvector_2d& InCurrentPosition)
{
    for (auto& Tmp : Pins)
    {
        if (Tmp->IsInTheScope(InCurrentPosition))
        {
            return Tmp;
        }
    }

    return nullptr;
}

void FBlueprintNode::ConstructionPin(
    CPropertyObject* InProperty,
    const FCanvasGeometry& InGeometry, 
    EConstructionPinType InType)
{
    if (InProperty)
    {
        CPropertyObject* StartPtr = InProperty;
        while (StartPtr)
        {
            switch (InType)
            {
                case ACCORDING_PROPERTY_TYPE:
                {
                    EPropertyType PropertyType = StartPtr->GetPropertyType();
                    switch (PropertyType)
                    {
                        case INPUT_PROPERTY:
                            AddPin(EBlueprintPinType::Pin_ParamInput, InGeometry, StartPtr);
                            break;
                        case OUT_PROPERTY:
                        case RETURN_PROPERTY:
                            AddPin(EBlueprintPinType::Pin_ParamOutput, InGeometry, StartPtr);
                            break;
                    }
                    break;
                }
                case ALL_INPUT_PARAM:
                {
                    AddPin(EBlueprintPinType::Pin_ParamInput, InGeometry, StartPtr);
                    break;
                }
                case ALL_OUTPUT_PARAM:
                {
                    AddPin(EBlueprintPinType::Pin_ParamOutput, InGeometry, StartPtr);
                    break;
                }
            }

            StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Nest);
        }
    }
}

void FBlueprintNode::BuildPinsNameLenBasedPixels()
{
    InputNameMaxLen = 0;
    OutputNameMaxLen = 0;

    for (auto& Tmp : Pins)
    {
        EBlueprintPinType PinType = Tmp->GetType();
        switch (PinType)
        {
        case Pin_Input:
        case Pin_ParamInput:
            InputNameMaxLen = max(Tmp->GetName().length(), InputNameMaxLen);
            break;
        case Pin_Output:
        case Pin_ParamOutput:
            OutputNameMaxLen = max(Tmp->GetName().length(), OutputNameMaxLen);
            break;
        }
    }
}

void FBlueprintNode::BlueprintNodeSelfAdaption(float InRatio)
{
    int PinSize = FBlueprintConfigInfo::Get()->BlueprintPinSize.x * InRatio;
    int GridStep = FBlueprintConfigInfo::Get()->StandardGridStep * InRatio;

    BuildPinsNameLenBasedPixels();

    int ZoomTextSize = FBlueprintConfigInfo::Get()->BlueprintPinFontSize * InRatio;
    int FillFontSpace = ZoomTextSize * OutputNameMaxLen + ZoomTextSize * InputNameMaxLen;
    
    int SpaceSize = PinSize + //左右空隙
        PinSize * 2.f + //两个按钮大小
        PinSize * 2.f; //两个字体之间的空间
    
    const float ZoomNodeSizeX = FBlueprintConfigInfo::Get()->BlueprintNodeSize.x * InRatio;
    int CurrentViewSize = FillFontSpace + SpaceSize;
    
    //计算节点的宽自适应
    if (CurrentViewSize > (int)ZoomNodeSizeX)
    {
        NodeSize.x = CurrentViewSize;
    }

    //都要执行pins
    for (auto& Tmp : Pins)
    {
        auto InType = Tmp->GetType();
        fvector_2d InNewPosition;

        switch (InType)
        {
            case Pin_Input:
            case Pin_ParamInput:
            {
                InNewPosition = fvector_2d(
                    OffsetPosition.x + PinSize * 2.f,
                    OffsetPosition.y + GridStep * (InputNumber + 1.f));

                InputNumber++;
                break;
            }
            case Pin_Output:
            case Pin_ParamOutput:
            {
                InNewPosition = fvector_2d(
                    OffsetPosition.x + NodeSize.x - PinSize * 2.f,
                    OffsetPosition.y + GridStep * (OutputNumber + 1.f));

                OutputNumber++;
                break;
            }
        }

        Tmp->SetStandardOffsetPosition(InNewPosition / InRatio);
        Tmp->SetOffsetPosition(InNewPosition);
        Tmp->SetOriginPosition(OriginPosition);
        Tmp->SetSize(PinSize);
    }
   
    //计算节点的高自适应
    int MaxNumber = max(InputNumber, OutputNumber);
    NodeSize.y = GridStep * MaxNumber + GridStep;
   
    //求出标准NodeSize
    StandardNodeSize = NodeSize / InRatio;
}

void FBlueprintNode::SetFunction(CFunctionObject* NewFunction)
{
    Function = NewFunction;
}

void FBlueprintNode::ZoomSize(float InRatio)
{
    NodeSize.x = StandardNodeSize.x* InRatio;

    float StandardGridStep = FBlueprintConfigInfo::Get()->StandardGridStep;

    int MaxNumber = max(InputNumber, OutputNumber);

    NodeSize.y = (MaxNumber * StandardGridStep + StandardGridStep) * InRatio;
}

void FBlueprintNode::ZoomOffset(float InRatio)
{
    Super::ZoomOffset(InRatio);
}

void FBlueprintNode::BlueprintPinInteraction(const FCanvasGeometry& InGeometry)
{
    fvector_2d CurrentPosition = fvector_2d(
        InGeometry.MousePositionInCanvas.x + OriginPosition.x,
        InGeometry.MousePositionInCanvas.y + OriginPosition.y);

    for (auto& Tmp : Pins)
    {
        if (Tmp->IsInTheScope(CurrentPosition))
        {
            Tmp->OnMouseHovered(InGeometry);
            break;
        }
    }
}

void FBlueprintNode::DrawPins(float DeltaTime)
{
    for (auto& Tmp : Pins)
    {
        Tmp->SetOriginPosition(OriginPosition);
    }

    for (auto& Tmp : Pins)
    {
        Tmp->DrawEditor(DeltaTime);
    }
}

std::shared_ptr<FBlueprintPin> FBlueprintNode::AddPin(EBlueprintPinType InType,CPropertyObject* InPinProperty)
{
    Pins.push_back(MakePin(InPinProperty));
    auto& Pin = Pins[Pins.size() - 1];

    Pin->SetType(InType);
    Pin->SetOuter(this->shared_from_this());

    return Pin;
}

std::shared_ptr<FBlueprintPin> FBlueprintNode::AddPin(
    EBlueprintPinType InType,
    const FCanvasGeometry& InGeometry,
    CPropertyObject* InPinProperty)
{
    std::shared_ptr<FBlueprintPin> Pin;
    switch (InType)
    {
        case Pin_Input:
        case Pin_ParamInput:    
        {
            Pin = AddPin(InType, InPinProperty);
            if (InPinProperty)
            {
                Pin->SetProperty(InPinProperty);
                Pin->SetName(InPinProperty->GetName());
            }

            Pin->BuildEditor(InGeometry);

            break;
        }
        case Pin_Output:
        case Pin_ParamOutput:
        {
            Pin = AddPin(InType, InPinProperty);
            if (InPinProperty)
            {
                Pin->SetProperty(InPinProperty);
                Pin->SetName(InPinProperty->GetName());
            }

            Pin->BuildEditor(InGeometry);

            break;
        }
    }

    return Pin;
}
