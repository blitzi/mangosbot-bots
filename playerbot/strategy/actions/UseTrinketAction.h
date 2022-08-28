#pragma once

#include "../Action.h"
#include "InventoryAction.h"
#include "UseItemAction.h"

namespace ai
{
    class UseTrinketAction : public Action {
    public:
        UseTrinketAction(PlayerbotAI* ai) : Action(ai, "use trinket") {	}
        virtual bool Execute(Event event);

		virtual bool isPossible();
		virtual bool isUseful() { return UseItemAction::isUseful() && !bot->HasStealthAura(); }

	private:
    };
}