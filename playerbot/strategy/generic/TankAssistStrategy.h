#include "../generic/NonCombatStrategy.h"
#pragma once

namespace ai
{
    class TankAssistStrategy : public Strategy
    {
    public:
        TankAssistStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        virtual string getName() { return "tank assist"; }
        virtual int GetType() { return STRATEGY_TYPE_TANK; }

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
    };

    class CarefulTankingStrategy : public Strategy
    {
    public:
        CarefulTankingStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        virtual string getName() { return "careful tanking"; }
        virtual int GetType() { return STRATEGY_TYPE_COMBAT; }
    };

}
