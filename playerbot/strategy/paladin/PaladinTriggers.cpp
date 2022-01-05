#include "botpch.h"
#include "../../playerbot.h"
#include "PaladinTriggers.h"
#include "PaladinActions.h"

using namespace ai;

bool SealTrigger::IsActive()
{
	Unit* target = GetTarget();
	return !ai->HasAura("seal of justice", target) &&
        !ai->HasAura("seal of command", target) &&
        !ai->HasAura("seal of vengeance", target) &&
		!ai->HasAura("seal of righteousness", target) &&
		!ai->HasAura("seal of light", target) &&
        !ai->HasAura("seal of wisdom", target) &&
        AI_VALUE2(bool, "combat", "self target");
}

bool SealOfWisdomTrigger::IsActive()
{
    bool lowMana = AI_VALUE2(bool, "has mana", "self target") && AI_VALUE2(uint8, "mana", "self target") < sPlayerbotAIConfig.mediumMana;

    return SealTrigger::IsActive() && lowMana;
}

bool CrusaderAuraTrigger::IsActive()
{
	Unit* target = GetTarget();
	return AI_VALUE2(bool, "mounted", "self target") && !ai->HasAura("crusader aura", target);
}

bool BlessingTrigger::IsActive()
{
    Unit* target = GetTarget();
    return SpellTrigger::IsActive() && !ai->HasAnyAuraOf(target,
        "blessing of might", "blessing of wisdom", "blessing of sanctuary", NULL);
}
