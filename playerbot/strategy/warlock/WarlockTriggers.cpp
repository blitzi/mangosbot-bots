#include "botpch.h"
#include "../../playerbot.h"
#include "WarlockTriggers.h"
#include "WarlockActions.h"

using namespace ai;

bool NoArmorTrigger::IsActive() 
{
	Unit* target = GetTarget();
	return !ai->HasAura("demon skin", target) &&
		!ai->HasAura("demon armor", target) &&
		!ai->HasAura("fel armor", target);
}

bool SpellstoneTrigger::IsActive() 
{
	Item* weapon;
	weapon = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
	if (weapon)
	{
		bool isEnchanted = weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) != 0;
		return SpellTrigger::IsActive() &&
			!isEnchanted && //!ai->HasAura(55194, target) && 
			AI_VALUE2(uint32, "item count", getName()) > 0;
	}
	return false;

}
