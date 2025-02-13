#pragma once

#include "../Action.h"
#include "../../LootObjectStack.h"
#include "MovementActions.h"
#include "InventoryAction.h"

namespace ai
{
    class LootAction : public MovementAction
    {
    public:
        LootAction(PlayerbotAI* ai) : MovementAction(ai, "loot") {}
        virtual bool Execute(Event event);
        virtual bool isPossible() { return Action::isPossible();  }
    };

    class OpenLootAction : public MovementAction
    {
    public:
        OpenLootAction(PlayerbotAI* ai) : MovementAction(ai, "open loot") {}
        virtual bool Execute(Event event);
        virtual bool isPossible() { return Action::isPossible(); }

    private:
        bool DoLoot(LootObject& lootObject);
        uint32 GetOpeningSpell(LootObject& lootObject);
        uint32 GetOpeningSpell(LootObject& lootObject, GameObject* go);
        bool CanOpenLock(LootObject& lootObject, const SpellEntry* pSpellInfo, GameObject* go);
        bool CanOpenLock(uint32 skillId, uint32 reqSkillValue);
    };

    class StoreLootAction : public InventoryAction
    {
    public:
        StoreLootAction(PlayerbotAI* ai) : InventoryAction(ai, "store loot") {}
        virtual bool Execute(Event event);
        static bool IsLootAllowed(uint32 itemid, PlayerbotAI *ai);  
    };

    class ReleaseLootAction : public MovementAction
    {
    public:
        ReleaseLootAction(PlayerbotAI* ai) : MovementAction(ai, "release loot") {}
        virtual bool Execute(Event event);
        virtual bool isPossible() { return Action::isPossible(); }
    };
}
