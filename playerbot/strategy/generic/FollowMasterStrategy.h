#include "../generic/NonCombatStrategy.h"
#pragma once

namespace ai
{
	class FollowMultiplier : public Multiplier
	{
	public:
		FollowMultiplier(PlayerbotAI* ai) : Multiplier(ai, "follow multiplier") {}

	public:
		virtual float GetValue(Action* action);
	};

    class FollowMasterStrategy : public Strategy
    {
    public:
        FollowMasterStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        virtual string getName() { return "follow"; }
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
		virtual void InitMultipliers(std::list<Multiplier*>& multipliers);
    };

	class StanceMultiplier : public Multiplier
	{
	public:
		StanceMultiplier(PlayerbotAI* ai) : Multiplier(ai, "stance multiplier") {}

	public:
		virtual float GetValue(Action* action);
	};

	class StanceStrategy : public Strategy
	{
	public:
		StanceStrategy(PlayerbotAI* ai) : Strategy(ai) {}
		virtual string getName() { return "stance"; }
		virtual void InitTriggers(std::list<TriggerNode*>& triggers);
		virtual void InitMultipliers(std::list<Multiplier*>& multipliers);
	};

}
