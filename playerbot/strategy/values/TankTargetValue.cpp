#include "botpch.h"
#include "../../playerbot.h"
#include "TankTargetValue.h"
#include "AttackersValue.h"

using namespace ai;

class FindTargetForTankStrategy : public FindNonCcTargetStrategy
{
public:
    FindTargetForTankStrategy(PlayerbotAI* ai) : FindNonCcTargetStrategy(ai)
    {
        minThreat = 0;
    }

public:
    virtual void CheckAttacker(Unit* creature, ThreatManager* threatManager)
    {
        Player* bot = ai->GetBot();
        if (IsCcTarget(creature)) return;

        Unit* target = creature->GetTarget();

        if (target && sServerFacade.IsFriendlyTo(bot, target) == false)
            return;

        float threat = threatManager->getThreat(bot);
        if (!result || (minThreat - threat) > 0.1f)
        {
            minThreat = threat;
            result = creature;
        }
    }

protected:
    float minThreat;
};


Unit* TankTargetValue::Calculate()
{
    FindTargetForTankStrategy strategy(ai);
    return FindTarget(&strategy);
}
