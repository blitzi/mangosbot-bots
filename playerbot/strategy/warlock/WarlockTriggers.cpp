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
			!isEnchanted &&
			AI_VALUE2(uint32, "item count", getName()) > 0;
	}
	return false;
}

bool SoulstoneOnTankTrigger::IsActive() 
{
	bool hasItem = AI_VALUE2(uint32, "item count", "soulstone") > 0;
	if (hasItem && GetTarget() && !ai->HasAura("soulstone resurrection", GetTarget()))
	{
		bool isInGroup = (ai->GetBot()->IsInGroup((Player*)GetTarget(), true) || ai->GetBot()->IsInGroup((Player*)GetTarget()));
		return isInGroup;
	}
	return false;
}

bool DrainSoulTrigger::IsActive()
{
	bool isAoEActive = AI_VALUE(uint8, "aoe count") >= aoeEnemyAmount && AI_VALUE(uint8, "attacker count") >= aoeEnemyAmount;
	bool isLowHealth = ValueInRangeTrigger::IsActive();
	bool hasLessItems = AI_VALUE2(uint32, "item count", "soul shard") < 28;// int(AI_VALUE(uint8, "bag space") * 0.2);
	return isLowHealth && hasLessItems && !AI_VALUE2(bool, "dead", GetTargetName()) && !isAoEActive;
}

bool RemoveSoulShardTrigger::IsActive()
{
	// int(AI_VALUE(uint8, "bag space") * 0.2);
	uint32 count = AI_VALUE2(uint32, "item count", "soul shard");
	return AI_VALUE2(uint32, "item count", "soul shard") > 20;
}
