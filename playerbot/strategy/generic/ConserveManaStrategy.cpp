#include "botpch.h"
#include "../../playerbot.h"
#include "ConserveManaStrategy.h"
#include "../../PlayerbotAIConfig.h"
#include "../actions/GenericSpellActions.h"
#include "../values/LastSpellCastValue.h"
#include "../../ServerFacade.h"

using namespace ai;

float ConserveManaMultiplier::GetValue(Action* action)
{
    if (action == NULL) return 1.0f;

    float health = AI_VALUE2(float, "health", "self target");
    float targetHealth = AI_VALUE2(float, "health", "current target");
	float mana = AI_VALUE2(float, "mana", "self target");
    bool hasMana = AI_VALUE2(bool, "has mana", "self target");
    bool mediumMana = hasMana && mana < sPlayerbotAIConfig.mediumMana;

    string name = action->GetName();

    if (health < sPlayerbotAIConfig.lowHealth)
        return 1.0f;

    Unit* target = AI_VALUE(Unit*, "current target");
    if (action->GetTarget() != target)
        return 1.0f;

    CastRangeSpellAction* spellAction = dynamic_cast<CastRangeSpellAction*>(action);
    if (!spellAction)
        return 1.0f;

    string spell = spellAction->GetName();
    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
    const SpellEntry* const spellInfo = sServerFacade.LookupSpellInfo(spellId);
    if (!spellInfo || spellInfo->powerType != POWER_MANA)
        return 1.0f;

    if (mediumMana && dynamic_cast<CastBuffSpellAction*>(action))
        return 0.0f;

    if (target)
    {
        if (((int)target->GetLevel() - (int)bot->GetLevel()) >= 0)
            return 1.0f;
    }

    return 1.0f;
}

float SaveManaMultiplier::GetValue(Action* action)
{
    if (action == NULL)
        return 1.0f;

    if (action->GetTarget() != AI_VALUE(Unit*, "current target"))
        return 1.0f;

    double saveLevel = AI_VALUE(double, "mana save level");
    if (saveLevel <= 1.0)
        return 1.0f;

    CastRangeSpellAction* spellAction = dynamic_cast<CastRangeSpellAction*>(action);
    if (!spellAction)
        return 1.0f;

    string spell = spellAction->GetName();
    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
    const SpellEntry* const spellInfo = sServerFacade.LookupSpellInfo(spellId);
    if (!spellInfo || spellInfo->powerType != POWER_MANA)
        return 1.0f;

    int32 cost = spellInfo->manaCost;
    if (!cost)
        return 1.0f;

    time_t lastCastTime = AI_VALUE2(time_t, "last spell cast time", spell);
    if (!lastCastTime)
        return 1.0f;

    time_t elapsed = time(0) - lastCastTime;
    if ((double)elapsed < 10 * saveLevel)
        return 0.0f;

    return 1.0f;
}


void ConserveManaStrategy::InitMultipliers(std::list<Multiplier*> &multipliers)
{
    multipliers.push_back(new ConserveManaMultiplier(ai));
}
