#pragma once

#include "../Action.h"

namespace ai
{
    class CancelFormAction : public Action {
    public:
        CancelFormAction(PlayerbotAI* ai) : Action(ai, "cancel form") {}
        virtual bool Execute(Event event);

        virtual bool isPossible();
    };

}
