#include "botpch.h"
#include "WarlockActions.h"

using namespace ai;

bool UseSoulStone::Execute(Event event)
{
#ifdef CMANGOS
    if (bot->IsInCombat())
#endif
#ifdef MANGOS
    if (bot->IsInCombat())
#endif
       return false;

    // hp check
    if (bot->getStandState() != UNIT_STAND_STATE_STAND)
        bot->SetStandState(UNIT_STAND_STATE_STAND);

    list<Item*> items = AI_VALUE2(list<Item*>, "inventory items", "soulstone");
    if (items.size() > 0)
    {
        Item* spellstone = *(items.begin());
        if (spellstone)
            ai->ImbueItem(spellstone, ai->GetMaster());
    }

	return true;
}

bool RemoveToManySoulShards::Execute(Event event)
{
    // hp check
    if (bot->getStandState() != UNIT_STAND_STATE_STAND)
        bot->SetStandState(UNIT_STAND_STATE_STAND);

    list<Item*> items = AI_VALUE2(list<Item*>, "inventory items", "soul shard");
    if (items.size() > 20)
    {
        int itemCount = items.size() - 20;
        for(list<Item*>::iterator item = items.begin(); itemCount > 0;item++)
        {
            bot->RemoveItem((*item)->GetBagSlot(), (*item)->GetSlot(), true);
            itemCount--;
        }
        return true;
    }
    return false;
}

bool UseSpellStone::Execute(Event event)
{
#ifdef CMANGOS
    if (bot->IsInCombat())
#endif
#ifdef MANGOS
    if (bot->IsInCombat())
#endif
        return false;

    // hp check
    if (bot->getStandState() != UNIT_STAND_STATE_STAND)
        bot->SetStandState(UNIT_STAND_STATE_STAND);

    // Search and apply stone to weapon
    Item* spellstone, * weapon;
    weapon = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
    if (weapon && weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 0)
    {
        list<Item*> items = AI_VALUE2(list<Item*>, "inventory items", "spellstone");
        if(items.size() > 0)
        {
            spellstone = *(items.begin());
            if (spellstone)
                ai->ImbueItem(spellstone, EQUIPMENT_SLOT_MAINHAND);
        }
    }

	return true;
}
