#pragma once

#include "../Action.h"
#include "InventoryAction.h"
#include "UseItemAction.h"

namespace ai
{
    class UseTrinketAction : public UseItemAction {
    public:
        UseTrinketAction(PlayerbotAI* ai) : UseItemAction(ai, "use trinket") {}
        virtual bool Execute(Event event);    

        virtual bool isUseful() { return true; };
        virtual bool isPossible() { return true; }
    };
}