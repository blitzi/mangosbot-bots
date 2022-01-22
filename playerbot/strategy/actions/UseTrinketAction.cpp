#include "botpch.h"
#include "../../playerbot.h"
#include "UseTrinketAction.h"

using namespace ai;

bool UseTrinketAction::Execute(Event event)
{
  /*  Item* trinket1 = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET1);
    Item* trinket2 = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET2);

    ItemPrototype const* proto = NULL;

    if (trinket1 && UseItemAuto(trinket1))
        proto = trinket1->GetProto();

    if (trinket2 && UseItemAuto(trinket2))
        proto = trinket2->GetProto();

    if (proto != NULL && (proto->Spells->SpellCategory == 1141))
    {
        for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; i++)
        {
            uint32 spellId = item->GetProto()->Spells[i].SpellId;
            if (!spellId)
                continue;

            if (!ai->CanCastSpell(spellId, bot, false))
                continue;
        }
    }*/

    return false;
}