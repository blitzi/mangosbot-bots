#include "botpch.h"
//#include "../../playerbot.h"
#include "WarlockActions.h"

using namespace ai;

//bool UseSoulStone::Execute(Event event)
//{
//    if (bot->IsMoving())
//    {
//        MotionMaster& mm = *bot->GetMotionMaster();
//        bot->StopMoving();
//        mm.Clear();
//    }
//    bool used = UseItemAction::Execute(event);
//
//    return used;
//}

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

    // Search and apply poison to weapons
    // Mainhand ...
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
}
//
//if (oldItem)
//{
//    bot->RemoveItem(INVENTORY_SLOT_BAG_0, slot, true);
//    oldItem->DestroyForPlayer(bot);
//}
//bot->RemoveItem(pItem->GetBagSlot(), pItem->GetSlot(), true);