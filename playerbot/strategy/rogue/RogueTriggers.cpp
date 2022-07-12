#include "botpch.h"
#include "../../playerbot.h"
#include "RogueTriggers.h"
#include "RogueActions.h"

using namespace ai;

bool SliceAndDiceTrigger::IsActive()
{
	Unit* target = GetTarget();
	bool hasComboPoints = AI_VALUE2(uint8, "combo", "current target") >= 5;// amount;
	return hasComboPoints && SpellTrigger::IsActive() && !ai->HasAura(spell, target);
}

bool RuptureTrigger::IsActive()
{
	Unit* target = GetTarget();
	bool hasComboPoints = AI_VALUE2(uint8, "combo", "current target") >= 5;// amount;
	return hasComboPoints && DebuffTrigger::IsActive();// && !ai->HasAura(spell, target);
}

bool EviscerateTrigger::IsActive()
{
	Unit* target = GetTarget();
	bool hasComboPoints = AI_VALUE2(uint8, "combo", "current target") >= 5;// amount;
	return hasComboPoints && SpellTrigger::IsActive();// && !ai->HasAura(spell, target);
}
