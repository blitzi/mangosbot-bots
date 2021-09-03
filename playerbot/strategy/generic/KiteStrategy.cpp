#include "botpch.h"
#include "../../playerbot.h"
#include "KiteStrategy.h"
#include <playerbot/ServerFacade.h>

using namespace ai;

KiteStrategy::KiteStrategy(PlayerbotAI* ai) : Strategy(ai)
{
}

void KiteStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "has aggro",
        NextAction::array(0, new NextAction("runaway", 51.0f), NULL)));
}


RunAwayOnAreaDebuff::RunAwayOnAreaDebuff(PlayerbotAI* ai) : Strategy(ai)
{
}


class RunAwayOnAreaDebuffMultiplier : public Multiplier
{
public:
    RunAwayOnAreaDebuffMultiplier(PlayerbotAI* ai) : Multiplier(ai, "run away on area debuff") {}

public:
    virtual float GetValue(Action* action);

private:
};

float RunAwayOnAreaDebuffMultiplier::GetValue(Action* action)
{
    if (!action)
        return 1.0f;

    string name = action->getName();
    if (name == "follow" || name == "co" || name == "nc" || name == "drop target")
        return 1.0f;

    uint32 spellId = AI_VALUE2(uint32, "spell id", name);
    const SpellEntry* const pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
    if (!pSpellInfo) return 1.0f;

    if (spellId && pSpellInfo->Targets & TARGET_FLAG_DEST_LOCATION)
        return 1.0f;
    else if (spellId && pSpellInfo->Targets & TARGET_FLAG_SOURCE_LOCATION)
        return 1.0f;

    uint32 castTime = GetSpellCastTime(pSpellInfo
#ifdef CMANGOS
        , bot
#endif
    );

    if (AI_VALUE2(bool, "has area debuff", "self target") && spellId && castTime > 0)
    {
        return 0.0f;
    }

    return 1.0f;
}


void RunAwayOnAreaDebuff::InitTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "has area debuff",
        NextAction::array(0, new NextAction("return", 250.0f), NULL)));
}

void RunAwayOnAreaDebuff::InitMultipliers(std::list<Multiplier*>& multipliers)
{
    multipliers.push_back(new RunAwayOnAreaDebuffMultiplier(ai));
}
