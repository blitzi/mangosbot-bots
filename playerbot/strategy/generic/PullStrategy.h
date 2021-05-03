#pragma once

#include "CombatStrategy.h"

namespace ai
{
    class PullStrategy : public CombatStrategy
    {
    public:
        PullStrategy(PlayerbotAI* ai, string action);

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual string getName() { return "pull"; }
        virtual NextAction** getDefaultActions();
        virtual void InitMultipliers(std::list<Multiplier*>& multipliers);

    private:
    };

    class PossibleAdsStrategy : public Strategy
    {
    public:
        PossibleAdsStrategy(PlayerbotAI* ai) : Strategy(ai) {}

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual string getName() { return "ads"; }
    };
}
