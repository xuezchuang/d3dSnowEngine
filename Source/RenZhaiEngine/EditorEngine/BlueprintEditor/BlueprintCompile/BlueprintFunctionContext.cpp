#include "BlueprintFunctionContext.h"
#include "Terminal.h"
#include "CoreObject/PropertyObject.h"

bool FBlueprintFunctionContext::FindTerminal(
    const simple_c_guid& InFunctionObjectKey,
    CPropertyObject* InProperty, 
    FFunctionTerminal& OutTerminal)
{
    if (auto FunctionTerminals = FindTerminals(InFunctionObjectKey))
    {
        for (auto& Tmp : **FunctionTerminals)
        {
            if (Tmp.Name == InProperty->GetName())
            {
                OutTerminal = Tmp;

                return true;
            }
        }
    }

    return false;
}

std::shared_ptr<vector<FFunctionTerminal>>* FBlueprintFunctionContext::FindTerminals(
    const simple_c_guid& InFunctionObjectKey)
{
    for (auto &Tmp :TerminalMap)
    {
        if (guid_equal(&InFunctionObjectKey,Tmp.first.get()))
        {
            return &Tmp.second;
        }
    }

    return nullptr;
}
