#pragma once

#include "../Action.h"
#include "MovementActions.h"
#include "../values/LastMovementValue.h"
#include "UseItemAction.h"

namespace ai
{
    class CheckMountStateAction : public UseItemAction {
    public:
        CheckMountStateAction(PlayerbotAI* ai) : UseItemAction(ai, "check mount state", true) {}

        virtual bool Execute(Event event);
        virtual bool isPossible() { return Action::isPossible(); }
        virtual bool isUseful();        

    //private:
        bool Mount();
    };

}
