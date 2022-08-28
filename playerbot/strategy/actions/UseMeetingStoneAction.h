#pragma once

#include "../Action.h"
#include "MovementActions.h"
#include "ChatShortcutActions.h"

namespace ai
{
    class SummonAction : public ReturnPositionResetAction
    {
    public:
        SummonAction(PlayerbotAI* ai, string name = "summon") : ReturnPositionResetAction(ai, name) {}

        virtual bool Execute(Event event);

    protected:
        bool Teleport(Player *summoner, Player *player);
        bool SummonUsingGos(Player *summoner, Player *player);
        bool SummonUsingNpcs(Player *summoner, Player *player);
    };

    class UseMeetingStoneAction : public SummonAction
    {
    public:
        UseMeetingStoneAction(PlayerbotAI* ai) : SummonAction(ai, "use meeting stone") {}

        virtual bool Execute(Event event);
    };

    class AcceptSummonAction : public Action
    {
    public:
        AcceptSummonAction(PlayerbotAI* ai) : Action(ai, "accept summon") {}

        virtual bool Execute(Event event);
    };
}
