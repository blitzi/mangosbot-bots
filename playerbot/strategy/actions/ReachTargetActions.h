#pragma once

#include "../Action.h"
#include "MovementActions.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"
#include <playerbot/strategy/values/Stances.h>

namespace ai
{
    class ReachTargetAction : public MovementAction
    {
    public:
        ReachTargetAction(PlayerbotAI* ai, string name, float distance) : MovementAction(ai, name)
		{
            this->distance = distance;
        }
        virtual bool Execute(Event event)
		{
            Unit* target = AI_VALUE(Unit*, GetTargetName());
            if (!target)
                return false;

            float distance = ai->IsRanged(bot) ? ai->GetRange("spell") : 0;
            Stance* stance = context->GetValue<Stance*>("stance")->Get();

            if (stance && stance->getName() != "near")
                return MoveTo(target);
            else
            {
                if (distance < max(5.0f, bot->GetCombinedCombatReach(target, true)))
                {
                    return ChaseTo(target, 0.0f, GetFollowAngle());
                }
                else
                {
                    bool inLos = bot->IsWithinLOSInMap(target);
                    bool isFriend = sServerFacade.IsFriendlyTo(bot, target);
                    float meleeDist = inLos ? distance - sPlayerbotAIConfig.contactDistance : isFriend ? distance / 2 : distance - sPlayerbotAIConfig.contactDistance;
                    float distance = ai->IsRanged(bot) ? sPlayerbotAIConfig.spellDistance : meleeDist;

                    return ChaseTo(target, distance, bot->GetAngle(target));
                }
            }                  
        }
        virtual bool isUseful()
		{
            Unit* target = AI_VALUE(Unit*, GetTargetName());
            Stance* stance = context->GetValue<Stance*>("stance")->Get();

            if (target && stance && stance->getName() != "near")
            {
                WorldLocation location;
                WorldLocation loc = stance->GetLocation();
                bot->GetPosition(location);
                return !Formation::IsSameLocation(location, loc);
            }

            if (target)
            {
                float distance = ai->IsRanged(bot) ? ai->GetRange("spell") : 0;
                bool a = !bot->IsWithinDistInMap(target, distance);
                bool b = bot->IsWithinDistInMap(target, distance);
                bool c = !bot->IsWithinLOSInMap(target);
                
                return (a || (b && c));
            }

            return false;
        }
        virtual string GetTargetName() { return "current target"; }
        virtual bool IgnoresCasting() { return true; }

    protected:
        float distance;
    };

    class CastReachTargetSpellAction : public CastSpellAction
    {
    public:
        CastReachTargetSpellAction(PlayerbotAI* ai, string spell, float distance) : CastSpellAction(ai, spell)
		{
            this->distance = distance;
        }
		virtual bool isUseful()
		{
			return sServerFacade.IsDistanceGreaterThan(AI_VALUE2(float, "distance", "current target"), (distance + sPlayerbotAIConfig.contactDistance));
		}

    protected:
        float distance;
    };

    class ReachMeleeAction : public ReachTargetAction
	{
    public:
        ReachMeleeAction(PlayerbotAI* ai) : ReachTargetAction(ai, "reach melee", sPlayerbotAIConfig.meleeDistance) {}
    };

    class ReachSpellAction : public ReachTargetAction
	{
    public:
        ReachSpellAction(PlayerbotAI* ai) : ReachTargetAction(ai, "reach spell", ai->GetRange("spell")) {}
    };

    class ReachPartyMemberToHealAction : public ReachTargetAction
    {
    public:
        ReachPartyMemberToHealAction(PlayerbotAI* ai) : ReachTargetAction(ai, "reach party member to heal", ai->GetRange("spell")) {}
        virtual string GetTargetName() { return "party member to heal"; }
    };
}
