#include "botpch.h"
#include "../../playerbot.h"
#include "CombatStrategy.h"
#include "../../ServerFacade.h"

using namespace ai;

void CombatStrategy::InitTriggers(list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "enemy out of spell",
        NextAction::array(0, new NextAction("reach spell", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "invalid target",
        NextAction::array(0, new NextAction("drop target", 89.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"no group member in combat",
		NextAction::array(0, new NextAction("leave combat state", 54), NULL)));

    triggers.push_back(new TriggerNode(
        "mounted",
        NextAction::array(0, new NextAction("check mount state", 88.0f), NULL)));

    /*triggers.push_back(new TriggerNode(
        "out of react range",
        NextAction::array(0, new NextAction("flee to master", 52), NULL)));*/

    triggers.push_back(new TriggerNode(
        "out of react range",
        NextAction::array(0, new NextAction("flee to master", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "combat stuck",
        NextAction::array(0, new NextAction("reset", 1.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "combat long stuck",
        NextAction::array(0, new NextAction("hearthstone", 0.9f), new NextAction("repop", 0.8f), NULL)));

    triggers.push_back(new TriggerNode(
        "often",
        NextAction::array(0, new NextAction("use trinket", 50.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "very often",
        NextAction::array(0, new NextAction("use lightwell", 80.0f), NULL)));

	triggers.push_back(new TriggerNode(
        "party member to cancel health",
        NextAction::array(0, new NextAction("cancel heal", 200.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "use trinket",
        NextAction::array(0, new NextAction("use trinket", ACTION_EMERGENCY), NULL)));

    triggers.push_back(new TriggerNode(
        "damage stop",
        NextAction::array(0, new NextAction("damage stop", 200.0f), NULL)));
}

AvoidAoeStrategy::AvoidAoeStrategy(PlayerbotAI* ai) : Strategy(ai)
{
}


class AvoidAoeStrategyMultiplier : public Multiplier
{
public:
    AvoidAoeStrategyMultiplier(PlayerbotAI* ai) : Multiplier(ai, "run away on area debuff") {}

public:
    virtual float GetValue(Action* action);

private:
};

float AvoidAoeStrategyMultiplier::GetValue(Action* action)
{
    if (!action)
        return 1.0f;

    string name = action->GetName();
    if (name == "follow" || name == "co" || name == "nc" || name == "drop target" || name == "flee")
        return 1.0f;

    uint32 spellId = AI_VALUE2(uint32, "spell id", name);
    const SpellEntry* const pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
    if (!pSpellInfo) return 1.0f;

    if (spellId && pSpellInfo->Targets & TARGET_FLAG_DEST_LOCATION)
        return 1.0f;
    else if (spellId && pSpellInfo->Targets & TARGET_FLAG_SOURCE_LOCATION)
        return 1.0f;

    uint32 CastingTime = !IsChanneledSpell(pSpellInfo) ? GetSpellCastTime(pSpellInfo, bot) : GetSpellDuration(pSpellInfo);

    if (AI_VALUE2(bool, "has area debuff", "self target") && spellId && CastingTime > 0)
    {
        return 0.0f;
    }

    return 1.0f;
}


void AvoidAoeStrategy::InitTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "has area debuff",
        NextAction::array(0, new NextAction("flee", ACTION_EMERGENCY + 5), NULL)));
}

void AvoidAoeStrategy::InitMultipliers(std::list<Multiplier*>& multipliers)
{
    multipliers.push_back(new AvoidAoeStrategyMultiplier(ai));
}
