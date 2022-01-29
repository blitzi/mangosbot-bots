#include "botpch.h"
#include "../../playerbot.h"
#include "DpsTargetValue.h"
#include "PlayerbotAIConfig.h"

using namespace ai;

class FindLeastHpTargetStrategy : public FindTargetStrategy
{
public:
    FindLeastHpTargetStrategy(PlayerbotAI* ai) : FindTargetStrategy(ai)
    {
        minHealth = 0;
    }

public:
    virtual void CheckAttacker(Unit* attacker, ThreatManager* threatManager)
    {
        Group* group = ai->GetBot()->GetGroup();
        if (group)
        {
            uint64 guid = group->GetTargetIcon(4);
            if (guid && attacker->GetObjectGuid() == ObjectGuid(guid))
                return;   
        }

        Unit* victim = attacker->GetVictim();

        if (victim && sServerFacade.IsFriendlyTo(ai->GetBot(), victim))
        {
            if (!result || result->GetHealth() > attacker->GetHealth())
                result = attacker;
        }
    }

protected:
    float minHealth;
};

Unit* DpsTargetValue::Calculate()
{
    Unit* rti = RtiTargetValue::Calculate();
    if (rti) return rti;

    FindLeastHpTargetStrategy strategy(ai);
    return TargetValue::FindTarget(&strategy);
}

class FindMaxHpTargetStrategy : public FindTargetStrategy
{
public:
    FindMaxHpTargetStrategy(PlayerbotAI* ai) : FindTargetStrategy(ai)
    {
        maxHealth = 0;
    }

public:
    virtual void CheckAttacker(Unit* attacker, ThreatManager* threatManager)
    {
        Group* group = ai->GetBot()->GetGroup();
        if (group)
        {
            uint64 guid = group->GetTargetIcon(4);
            if (guid && attacker->GetObjectGuid() == ObjectGuid(guid))
                return;
        }

        Unit* victim = attacker->GetVictim();

        if (victim && sServerFacade.IsFriendlyTo(ai->GetBot(), victim))
        {
            if (!result || result->GetHealth() < attacker->GetHealth())
                result = attacker;
        }
     }


protected:
    float maxHealth;
};

Unit* DpsAoeTargetValue::Calculate()
{
    Unit* rti = RtiTargetValue::Calculate();
    if (rti) return rti;

    FindMaxHpTargetStrategy strategy(ai);
    return TargetValue::FindTarget(&strategy);
}



class FindStopDpsTargetStrategy : public FindTargetStrategy
{
public:
    FindStopDpsTargetStrategy(PlayerbotAI* ai) : FindTargetStrategy(ai) {}

public:
    virtual void CheckAttacker(Unit* attacker, ThreatManager* threatManager)
    {
        for (uint32 spellId : sPlayerbotAIConfig.damageStopSpellIds)
        {
            if (ai->HasAura(spellId, attacker))
            {
                result = attacker;
                return;
            }
            else if (ai->HasAura(spellId, ai->GetBot()))
            {
                result = ai->GetBot();
                return;
            }
        }
    }   
};

Unit* DamageStopValue::Calculate()
{
    FindStopDpsTargetStrategy strategy(ai);
    return TargetValue::FindTarget(&strategy);
}