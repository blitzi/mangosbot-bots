#include "botpch.h"
#include "../../playerbot.h"
#include "ItemCountValue.h"

using namespace ai;

list<Item*> InventoryItemValueBase::Find(string qualifier)
{
    list<Item*> result;

    Player* bot = InventoryAction::ai->GetBot();

    list<Item*> items = InventoryAction::parseItems(qualifier, ITERATE_ITEMS_IN_BAGS, false);
    for (list<Item*>::iterator i = items.begin(); i != items.end(); i++)
        result.push_back(*i);

    return result;
}


uint32 ItemCountValue::Calculate()
{
    uint32 count = 0;
    list<Item*> items = Find(qualifier);
    for (list<Item*>::iterator i = items.begin(); i != items.end(); ++i)
    {
        Item* item = *i;
        count += item->GetCount();
    }

    return count;
}

list<Item*> InventoryItemValue::Calculate()
{
    return Find(qualifier);
}

list<Item*> EquipedUsableTrinketValue::Calculate()
{
	list<Item*> trinkets;
	list<Item*> result;

	Player* bot = InventoryAction::ai->GetBot();

	Item* trinket1 = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET1);
	Item* trinket2 = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET2);

	if (trinket1)
		trinkets.push_back(trinket1);

	if (trinket2)
		trinkets.push_back(trinket2);

	for each (Item* item in trinkets)
	{
		ItemPrototype const* proto = item->GetProto();

		for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			if (proto->Spells[i].SpellTrigger == ITEM_SPELLTRIGGER_ON_USE &&
				proto->Spells[i].SpellId > 0 &&
				bot->IsSpellReady(proto->Spells[i].SpellId, proto))
			{
				result.push_back(item);
			}
		}
	}

	return result;
}

