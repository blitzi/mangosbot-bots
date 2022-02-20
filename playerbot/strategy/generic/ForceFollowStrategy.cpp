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

	string name = action->GetBaseName();
    if (name == "follow" || name == "co" || name == "nc" || name == "drop target")
        return 1.0f;

    if (name == "reach spell" || name == "reach melee" || name == "move to loot")
        return 0.0f;

    float distance = sServerFacade.GetDistance2d(ai->GetBot(), ai->GetMaster());

	if (name == "melee" && ai->IsRanged(ai->GetBot()))
		return 0.0f;

    if(name == "set behind" || name == "melee" || name == "dps assist")
        return distance > sPlayerbotAIConfig.forceFollowDistance ? 0.0f : 1.0f;

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

    if (spellId && castTime > 0)
    {        
        return distance > sPlayerbotAIConfig.forceFollowDistance ? 0.0f : 1.0f;
    }        

    return 1.0f;
}

NextAction** ForceFollowStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("follow", 1), NULL);
}

void ForceFollowStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	ai->GetMoveTimer()->Reset(0);

    triggers.push_back(new TriggerNode(
        "force follow too far",
        NextAction::array(0, new NextAction("follow", 250), NULL)));
}

void ForceFollowStrategy::InitMultipliers(std::list<Multiplier*>& multipliers)
{
    multipliers.push_back(new ForceFollowMultiplier(ai));
}
