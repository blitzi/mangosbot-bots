#include "botpch.h"
#include "../../playerbot.h"
#include "UseTrinketAction.h"

using namespace ai;

bool UseTrinketAction::Execute(Event event)
{
	auto trinkets = AI_VALUE(list<Item*>, "trinkets on use");

	for each(Item * item in trinkets)
	{
		uint32 spellId = 0;
		ItemPrototype const* proto = item->GetProto();

		for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			// wrong triggering type
			if (proto->Spells[i].SpellTrigger != ITEM_SPELLTRIGGER_ON_USE)
				continue;

			if (proto->Spells[i].SpellId > 0)
			{
				spellId = proto->Spells[i].SpellId;
				break;
			}
		}

		if (spellId > 0 && proto->InventoryType == INVTYPE_TRINKET && item->IsEquipped())
		{
			if (bot->CanUseItem(item) == EQUIP_ERR_OK && !item->IsInTrade())
			{				
				WorldPacket packet(CMSG_USE_ITEM, 1 + 1 + 1 + 4 + 8 + 4 + 1 + 4);
				packet << item->GetBagSlot() << item->GetSlot() << (uint8)1 << spellId << item->GetObjectGuid() << uint32(0) << (uint8)0 << (uint32)TARGET_FLAG_SELF;

				bot->GetSession()->HandleUseItemOpcode(packet);
				return true;
			}
		}
	}	

    return false;
}

bool UseTrinketAction::isPossible()
{
	return AI_VALUE(list<Item*>, "trinkets on use").size() > 0;
}