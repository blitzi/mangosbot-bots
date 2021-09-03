#pragma once

namespace ai
{
    class KiteStrategy : public Strategy
    {
    public:
        KiteStrategy(PlayerbotAI* ai);
        virtual string getName() { return "kite"; }
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
    };

    class RunAwayOnAreaDebuff : public Strategy
    {
    public:
        RunAwayOnAreaDebuff(PlayerbotAI* ai);
        virtual string getName() { return "run away on area debuff"; }
        virtual void InitMultipliers(std::list<Multiplier*>& multipliers);
        virtual void InitTriggers(std::list<TriggerNode*>& triggers);
    };

}
