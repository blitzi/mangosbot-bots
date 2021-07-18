#include "botpch.h"
#include "../../playerbot.h"
#include "ForceFollowStrategy.h"
#include "../PassiveMultiplier.h"
#include "../../ServerFacade.h"

using namespace ai;

class ForceFollowMultiplier : public Multiplier
{
public:
    ForceFollowMultiplier(PlayerbotAI* ai) : Multiplier(ai, "force follow") {}

public:
    virtual float GetValue(Action* action);

private:
};

float ForceFollowMultiplier::GetValue(Action* action)
{
    if (!action)
        return 1.0f;

    string name = action->getName();
    if (name == "follow" || name == "co" || name == "nc" || name == "drop target")
        return 1.0f;

    uint32 spellId = AI_VALUE2(uint32, "spell id", name);
    const SpellEntry* const pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
    if (!pSpellInfo) return 0.0f;

    if (spellId && pSpellInfo->Targets & TARGET_FLAG_DEST_LOCATION)
        return 1.0f;
    else if (spellId && pSpellInfo->Targets & TARGET_FLAG_SOURCE_LOCATION)
        return 1.0f;

    uint32 castTime = GetSpellCastTime(pSpellInfo
#ifdef CMANGOS
        , bot
#endif
    );

    if (spellId && castTime > 0)
    {
        float distance = sServerFacade.GetDistance2d(ai->GetBot(), ai->GetMaster());

        return distance > 5 ? 0.0f : 1.0f;
    }        

    return 1.0f;
}

NextAction** ForceFollowStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("follow", 250.0f), NULL);
}

void ForceFollowStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "out of react range",
        NextAction::array(0, new NextAction("flee to master", ACTION_HIGH), NULL)));
}

void ForceFollowStrategy::InitMultipliers(std::list<Multiplier*>& multipliers)
{
    multipliers.push_back(new ForceFollowMultiplier(ai));
}
