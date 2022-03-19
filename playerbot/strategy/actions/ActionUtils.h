#pragma once

#include "../Trigger.h"

void FillAOEHealPipeline(std::list<TriggerNode*>& triggers, string spell);
void FillHealPipeline(std::list<TriggerNode*>& triggers, string lightSpell, string instantSpell, string defaultSpell);
void AddGroupSpell(std::list<TriggerNode*>& triggers, string spell, float prio);
void AddGroupSpell(std::list<TriggerNode*>& triggers, string trigger, string spell, float prio);