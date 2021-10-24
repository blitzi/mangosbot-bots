#pragma once

#include "../Action.h"
#include "ChangeStrategyAction.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"

namespace ai
{
    class SwitchToMeleeAction : public ChangeCombatStrategyAction
    {
    public:
        SwitchToMeleeAction(PlayerbotAI* ai) : ChangeCombatStrategyAction(ai, "-ranged,+close") {}

        virtual bool Execute(Event event)
        {
            ai->TellMasterNoFacing("Switching to melee!");
            return ChangeCombatStrategyAction::Execute(event);
        }

        virtual bool isUseful()
        {
            if (bot->getClass() == CLASS_HUNTER)
            {
                if (bot->GetGroup())
                    return false;

                Unit* target = AI_VALUE(Unit*, "current target");                

                if (!target)
                    return false;

                if (!ai->HasStrategy("ranged", BOT_STATE_COMBAT))
                    return false;

                if (!sServerFacade.IsInCombat(bot))
                    return false;

                if (target->GetVictim() != bot)
                    return false;

                return sServerFacade.IsDistanceLessOrEqualThan(AI_VALUE2(float, "distance", "current target"), 8.0f);                  
            }
            return ai->HasStrategy("ranged", BOT_STATE_COMBAT);
        }
    };

    class SwitchToRangedAction : public ChangeCombatStrategyAction
    {
    public:
        SwitchToRangedAction(PlayerbotAI* ai) : ChangeCombatStrategyAction(ai, "-close,+ranged") {}

        virtual bool Execute(Event event)
        {
            ai->TellMasterNoFacing("Switching to ranged!");
            return ChangeCombatStrategyAction::Execute(event);
        }

        virtual bool isUseful()
        {
            if (bot->getClass() == CLASS_HUNTER)
            {
                if (!ai->HasStrategy("close", BOT_STATE_COMBAT))
                    return false;

                if (!sServerFacade.IsInCombat(bot))
                    return true;

                Unit* target = AI_VALUE(Unit*, "current target");
                bool hasAmmo = AI_VALUE2(uint32, "item count", "ammo");

                if (!target)
                    return false;

                if (!hasAmmo)
                    return false;

                return sServerFacade.IsDistanceGreaterThan(AI_VALUE2(float, "distance", "current target"), 8.0f);
            }
            return ai->HasStrategy("close", BOT_STATE_COMBAT);
        }
    };
}
