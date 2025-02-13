#pragma once

#include "GenericPaladinStrategy.h"

namespace ai
{
    class HealPaladinStrategy : public GenericPaladinStrategy
    {
    public:
        HealPaladinStrategy(PlayerbotAI* ai);

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual string getName() { return "heal"; }
		virtual int GetType() { return STRATEGY_TYPE_HEAL; }
    };
}
