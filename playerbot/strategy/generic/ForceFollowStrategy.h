#include "../generic/CombatStrategy.h"
#pragma once

namespace ai
{
    class ForceFollowStrategy : public CombatStrategy
    {
    public:
        ForceFollowStrategy(PlayerbotAI* ai) : CombatStrategy(ai) {}
        virtual string getName() { return "force follow"; }
        virtual NextAction** getDefaultActions();
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual void InitMultipliers(std::list<Multiplier*>& multipliers);

    };

}
