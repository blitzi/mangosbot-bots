#include "botpch.h"
#include "ActionUtils.h"
#include "../../playerbot.h"
#include "../../LootObjectStack.h"
#include "../../PlayerbotAIConfig.h"



void FillAOEHealPipeline(std::list<TriggerNode*>& triggers, string spell)
{	
	triggers.push_back(TRIGGER_CAST_A("almost full aoe heal", spell, ACTION_LIGHT_HEAL));
	triggers.push_back(TRIGGER_CAST_A("medium full aoe heal", spell, ACTION_MEDIUM_HEAL));
	triggers.push_back(TRIGGER_CAST_A("low full aoe heal", spell, ACTION_LOW_HEAL));
	triggers.push_back(TRIGGER_CAST_A("critical full aoe heal", spell, ACTION_CRITICAL_HEAL));
}

void FillHealPipeline(std::list<TriggerNode*>& triggers, string lightSpell, string instantSpell, string defaultSpell)
{
	//self
	triggers.push_back(TRIGGER_CAST_A("almost full health", lightSpell, ACTION_LIGHT_HEAL + 1));
	triggers.push_back(TRIGGER_CAST_A("medium full health", defaultSpell, ACTION_MEDIUM_HEAL + 1));
	triggers.push_back(TRIGGER_CAST_AB("low full health", instantSpell, defaultSpell, ACTION_LOW_HEAL + 1));
	triggers.push_back(TRIGGER_CAST_AB("critical full health", instantSpell, defaultSpell, ACTION_CRITICAL_HEAL + 1));

	//group heal
	triggers.push_back(TRIGGER_CAST_A("almost full health" + ON_PARTY, lightSpell + ON_PARTY, ACTION_LIGHT_HEAL));
	triggers.push_back(TRIGGER_CAST_A("medium full health" + ON_PARTY, defaultSpell + ON_PARTY, ACTION_MEDIUM_HEAL));
	triggers.push_back(TRIGGER_CAST_AB("low full health" + ON_PARTY, instantSpell + ON_PARTY, defaultSpell + ON_PARTY, ACTION_LOW_HEAL));
	triggers.push_back(TRIGGER_CAST_AB("critical full health" + ON_PARTY, instantSpell + ON_PARTY, defaultSpell + ON_PARTY, ACTION_CRITICAL_HEAL));
}

void AddGroupSpell(std::list<TriggerNode*>& triggers, string spell, float prio)
{
	//self
	triggers.push_back(TRIGGER_AND_CAST(spell, prio + 1));

	//group
	triggers.push_back(TRIGGER_AND_CAST(spell + ON_PARTY, prio));
}

void AddGroupSpell(std::list<TriggerNode*>& triggers, string trigger, string spell, float prio)
{
	//self
	triggers.push_back(TRIGGER_CAST_A(trigger, spell, prio + 1));

	//group
	triggers.push_back(TRIGGER_CAST_A(trigger, spell + ON_PARTY, prio));
}