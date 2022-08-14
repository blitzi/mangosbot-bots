#include "../botpch.h"
#include "playerbot.h"
#include "PlayerbotFactory.h"

#include "../ahbot/AhBotConfig.h"
#include "SQLStorages.h"
#include "ItemPrototype.h"
#include "PlayerbotAIConfig.h"
#include "AccountMgr.h"
#include "DBCStore.h"
#include "SharedDefines.h"
#include "ahbot/AhBot.h"
#include "RandomItemMgr.h"
#include "RandomPlayerbotFactory.h"
#include "ServerFacade.h"
#include "AiFactory.h"
#ifndef MANGOSBOT_ZERO
    #ifdef CMANGOS
        #include "Arena/ArenaTeam.h"
    #endif
    #ifdef MANGOS
        #include "ArenaTeam.h"
    #endif
#include <playerbot/strategy/actions/InventoryAction.cpp>
#endif

using namespace ai;
using namespace std;

#define PLAYER_SKILL_INDEX(x)       (PLAYER_SKILL_INFO_1_1 + ((x)*3))

uint32 PlayerbotFactory::tradeSkills[] =
{
    SKILL_ALCHEMY,
    SKILL_ENCHANTING,
    SKILL_SKINNING,
    SKILL_TAILORING,
    SKILL_LEATHERWORKING,
    SKILL_ENGINEERING,
    SKILL_HERBALISM,
    SKILL_MINING,
    SKILL_BLACKSMITHING,
    SKILL_COOKING,
    SKILL_FIRST_AID,
    SKILL_FISHING
#ifndef MANGOSBOT_ZERO
	,
	SKILL_JEWELCRAFTING
#endif
#ifdef MANGOSBOT_TWO
	,SKILL_INSCRIPTION
#endif
};

list<uint32> PlayerbotFactory::classQuestIds;
list<uint32> PlayerbotFactory::specialQuestIds;

void PlayerbotFactory::Init()
{
}

void PlayerbotFactory::Prepare()
{
    if (!itemQuality)
    {
        if (level < 20)
            itemQuality = urand(ITEM_QUALITY_NORMAL, ITEM_QUALITY_UNCOMMON);
        else if (level < 40)
            itemQuality = urand(ITEM_QUALITY_UNCOMMON, ITEM_QUALITY_RARE);
        else if (level < 60)
#ifdef MANGOSBOT_ZERO
            itemQuality = urand(ITEM_QUALITY_RARE, ITEM_QUALITY_EPIC);
		else if (level < 70)
			itemQuality = ITEM_QUALITY_EPIC;
#else
			itemQuality = urand(ITEM_QUALITY_UNCOMMON, ITEM_QUALITY_EPIC);
		else if (level < 70)
			itemQuality = urand(ITEM_QUALITY_RARE, ITEM_QUALITY_EPIC);
#endif
		else if (level < 80)
			itemQuality = urand(ITEM_QUALITY_RARE, ITEM_QUALITY_EPIC);
        else
            itemQuality = ITEM_QUALITY_EPIC;
    }

    if (sServerFacade.UnitIsDead(bot))
        bot->ResurrectPlayer(1.0f, false);

    bot->CombatStop(true);

    if (!sPlayerbotAIConfig.randomBotShowHelmet)
    {
       bot->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM);
    }

    if (!sPlayerbotAIConfig.randomBotShowCloak)
    {
       bot->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK);
    }
}

void PlayerbotFactory::Randomize(bool incremental)
{
    sLog.outDetail("Preparing to %s randomize...", (incremental ? "incremental" : "full"));
    Prepare();

    sLog.outDetail("Initializing skills (step 1)...");
    InitSkills();
    InitTradeSkills();

    sLog.outDetail("Initializing mounts...");
    InitMounts();

    sLog.outDetail("Initializing skills (step 2)...");
    UpdateTradeSkills();
	
	if (bot->GetLevel() >= sPlayerbotAIConfig.minEnchantingBotLevel)
	{
		sLog.outDetail("Initializing enchant templates...");
		LoadEnchantContainer();
	}

    sLog.outDetail("Initializing bags...");
    InitBags();

    sLog.outDetail("Initializing ammo...");
    InitAmmo();

    sLog.outDetail("Initializing food...");
    InitFood();

    sLog.outDetail("Initializing potions...");
    InitPotions();

    sLog.outDetail("Initializing reagents...");
    InitReagents();

	if (bot->GetLevel() >= sPlayerbotAIConfig.minEnchantingBotLevel)
	{
		ApplyEnchantTemplate();
	}

    sLog.outDetail("Initializing inventory...");
    InitInventory();
    AddConsumables();
    
    sLog.outDetail("Initializing guilds & ArenaTeams");

#ifndef MANGOSBOT_ZERO
    if (bot->GetLevel() >= 70)
        InitArenaTeam();
#endif

	if (bot->GetLevel() >= 10) {
		sLog.outDetail("Initializing pet...");
		InitPet();
	}

    if (incremental)
    {
        uint32 money = bot->GetMoney();
        bot->SetMoney(money + 1000 * sqrt(urand(1, level * 5)));
    }
    else
    {
        bot->SetMoney(10000 * sqrt(urand(1, level * 5)));
    }

    sLog.outDetail("Saving to DB...");
    bot->SaveToDB();
    sLog.outDetail("Done.");

    ai->DoSpecificAction("equip upgrades");
}

void PlayerbotFactory::Refresh()
{
    Prepare();
    InitAmmo();
    InitFood();
    InitPotions();
    InitReagents();
    bot->SaveToDB();
}

void PlayerbotFactory::AddConsumables()
{
   switch (bot->getClass())
   {
      case CLASS_PRIEST:
      case CLASS_MAGE:
      case CLASS_WARLOCK:
      {
         if (level >= 5 && level < 20) {
            StoreItem(CONSUM_ID_MINOR_WIZARD_OIL, 5);
            }
         if (level >= 20 && level < 40) {
            StoreItem(CONSUM_ID_MINOR_MANA_OIL, 5);
            StoreItem(CONSUM_ID_MINOR_WIZARD_OIL, 5);
         }
         if (level >= 40 && level < 45) {
             StoreItem(CONSUM_ID_MINOR_MANA_OIL, 5);
             StoreItem(CONSUM_ID_WIZARD_OIL, 5);
         }
#ifdef MANGOSBOT_ZERO
         if (level >= 45 ) {
             StoreItem(CONSUM_ID_BRILLIANT_MANA_OIL, 5);
             StoreItem(CONSUM_ID_BRILLIANT_WIZARD_OIL, 5);
         }
#else
         if (level >= 45 && level < 52) {
            StoreItem(CONSUM_ID_BRILLIANT_MANA_OIL, 5);
            StoreItem(CONSUM_ID_BRILLIANT_WIZARD_OIL, 5);
         }
         if (level >= 52 && level < 58) {
            StoreItem(CONSUM_ID_SUPERIOR_MANA_OIL, 5);
            StoreItem(CONSUM_ID_BRILLIANT_WIZARD_OIL, 5);
         }
         if (level >= 58) {
           StoreItem(CONSUM_ID_SUPERIOR_MANA_OIL, 5);
           StoreItem(CONSUM_ID_SUPERIOR_WIZARD_OIL, 5);
         }
#endif
   }
      break;
      case CLASS_PALADIN:
      case CLASS_WARRIOR:
      case CLASS_HUNTER:
       {
         if (level >= 1 && level < 5) {
            StoreItem(CONSUM_ID_ROUGH_SHARPENING_STONE, 5);
            StoreItem(CONSUM_ID_ROUGH_WEIGHTSTONE, 5);
        }
         if (level >= 5 && level < 15) {
            StoreItem(CONSUM_ID_COARSE_WEIGHTSTONE, 5);
            StoreItem(CONSUM_ID_COARSE_SHARPENING_STONE, 5);
         }
         if (level >= 15 && level < 25) {
            StoreItem(CONSUM_ID_HEAVY_WEIGHTSTONE, 5);
            StoreItem(CONSUM_ID_HEAVY_SHARPENING_STONE, 5);
         }
         if (level >= 25 && level < 35) {
            StoreItem(CONSUM_ID_SOL_SHARPENING_STONE, 5);
            StoreItem(CONSUM_ID_SOLID_WEIGHTSTONE, 5);
         }
#ifdef MANGOSBOT_ZERO
         if (level >= 35) {
             StoreItem(CONSUM_ID_DENSE_WEIGHTSTONE, 5);
             StoreItem(CONSUM_ID_DENSE_SHARPENING_STONE, 5);
         }
#else
         if (level >= 35 && level < 50) {
            StoreItem(CONSUM_ID_DENSE_WEIGHTSTONE, 5);
            StoreItem(CONSUM_ID_DENSE_SHARPENING_STONE, 5);
         }
         if (level >= 50 && level < 60) {
            StoreItem(CONSUM_ID_FEL_SHARPENING_STONE, 5);
            StoreItem(CONSUM_ID_FEL_WEIGHTSTONE, 5);
         }
         if (level >= 60) {
            StoreItem(CONSUM_ID_ADAMANTITE_WEIGHTSTONE, 5);
            StoreItem(CONSUM_ID_ADAMANTITE_SHARPENING_STONE, 5);
         }
#endif
   }
       break;
       case CLASS_ROGUE:
      {
         if (level >= 20 && level < 28) {
            StoreItem(CONSUM_ID_INSTANT_POISON, 5);
         }
         if (level >= 28 && level < 30) {
            StoreItem(CONSUM_ID_INSTANT_POISON_II, 5);
         }
         if (level >= 30 && level < 36) {
            StoreItem(CONSUM_ID_DEADLY_POISON, 5);
            StoreItem(CONSUM_ID_INSTANT_POISON_II, 5);
         }
         if (level >= 36 && level < 44) {
            StoreItem(CONSUM_ID_DEADLY_POISON_II, 5);
           StoreItem(CONSUM_ID_INSTANT_POISON_III, 5);
         }
         if (level >= 44 && level < 52) {
            StoreItem(CONSUM_ID_DEADLY_POISON_III, 5);
            StoreItem(CONSUM_ID_INSTANT_POISON_IV, 5);
         }
         if (level >= 52 && level < 60) {
            StoreItem(CONSUM_ID_DEADLY_POISON_IV, 5);
            StoreItem(CONSUM_ID_INSTANT_POISON_V, 5);
         }
         if (level >= 60 && level < 62) {
            StoreItem(CONSUM_ID_DEADLY_POISON_V, 5);
            StoreItem(CONSUM_ID_INSTANT_POISON_VI, 5);
         }
         if (level >= 62 && level < 68) {
            StoreItem(CONSUM_ID_DEADLY_POISON_VI, 5);
            StoreItem(CONSUM_ID_INSTANT_POISON_VI, 5);
         }
         if (level >= 68) {
            StoreItem(CONSUM_ID_DEADLY_POISON_VII, 5);
            StoreItem(CONSUM_ID_INSTANT_POISON_VII, 5);
         }
         break;
      }
   }
}

void PlayerbotFactory::InitPet()
{
    Pet* pet = bot->GetPet();
    if (!pet)
    {
        if (bot->getClass() != CLASS_HUNTER)
            return;

        Map* map = bot->GetMap();
        if (!map)
            return;

		vector<uint32> ids;
        for (uint32 id = 0; id < sCreatureStorage.GetMaxEntry(); ++id)
        {
            CreatureInfo const* co = sCreatureStorage.LookupEntry<CreatureInfo>(id);
			if (!co)
				continue;

#ifdef MANGOSBOT_TWO
            if (!co->isTameable(bot->CanTameExoticPets()))
#else
            if (!co->isTameable())
#endif
                continue;

            if ((int)co->MinLevel > (int)bot->GetLevel())
                continue;

			ids.push_back(id);
		}

        if (ids.empty())
        {
            sLog.outError("No pets available for bot %s (%d level)", bot->GetName(), bot->GetLevel());
            return;
        }

		for (int i = 0; i < 100; i++)
		{
			int index = urand(0, ids.size() - 1);
            CreatureInfo const* co = sCreatureStorage.LookupEntry<CreatureInfo>(ids[index]);
            if (!co)
                continue;

            uint32 guid = map->GenerateLocalLowGuid(HIGHGUID_PET);
#ifdef MANGOSBOT_TWO
            CreatureCreatePos pos(map, bot->GetPositionX(), bot->GetPositionY(), bot->GetPositionZ(), bot->GetOrientation(), bot->GetPhaseMask());
#else
            CreatureCreatePos pos(map, bot->GetPositionX(), bot->GetPositionY(), bot->GetPositionZ(), bot->GetOrientation());
#endif
            uint32 pet_number = sObjectMgr.GeneratePetNumber();
            pet = new Pet(HUNTER_PET);
            if (!pet->Create(guid, pos, co, pet_number))
            {
                delete pet;
                pet = NULL;
                continue;
            }

            pet->SetOwnerGuid(bot->GetObjectGuid());
            pet->SetGuidValue(UNIT_FIELD_CREATEDBY, bot->GetObjectGuid());
            pet->SetFaction(bot->GetFaction());
            pet->SetLevel(bot->GetLevel());
            pet->InitStatsForLevel(bot->GetLevel());
#ifdef MANGOSBOT_TWO

#else
            pet->SetLoyaltyLevel(BEST_FRIEND);
#endif
            pet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2);
            pet->GetCharmInfo()->SetPetNumber(sObjectMgr.GeneratePetNumber(), true);
            pet->AIM_Initialize();
#ifdef CMANGOS
            pet->GetMap()->Add((Creature*)pet);
            pet->AIM_Initialize();
#endif
#ifdef MANGOS
            pet->GetMap()->Add((Creature*)pet);
            pet->AIM_Initialize();
#endif
            pet->InitPetCreateSpells();
#ifdef CMANGOS
            pet->LearnPetPassives();
            pet->CastPetAuras(true);
            pet->CastOwnerTalentAuras();
            pet->UpdateAllStats();
#endif
#ifdef MANGOS
            pet->LearnPetPassives();
            pet->CastPetAuras(true);
            pet->CastOwnerTalentAuras();
            pet->UpdateAllStats();
#endif
            bot->SetPet(pet);
            bot->SetPetGuid(pet->GetObjectGuid());

            sLog.outDebug(  "Bot %s: assign pet %d (%d level)", bot->GetName(), co->Entry, bot->GetLevel());
            pet->SavePetToDB(PET_SAVE_AS_CURRENT
#ifdef CMANGOS
                    , bot
#endif
            );
            bot->PetSpellInitialize();
            break;
        }
    }

    pet = bot->GetPet();
    if (pet)
    {
        pet->InitStatsForLevel(bot->GetLevel());
        pet->SetLevel(bot->GetLevel());
#ifdef MANGOSBOT_TWO

#else
        pet->SetLoyaltyLevel(BEST_FRIEND);
#endif
        pet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2);
        pet->SetHealth(pet->GetMaxHealth());
    }
    else
    {
        sLog.outError("Cannot create pet for bot %s", bot->GetName());
        return;
    }

    for (PetSpellMap::const_iterator itr = pet->m_spells.begin(); itr != pet->m_spells.end(); ++itr)
    {
        if(itr->second.state == PETSPELL_REMOVED)
            continue;

        uint32 spellId = itr->first;
        if(IsPassiveSpell(spellId))
            continue;

        pet->ToggleAutocast(spellId, true);
    }
}

void PlayerbotFactory::ClearSkills()
{
    for (int i = 0; i < sizeof(tradeSkills) / sizeof(uint32); ++i)
    {
        bot->SetSkill(tradeSkills[i], 0, 0, 0);
    }
    bot->SetUInt32Value(PLAYER_SKILL_INDEX(0), 0);
    bot->SetUInt32Value(PLAYER_SKILL_INDEX(1), 0);
}

void PlayerbotFactory::ClearSpells()
{
#ifdef MANGOS
    list<uint32> spells;
    for(PlayerSpellMap::iterator itr = bot->GetSpellMap().begin(); itr != bot->GetSpellMap().end(); ++itr)
    {
        uint32 spellId = itr->first;
		if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled || IsPassiveSpell(spellId))
			continue;

        spells.push_back(spellId);
    }

    for (list<uint32>::iterator i = spells.begin(); i != spells.end(); ++i)
    {
        bot->removeSpell(*i, false, false);
    }
#endif
#ifdef CMANGOS
    bot->resetSpells();
#endif
}

void PlayerbotFactory::InitSpells()
{
    for (int i = 0; i < 15; i++)
        InitAvailableSpells();
}

void PlayerbotFactory::InitTalentsTree(bool incremental)
{
    uint32 specNo = sRandomPlayerbotMgr.GetValue(bot->GetGUIDLow(), "specNo");
    if (incremental && specNo)
    {
        specNo -=  1;
    }
    else
    {
        uint32 point = urand(0, 100);
        uint8 cls = bot->getClass();
        uint32 p1 = sPlayerbotAIConfig.specProbability[cls][0];
        uint32 p2 = p1 + sPlayerbotAIConfig.specProbability[cls][1];

        specNo = (point < p1 ? 0 : (point < p2 ? 1 : 2));
        sRandomPlayerbotMgr.SetValue(bot, "specNo", specNo + 1);
    }

    InitTalents(specNo);

    if (bot->GetFreeTalentPoints()) {
        InitTalents(2 - specNo);
    }
}


class DestroyItemsVisitor : public IterateItemsVisitor
{
public:
    DestroyItemsVisitor(Player* bot) : IterateItemsVisitor(), bot(bot) {}

    virtual bool Visit(Item* item)
    {
        uint32 id = item->GetProto()->ItemId;
        if (CanKeep(id))
        {
            keep.insert(id);
            return true;
        }

        bot->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
        return true;
    }

private:
    bool CanKeep(uint32 id)
    {
        if (keep.find(id) != keep.end())
            return false;

        if (sPlayerbotAIConfig.IsInRandomQuestItemList(id))
            return true;

        return false;
    }

private:
    Player* bot;
    set<uint32> keep;

};

bool PlayerbotFactory::ShouldEquipArmor(Item* item)
{
    ItemPrototype const* proto = item->GetProto();

    if (proto->InventoryType == INVTYPE_TABARD)
        return true;

    if (bot->HasSkill(SKILL_SHIELD) && proto->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD)
        return true;

    else if (bot->HasSkill(SKILL_MAIL))
    {
        if (proto->SubClass > ITEM_SUBCLASS_ARMOR_MAIL && proto->SubClass <= ITEM_SUBCLASS_ARMOR_PLATE)
            return false;
    }
    else if (bot->HasSkill(SKILL_LEATHER))
    {
        if (proto->SubClass > ITEM_SUBCLASS_ARMOR_LEATHER && proto->SubClass <= ITEM_SUBCLASS_ARMOR_PLATE)
            return false;
    }

    if (proto->Quality <= ITEM_QUALITY_NORMAL)
        return true;

    uint8 sp = 0, ap = 0, tank = 0;
    for (int j = 0; j < MAX_ITEM_PROTO_STATS; ++j)
    {
        // for ItemStatValue != 0
        if(!proto->ItemStat[j].ItemStatValue)
            continue;

        AddItemStats(proto->ItemStat[j].ItemStatType, sp, ap, tank);
    }

    //add enchantments for items with random values e.g. item id 6572    
    if (item->GetItemRandomPropertyId())
    {
        for (uint32 i = PROP_ENCHANTMENT_SLOT_2; i < PROP_ENCHANTMENT_SLOT_2 + 3; ++i)
        {
            EnchantmentSlot slot = EnchantmentSlot(i);
            int enchantmentId = item->GetEnchantmentId(slot);

            SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchantmentId);
            if (enchantEntry)
            {
                const SpellEntry* enchantSpell = sServerFacade.LookupSpellInfo(enchantEntry->spellid[0]);                
                         
                if (enchantSpell)
                {
                    Stats stats = (Stats)enchantSpell->EffectMiscValue[0];
                    //int multiplier = enchantSpell->EffectBasePoints[0] + 1;

                    switch (stats)
                    {
                    case STAT_STRENGTH: AddItemStats(ITEM_MOD_STRENGTH, sp, ap, tank); break;
                    case STAT_AGILITY: AddItemStats(ITEM_MOD_AGILITY, sp, ap, tank); break;
                    case STAT_STAMINA: AddItemStats(ITEM_MOD_STAMINA, sp, ap, tank); break;
                    case STAT_INTELLECT: AddItemStats(ITEM_MOD_INTELLECT, sp, ap, tank); break;
                    case STAT_SPIRIT: AddItemStats(ITEM_MOD_SPIRIT, sp, ap, tank); break;
                    }                            
                }
            }
        }
    }

    return CheckItemStats(sp, ap, tank);
}

bool PlayerbotFactory::CheckItemStats(uint8 sp, uint8 ap, uint8 tank)
{
    switch (bot->getClass())
    {
    case CLASS_PRIEST:
    case CLASS_MAGE:
    case CLASS_WARLOCK:
        if (!sp || ap > sp || tank > sp)
            return false;
        break;
    case CLASS_PALADIN:
    case CLASS_WARRIOR:
        if ((!ap && !tank) || sp > ap || sp > tank)
            return false;
        break;
    case CLASS_HUNTER:
    case CLASS_ROGUE:
        if (!ap || sp > ap || sp > tank)
            return false;
        break;
#ifdef MANGOSBOT_TWO
    case CLASS_DEATH_KNIGHT:
        if ((!ap && !tank) || sp > ap || sp > tank)
            return false;
        break;
#endif
    }

    return sp || ap || tank;
}

void PlayerbotFactory::AddItemStats(uint32 mod, uint8 &sp, uint8 &ap, uint8 &tank)
{
    switch (mod)
    {
    case ITEM_MOD_HEALTH:
    case ITEM_MOD_STAMINA:
    case ITEM_MOD_MANA:
    case ITEM_MOD_INTELLECT:
    case ITEM_MOD_SPIRIT:
        sp++;
        break;
    }

    switch (mod)
    {
    case ITEM_MOD_AGILITY:
    case ITEM_MOD_STRENGTH:
    case ITEM_MOD_HEALTH:
    case ITEM_MOD_STAMINA:
        tank++;
        break;
    }

    switch (mod)
    {
    case ITEM_MOD_HEALTH:
    case ITEM_MOD_STAMINA:
    case ITEM_MOD_AGILITY:
    case ITEM_MOD_STRENGTH:
        ap++;
        break;
    }
}

bool PlayerbotFactory::ShouldEquipWeapon(Item* item)
{
    ItemPrototype const* proto = item->GetProto();

   int tab = AiFactory::GetPlayerSpecTab(bot);

   switch (bot->getClass())
   {
   case CLASS_PRIEST:
      if (proto->SubClass != ITEM_SUBCLASS_WEAPON_STAFF &&
         proto->SubClass != ITEM_SUBCLASS_WEAPON_WAND &&
         proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE)
         return false;
      break;
   case CLASS_MAGE:
     if (proto->SubClass != ITEM_SUBCLASS_WEAPON_STAFF &&
         proto->SubClass != ITEM_SUBCLASS_WEAPON_WAND)
         return false;
      break;
   case CLASS_WARLOCK:
      if (proto->SubClass != ITEM_SUBCLASS_WEAPON_STAFF &&
         proto->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER &&
         proto->SubClass != ITEM_SUBCLASS_WEAPON_WAND &&
         proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD)
         return false;
      break;
   case CLASS_WARRIOR:
      if (tab == 1) //fury
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_AXE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_FIST &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
            return false;
      }
      if ((tab == 0) && (bot->GetLevel() > 10))   //arms
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_AXE2 &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_POLEARM &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
            return false;
      }
      else //prot +lowlvl
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_AXE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
            return false;
      }
      break;
   case CLASS_PALADIN:
       if (tab == 2 && bot->GetLevel() > 10) //Retri
       {
           if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
               proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2)
               return false;
       }
       else
       {
            if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
                proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
                proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
                proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD)
                return false;
       }
      break;
   case CLASS_SHAMAN:
      if (tab == 1) //enh
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_FIST &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_AXE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_AXE2 &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE2)
            return false;
      }
      else //ele,resto
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_STAFF)
            return false;
      }
      break;
   case CLASS_DRUID:
      if (tab == 1) //feral
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_STAFF)
            return false;
      }
      else //ele,resto
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_STAFF)
            return false;
      }
      break;
   case CLASS_HUNTER:
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_AXE2 &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_POLEARM &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_STAFF &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_BOW)
            return false;
      break;
   case CLASS_ROGUE:
      if (tab == 0) //assa
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
            return false;
      }
      else
      {
         if (proto->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_FIST &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
            proto->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
            return false;
      }
      break;
#ifdef MANGOSBOT_TWO
   case CLASS_DEATH_KNIGHT:
       if (proto->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
           proto->SubClass != ITEM_SUBCLASS_WEAPON_POLEARM &&
           proto->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
           proto->SubClass != ITEM_SUBCLASS_WEAPON_AXE2)
           return false;
       break;
#endif
   }

   return true;
}

bool PlayerbotFactory::CanEquipItem(ItemPrototype const* proto, uint32 desiredQuality)
{
    if (proto->Duration & 0x80000000)
        return false;

    if (proto->Quality != desiredQuality)
        return false;

    if (proto->Bonding == BIND_QUEST_ITEM || proto->Bonding == BIND_WHEN_USE)
        return false;

    if (proto->Class == ITEM_CLASS_CONTAINER)
        return true;

    uint32 requiredLevel = proto->RequiredLevel;
    if (!requiredLevel)
        return false;

    uint32 level = bot->GetLevel();
    uint32 delta = 2;
    if (level < 15)
        delta = urand(7, 15);
    else if (proto->Class == ITEM_CLASS_WEAPON || proto->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD)
        delta = urand(2, 3);
    else if (!(level % 10) || (level % 10) == 9)
        delta = 2;
    else if (level < 40)
        delta = urand(5, 10);
    else if (level < 60)
        delta = urand(3, 7);
    else if (level < 70)
        delta = urand(2, 5);
    else if (level < 80)
        delta = urand(2, 4);

    if (desiredQuality > ITEM_QUALITY_NORMAL &&
            (requiredLevel > level || requiredLevel < level - delta))
        return false;

    for (uint32 gap = 60; gap <= 80; gap += 10)
    {
        if (level > gap && requiredLevel <= gap)
            return false;
    }

    return true;
}

void PlayerbotFactory::InitEquipment(bool incremental)
{
	//if (bot->getLevel() >= 10)
	//{
        DestroyItemsVisitor visitor(bot);
        IterateItems(&visitor, ITERATE_ALL_ITEMS);
	//}

    for(uint8 slot = 0; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (slot == EQUIPMENT_SLOT_TABARD && !bot->GetGuildId())
            continue;

        bool found = false;
        uint32 quality = itemQuality;
        if (urand(0, 100) < 100 * sPlayerbotAIConfig.randomGearLoweringChance && quality > ITEM_QUALITY_NORMAL) {
            quality--;
        }
        do
        {
            vector<uint32> ids = sRandomItemMgr.Query(level, bot->getClass(), slot, quality);

            if (!ids.empty()) ahbot::Shuffle(ids);
            for (uint32 index = 0; index < ids.size(); ++index)
            {
                uint32 newItemId = ids[index];
                Item* oldItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

                if (incremental && !IsDesiredReplacement(oldItem)) {
                    continue;
                }

                uint16 dest;
                if (!CanEquipUnseenItem(slot, dest, newItemId))
                    continue;

                if (oldItem)
                {
                    bot->RemoveItem(INVENTORY_SLOT_BAG_0, slot, true);
                    oldItem->DestroyForPlayer(bot);
                }

                Item* newItem = bot->EquipNewItem(dest, newItemId, true);
                if (newItem)
                {
                    //newItem->AddToWorld();
                    //newItem->AddToUpdateQueueOf(bot);
                    bot->AutoUnequipOffhandIfNeed();
                    EnchantItem(newItem);
                    found = true;
                    break;
                }
            }
            quality--;
        } while (!found && quality != ITEM_QUALITY_POOR);
        if (!found)
        {
            sLog.outDetail(  "%s: no items to equip for slot %d", bot->GetName(), slot);
            continue;
        }
    }
}

bool PlayerbotFactory::IsDesiredReplacement(Item* item)
{
    if (!item)
        return true;

    ItemPrototype const* proto = item->GetProto();
    int delta = 1 + (80 - bot->GetLevel()) / 10;
    return (int)bot->GetLevel() - (int)proto->RequiredLevel > delta;
}


void PlayerbotFactory::InitBags()
{
    for (uint8 slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; ++slot)
    {
        Bag* pBag = (Bag*)bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (!pBag)
            bot->StoreNewItemInBestSlots(4500, 1); // add Traveler's Backpack if no bag in slot
    }

    /*vector<uint32> ids;

    for (uint32 itemId = 0; itemId < sItemStorage.GetMaxEntry(); ++itemId)
    {
        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
        if (!proto || proto->Class != ITEM_CLASS_CONTAINER)
            continue;

        if (!CanEquipItem(proto, ITEM_QUALITY_NORMAL))
            continue;

        ids.push_back(itemId);
    }

    if (ids.empty())
    {
        sLog.outError("%s: no bags found", bot->GetName());
        return;
    }

    for (uint8 slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; ++slot)
    {
        for (int attempts = 0; attempts < 15; attempts++)
        {
            uint32 index = urand(0, ids.size() - 1);
            uint32 newItemId = ids[index];

            uint16 dest;
            if (!CanEquipUnseenItem(slot, dest, newItemId))
                continue;
            Item* newItem = bot->EquipNewItem(dest, newItemId, true);
            if (newItem)
            {
                newItem->AddToWorld();
                newItem->AddToUpdateQueueOf(bot);
                break;
            }
        }
    }*/
}

void PlayerbotFactory::EnchantItem(Item* item)
{
#ifdef CMANGOS
    if (item->GetOwner() == nullptr)
        return;
#endif
    if (urand(0, 100) < 100 * sPlayerbotAIConfig.randomGearLoweringChance)
        return;

    if (bot->GetLevel() < urand(40, 50))
        return;

    ItemPrototype const* proto = item->GetProto();
    int32 itemLevel = proto->ItemLevel;

    vector<uint32> ids;
    for (uint32 id = 0; id < sServerFacade.GetSpellInfoRows(); ++id)
    {
        SpellEntry const *entry = sServerFacade.LookupSpellInfo(id);
        if (!entry)
            continue;

        int32 requiredLevel = (int32)entry->baseLevel;
        if (requiredLevel && (requiredLevel > itemLevel || requiredLevel < itemLevel - 35))
            continue;

        if (entry->maxLevel && level > entry->maxLevel)
            continue;

        uint32 spellLevel = entry->spellLevel;
        if (spellLevel && (spellLevel > level || spellLevel < level - 10))
            continue;
#ifdef CMANGOS
        for (int j = 0; j < MAX_SPELL_EFFECTS; ++j)
#endif
#ifdef MANGOS
        for (int j = 0; j < TOTAL_SPELL_EFFECTS; ++j)
#endif
        {
            if (entry->Effect[j] != SPELL_EFFECT_ENCHANT_ITEM)
                continue;

            uint32 enchant_id = entry->EffectMiscValue[j];
            if (!enchant_id)
                continue;

            SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
            if (!enchant || enchant->flags != PERM_ENCHANTMENT_SLOT)
                continue;

			const SpellEntry *enchantSpell = sServerFacade.LookupSpellInfo(enchant->spellid[0]);
            if (!enchantSpell || (enchantSpell->spellLevel && enchantSpell->spellLevel > level))
                continue;

            uint8 sp = 0, ap = 0, tank = 0;
            for (int i = ITEM_MOD_MANA; i < MAX_ITEM_MOD; ++i)
            {
                if (enchant->type[i] != ITEM_ENCHANTMENT_TYPE_STAT)
                    continue;

                AddItemStats(enchant->spellid[i], sp, ap, tank);
            }

            if (!CheckItemStats(sp, ap, tank))
                continue;
/*#ifndef MANGOSBOT_ZERO
            if (enchant->EnchantmentCondition && !bot->EnchantmentFitsRequirements(enchant->EnchantmentCondition, -1))
               continue;
#endif*/
            if (!item->IsFitToSpellRequirements(entry))
                continue;

            ids.push_back(enchant_id);
        }
    }

    if (ids.empty())
    {
        sLog.outDebug(  "%s: no enchantments found for item %d", bot->GetName(), item->GetProto()->ItemId);
        return;
    }

    int index = urand(0, ids.size() - 1);
    uint32 id = ids[index];

    SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(id);
    if (!enchant)
        return;

    bot->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
#ifndef MANGOSBOT_TWO
    item->SetEnchantment(PERM_ENCHANTMENT_SLOT, id, 0, 0);
#else
    item->SetEnchantment(PERM_ENCHANTMENT_SLOT, id, 0, 0, bot->GetObjectGuid());
#endif
    bot->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);
}

bool PlayerbotFactory::CanEquipUnseenItem(uint8 slot, uint16 &dest, uint32 item)
{
    dest = 0;
    Item *pItem = RandomPlayerbotMgr::CreateTempItem(item, 1, bot);
    if (pItem)
    {
        InventoryResult result = bot->CanEquipItem(slot, dest, pItem, true, false);
        pItem->RemoveFromUpdateQueueOf(bot);
        delete pItem;
        return result == EQUIP_ERR_OK;
    }

    return false;
}

void PlayerbotFactory::InitTradeSkills()
{
    uint16 firstSkill = sRandomPlayerbotMgr.GetValue(bot, "firstSkill");
    uint16 secondSkill = sRandomPlayerbotMgr.GetValue(bot, "secondSkill");
    if (!firstSkill || !secondSkill)
    {
        vector<uint32> firstSkills;
        vector<uint32> secondSkills;
        switch (bot->getClass())
        {
        case CLASS_WARRIOR:
        case CLASS_PALADIN:
#ifdef MANGOSBOT_TWO
        case CLASS_DEATH_KNIGHT:
#endif
            firstSkills.push_back(SKILL_MINING);
            secondSkills.push_back(SKILL_BLACKSMITHING);
            secondSkills.push_back(SKILL_ENGINEERING);
            break;
        case CLASS_SHAMAN:
        case CLASS_DRUID:
        case CLASS_HUNTER:
        case CLASS_ROGUE:
            firstSkills.push_back(SKILL_SKINNING);
            secondSkills.push_back(SKILL_LEATHERWORKING);
            break;
        default:
            firstSkills.push_back(SKILL_TAILORING);
            secondSkills.push_back(SKILL_ENCHANTING);
        }

        switch (urand(0, 6))
        {
        case 0:
            firstSkill = SKILL_HERBALISM;
            secondSkill = SKILL_ALCHEMY;
            break;
        case 1:
            firstSkill = SKILL_HERBALISM;
            secondSkill = SKILL_MINING;
            break;
        case 2:
            firstSkill = SKILL_MINING;
            secondSkill = SKILL_SKINNING;
            break;
        case 3:
#ifdef MANGOSBOT_ZERO
            firstSkill = SKILL_HERBALISM;
            secondSkill = SKILL_SKINNING;
#else
			firstSkill = SKILL_JEWELCRAFTING;
			secondSkill = SKILL_MINING;
#endif
            break;
        default:
            firstSkill = firstSkills[urand(0, firstSkills.size() - 1)];
            secondSkill = secondSkills[urand(0, secondSkills.size() - 1)];
            break;
        }
        sRandomPlayerbotMgr.SetValue(bot, "firstSkill", firstSkill);
        sRandomPlayerbotMgr.SetValue(bot, "secondSkill", secondSkill);
    }

    SetRandomSkill(SKILL_FIRST_AID);
    SetRandomSkill(SKILL_FISHING);
    SetRandomSkill(SKILL_COOKING);

    SetRandomSkill(firstSkill);
    SetRandomSkill(secondSkill);
}

void PlayerbotFactory::UpdateTradeSkills()
{
    for (int i = 0; i < sizeof(tradeSkills) / sizeof(uint32); ++i)
    {
        if (bot->GetSkillValue(tradeSkills[i]) == 1)
            bot->SetSkill(tradeSkills[i], 0, 0, 0);
    }
}

void PlayerbotFactory::InitSkills()
{
    uint32 maxValue = level * 5;
    bot->UpdateSkillsForLevel(true);

// Riding skills requirements are different
#ifdef MANGOSBOT_ZERO
    if (bot->GetLevel() >= 60)
        bot->SetSkill(SKILL_RIDING, 150, 150);
    else if (bot->GetLevel() >= 40)
        bot->SetSkill(SKILL_RIDING, 75, 75);
#endif
#ifdef MANGOSBOT_ONE
    if (bot->GetLevel() >= 70)
        bot->SetSkill(SKILL_RIDING, 300, 300);
    else if (bot->GetLevel() >= 68)
        bot->SetSkill(SKILL_RIDING, 225, 225);
    else if (bot->GetLevel() >= 60)
        bot->SetSkill(SKILL_RIDING, 150, 150);
    else if (bot->GetLevel() >= 30)
        bot->SetSkill(SKILL_RIDING, 75, 75);
#endif
#ifdef MANGOSBOT_TWO
    if (bot->GetLevel() >= 70)
        bot->SetSkill(SKILL_RIDING, 300, 300);
    else if (bot->GetLevel() >= 60)
        bot->SetSkill(SKILL_RIDING, 225, 225);
    else if (bot->GetLevel() >= 40)
        bot->SetSkill(SKILL_RIDING, 150, 150);
    else if (bot->GetLevel() >= 20)
        bot->SetSkill(SKILL_RIDING, 75, 75);
#endif
    else
        bot->SetSkill(SKILL_RIDING, 0, 0);

    uint32 skillLevel = bot->GetLevel() < 40 ? 0 : 1;
    switch (bot->getClass())
    {
    case CLASS_WARRIOR:
    case CLASS_PALADIN:
        bot->SetSkill(SKILL_PLATE_MAIL, skillLevel, skillLevel);
        break;
    case CLASS_SHAMAN:
    case CLASS_HUNTER:
        bot->SetSkill(SKILL_MAIL, skillLevel, skillLevel);
    }

    switch (bot->getClass())
    {
    case CLASS_DRUID:
        SetRandomSkill(SKILL_MACES);
        SetRandomSkill(SKILL_STAVES);
        SetRandomSkill(SKILL_2H_MACES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_POLEARMS);
        SetRandomSkill(SKILL_FIST_WEAPONS);
        break;
    case CLASS_WARRIOR:
        SetRandomSkill(SKILL_SWORDS);
        SetRandomSkill(SKILL_AXES);
        SetRandomSkill(SKILL_BOWS);
        SetRandomSkill(SKILL_GUNS);
        SetRandomSkill(SKILL_MACES);
        SetRandomSkill(SKILL_2H_SWORDS);
        SetRandomSkill(SKILL_STAVES);
        SetRandomSkill(SKILL_2H_MACES);
        SetRandomSkill(SKILL_2H_AXES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_CROSSBOWS);
        SetRandomSkill(SKILL_POLEARMS);
        SetRandomSkill(SKILL_FIST_WEAPONS);
        SetRandomSkill(SKILL_THROWN);
        break;
    case CLASS_PALADIN:
        bot->SetSkill(SKILL_PLATE_MAIL, 0, skillLevel, skillLevel);
        SetRandomSkill(SKILL_SWORDS);
        SetRandomSkill(SKILL_AXES);
        SetRandomSkill(SKILL_MACES);
        SetRandomSkill(SKILL_2H_SWORDS);
        SetRandomSkill(SKILL_2H_MACES);
        SetRandomSkill(SKILL_2H_AXES);
        SetRandomSkill(SKILL_POLEARMS);
        break;
    case CLASS_PRIEST:
        SetRandomSkill(SKILL_MACES);
        SetRandomSkill(SKILL_STAVES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_WANDS);
        break;
    case CLASS_SHAMAN:
        SetRandomSkill(SKILL_AXES);
        SetRandomSkill(SKILL_MACES);
        SetRandomSkill(SKILL_STAVES);
        SetRandomSkill(SKILL_2H_MACES);
        SetRandomSkill(SKILL_2H_AXES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_FIST_WEAPONS);
        break;
    case CLASS_MAGE:
        SetRandomSkill(SKILL_SWORDS);
        SetRandomSkill(SKILL_STAVES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_WANDS);
        break;
    case CLASS_WARLOCK:
        SetRandomSkill(SKILL_SWORDS);
        SetRandomSkill(SKILL_STAVES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_WANDS);
        break;
    case CLASS_HUNTER:
        SetRandomSkill(SKILL_SWORDS);
        SetRandomSkill(SKILL_AXES);
        SetRandomSkill(SKILL_BOWS);
        SetRandomSkill(SKILL_GUNS);
        SetRandomSkill(SKILL_2H_SWORDS);
        SetRandomSkill(SKILL_STAVES);
        SetRandomSkill(SKILL_2H_AXES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_CROSSBOWS);
        SetRandomSkill(SKILL_POLEARMS);
        SetRandomSkill(SKILL_FIST_WEAPONS);
        SetRandomSkill(SKILL_THROWN);
        bot->SetSkill(SKILL_MAIL, 0, skillLevel, skillLevel);
        break;
    case CLASS_ROGUE:
        SetRandomSkill(SKILL_SWORDS);
        SetRandomSkill(SKILL_AXES);
        SetRandomSkill(SKILL_BOWS);
        SetRandomSkill(SKILL_GUNS);
        SetRandomSkill(SKILL_MACES);
        SetRandomSkill(SKILL_DAGGERS);
        SetRandomSkill(SKILL_CROSSBOWS);
        SetRandomSkill(SKILL_FIST_WEAPONS);
        SetRandomSkill(SKILL_THROWN);
        break;
#ifdef MANGOSBOT_TWO
    case CLASS_DEATH_KNIGHT:
        SetRandomSkill(SKILL_SWORDS);
        SetRandomSkill(SKILL_AXES);
        SetRandomSkill(SKILL_MACES);
        SetRandomSkill(SKILL_2H_SWORDS);
        SetRandomSkill(SKILL_2H_MACES);
        SetRandomSkill(SKILL_2H_AXES);
        SetRandomSkill(SKILL_POLEARMS);
        break;
#endif
    }
}

void PlayerbotFactory::SetRandomSkill(uint16 id)
{
    uint32 maxValue = level * 5; // vanilla 60*5 = 300

// do not let skill go beyond limit even if maxlevel > blizzlike
#ifndef MANGOSBOT_ZERO
	if (level > 60)
    {
#ifdef MANGOSBOT_ONE
        maxValue = (level + 5) * 5;   // tbc (70 + 5)*5 = 375
#else
        maxValue = (level + 10) * 5;  // wotlk (80 + 10)*5 = 450
#endif
	}
#endif

    uint32 curValue = bot->GetSkillValue(id);
    if (!bot->HasSkill(id) || maxValue > curValue)
        bot->SetSkill(id, maxValue, maxValue);
}

void PlayerbotFactory::InitAvailableSpells()
{
    bot->learnDefaultSpells();

    for (uint32 id = 0; id < sCreatureStorage.GetMaxEntry(); ++id)
    {
        CreatureInfo const* co = sCreatureStorage.LookupEntry<CreatureInfo>(id);
        if (!co)
            continue;

        if (co->TrainerType != TRAINER_TYPE_TRADESKILLS && co->TrainerType != TRAINER_TYPE_CLASS)
            continue;

        if (co->TrainerType == TRAINER_TYPE_CLASS && co->TrainerClass != bot->getClass())
            continue;

        uint32 trainerId = co->TrainerTemplateId;
        if (!trainerId)
            trainerId = co->Entry;

        TrainerSpellData const* trainer_spells = sObjectMgr.GetNpcTrainerTemplateSpells(trainerId);
        if (!trainer_spells)
            trainer_spells = sObjectMgr.GetNpcTrainerSpells(trainerId);

        if (!trainer_spells)
            continue;

        for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
        {
            TrainerSpell const* tSpell = &itr->second;

            if (!tSpell)
                continue;

            uint32 reqLevel = 0;

#ifdef CMANGOS
            if (!tSpell->learnedSpell && !bot->IsSpellFitByClassAndRace(tSpell->learnedSpell, &reqLevel))
                continue;
#endif

            reqLevel = tSpell->isProvidedReqLevel ? tSpell->reqLevel : std::max(reqLevel, tSpell->reqLevel);
            TrainerSpellState state = bot->GetTrainerSpellState(tSpell, reqLevel);
            if (state != TRAINER_SPELL_GREEN)
                continue;

            SpellEntry const* proto = sServerFacade.LookupSpellInfo(tSpell->spell);
            if (!proto)
                continue;

            if (co->TrainerType == TRAINER_TYPE_TRADESKILLS)
            {
                SpellEntry const* spell = sServerFacade.LookupSpellInfo(tSpell->spell);
                if (spell)
                {
                    string SpellName = spell->SpellName[0];
                    if (spell->Effect[EFFECT_INDEX_1] == SPELL_EFFECT_SKILL_STEP)
                    {
                        uint32 skill = spell->EffectMiscValue[EFFECT_INDEX_1];

                        if (skill && !bot->HasSkill(skill))
                        {
                            SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(skill);
                            if (pSkill)
                            {
                                if (SpellName.find("Apprentice") != string::npos && pSkill->categoryId == SKILL_CATEGORY_PROFESSION || pSkill->categoryId == SKILL_CATEGORY_SECONDARY)
                                    continue;
                            }
                        }
                    }
                }
            }

#ifdef CMANGOS
            if (tSpell->IsCastable())
                ai->CastSpell(tSpell->spell, bot);
            else
                bot->learnSpell(tSpell->learnedSpell, false);            
#endif

#ifdef MANGOS
            bool learned = false;
            for (int j = 0; j < 3; ++j)
            {
                if (proto->Effect[j] == SPELL_EFFECT_LEARN_SPELL)
                {
                    uint32 learnedSpell = proto->EffectTriggerSpell[j];
                    bot->learnSpell(learnedSpell, false);
                    learned = true;
                }
            }
            if (!learned) bot->learnSpell(tSpell->spell, false);
#endif
        }
    }
}


void PlayerbotFactory::InitSpecialSpells()
{
    for (list<uint32>::iterator i = sPlayerbotAIConfig.randomBotSpellIds.begin(); i != sPlayerbotAIConfig.randomBotSpellIds.end(); ++i)
    {
        uint32 spellId = *i;
        bot->learnSpell(spellId, false);
    }
}

void PlayerbotFactory::InitTalents(uint32 specNo)
{
    uint32 classMask = bot->getClassMask();

    map<uint32, vector<TalentEntry const*> > spells;
    for (uint32 i = 0; i < sTalentStore.GetNumRows(); ++i)
    {
        TalentEntry const *talentInfo = sTalentStore.LookupEntry(i);
        if(!talentInfo)
            continue;

        TalentTabEntry const *talentTabInfo = sTalentTabStore.LookupEntry( talentInfo->TalentTab );
        if(!talentTabInfo || talentTabInfo->tabpage != specNo)
            continue;

        if( (classMask & talentTabInfo->ClassMask) == 0 )
            continue;

        spells[talentInfo->Row].push_back(talentInfo);
    }

    uint32 freePoints = bot->GetFreeTalentPoints();
    for (map<uint32, vector<TalentEntry const*> >::iterator i = spells.begin(); i != spells.end(); ++i)
    {
        vector<TalentEntry const*> &spells = i->second;
        if (spells.empty())
        {
            sLog.outError("%s: No spells for talent row %d", bot->GetName(), i->first);
            continue;
        }

        int attemptCount = 0;
        while (!spells.empty() && (int)freePoints - (int)bot->GetFreeTalentPoints() < 5 && attemptCount++ < 3 && bot->GetFreeTalentPoints())
        {
            int index = urand(0, spells.size() - 1);
            TalentEntry const *talentInfo = spells[index];
            for (int rank = 0; rank < MAX_TALENT_RANK && bot->GetFreeTalentPoints(); ++rank)
            {
                uint32 spellId = talentInfo->RankID[rank];
                if (!spellId)
                    continue;

                bot->learnSpell(spellId, false);
                bot->UpdateFreeTalentPoints(false);
            }
            spells.erase(spells.begin() + index);
        }

        freePoints = bot->GetFreeTalentPoints();
    }
}

ObjectGuid PlayerbotFactory::GetRandomBot()
{
    vector<ObjectGuid> guids;
    for (list<uint32>::iterator i = sPlayerbotAIConfig.randomBotAccounts.begin(); i != sPlayerbotAIConfig.randomBotAccounts.end(); i++)
    {
        uint32 accountId = *i;
        if (!sAccountMgr.GetCharactersCount(accountId))
            continue;

        QueryResult *result = CharacterDatabase.PQuery("SELECT guid FROM characters WHERE account = '%u'", accountId);
        if (!result)
            continue;

        do
        {
            Field* fields = result->Fetch();
            ObjectGuid guid = ObjectGuid(fields[0].GetUInt64());
            if (!sObjectMgr.GetPlayer(guid))
                guids.push_back(guid);
        } while (result->NextRow());

        delete result;
    }

    if (guids.empty())
        return ObjectGuid();

    int index = urand(0, guids.size() - 1);
    return guids[index];
}


void PlayerbotFactory::AddPrevQuests(uint32 questId, list<uint32>& questIds)
{
    Quest const *quest = sObjectMgr.GetQuestTemplate(questId);
    for (Quest::PrevQuests::const_iterator iter = quest->prevQuests.begin(); iter != quest->prevQuests.end(); ++iter)
    {
        uint32 prevId = abs(*iter);
        AddPrevQuests(prevId, questIds);
        questIds.remove(prevId);
        questIds.push_back(prevId);
    }
}

void PlayerbotFactory::InitQuests(list<uint32>& questMap)
{
    int count = 0;
    for (list<uint32>::iterator i = questMap.begin(); i != questMap.end(); ++i)
    {
        uint32 questId = *i;
        Quest const *quest = sObjectMgr.GetQuestTemplate(questId);

        if (!bot->SatisfyQuestClass(quest, false) ||
                quest->GetMinLevel() > bot->GetLevel() ||
                !bot->SatisfyQuestRace(quest, false))
            continue;

        bot->SetQuestStatus(questId, QUEST_STATUS_COMPLETE);
        bot->RewardQuest(quest, 0, bot, false);
        sLog.outDetail("Bot %s (%d level) rewarded quest %d: %s (MinLevel=%d, QuestLevel=%d)",
                bot->GetName(), bot->GetLevel(), questId, quest->GetTitle().c_str(),
                quest->GetMinLevel(), quest->GetQuestLevel());
        if (!(count++ % 10))
            ClearInventory();
    }
    ClearInventory();
}

void PlayerbotFactory::ClearInventory()
{
    DestroyItemsVisitor visitor(bot);
    IterateItems(&visitor);
}

void PlayerbotFactory::ClearAllItems()
{
    DestroyItemsVisitor visitor(bot);
    IterateItems(&visitor, ITERATE_ALL_ITEMS);
}

void PlayerbotFactory::InitAmmo()
{
    if (bot->getClass() != CLASS_HUNTER && bot->getClass() != CLASS_ROGUE && bot->getClass() != CLASS_WARRIOR)
        return;

    Item* const pItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
    if (!pItem)
        return;

    uint32 subClass = 0;
    switch (pItem->GetProto()->SubClass)
    {
    case ITEM_SUBCLASS_WEAPON_GUN:
        subClass = ITEM_SUBCLASS_BULLET;
        break;
    case ITEM_SUBCLASS_WEAPON_BOW:
    case ITEM_SUBCLASS_WEAPON_CROSSBOW:
        subClass = ITEM_SUBCLASS_ARROW;
        break;
    }

    if (!subClass)
        return;

    uint32 entry = bot->GetUInt32Value(PLAYER_AMMO_ID);
    uint32 count = bot->GetItemCount(entry) / 200;
    uint32 maxCount = 5 + level / 10;

    if (!entry || count <= 2)
    {
        entry = sRandomItemMgr.GetAmmo(level, subClass);
    }

    if (count < maxCount)
    {
        for (int i = 0; i < maxCount - count; i++)
        {
            Item* newItem = bot->StoreNewItemInInventorySlot(entry, 200);
            if (newItem)
                newItem->AddToUpdateQueueOf(bot);
        }
    }
    bot->SetAmmo(entry);
}

void PlayerbotFactory::InitMounts()
{
    uint32 firstmount =
#ifdef MANGOSBOT_ZERO
        40
#else
#ifdef MANGOSBOT_ONE
        30
#else
        20
#endif
#endif
        ;

    uint32 secondmount =
#ifdef MANGOSBOT_ZERO
        60
#else
#ifdef MANGOSBOT_ONE
        60
#else
        40
#endif
#endif
        ;

    uint32 thirdmount =
#ifdef MANGOSBOT_ZERO
        90
#else
#ifdef MANGOSBOT_ONE
        68
#else
        60
#endif
#endif
        ;

    uint32 fourthmount =
#ifdef MANGOSBOT_ZERO
        90
#else
#ifdef MANGOSBOT_ONE
        70
#else
        70
#endif
#endif
        ;

    if (bot->GetLevel() < firstmount)
        return;

    map<uint8, map<int32, vector<uint32> > > mounts;
    vector<uint32> slow, fast, fslow, ffast;
    vector<uint32> ridingSkill = { 33388 , 33391, 34090, 34091 };

    switch (bot->getRace())
    {
    case RACE_HUMAN:
        slow = { 2411, 5655, 5656, 2414 };
        fast = { 18778, 18776, 18777 };
        break;
    case RACE_ORC:
        slow = { 5668, 5665, 1132 };
        fast = { 18796, 18798, 18797 };
        break;
    case RACE_DWARF:
        slow = { 5872, 5864, 5873 };
        fast = { 18786, 18787, 18785 };
        break;
    case RACE_NIGHTELF:
        slow = { 8632, 8631, 8629 };
        fast = { 18766, 18767, 18902 };
        break;
    case RACE_UNDEAD:
        slow = { 13332, 13333, 13331 };
        fast = { 13334, 18791 };
        break;
    case RACE_TAUREN:
        slow = { 15290, 15277 };
        fast = { 18794, 18795, 18793 };
        break;
    case RACE_GNOME:
        slow = { 8595, 13321, 8563, 13322 };
        fast = { 18772, 18773, 18774 };
        break;
    case RACE_TROLL:
        slow = { 8588, 8591, 8592 };
        fast = { 18788, 18789, 18790 };
        break;
#ifndef MANGOSBOT_ZERO
    case RACE_DRAENEI:
        slow = { 28481, 29743, 29744 };
        fast = { 29745, 29746, 29747 };
        break;
    case RACE_BLOODELF:
        slow = { 29220, 29221, 29222 };
        fast = { 29223, 28936, 35025, 29224 };
        break;

    }
    switch (bot->GetTeam())
    {
    case ALLIANCE:
        fslow = { 25470, 25471, 25472 };
        ffast = { 25473, 25527, 25528, 25529 };
        break;
    case HORDE:
        fslow = { 25475, 25476, 25474 };
        ffast = { 25531, 25533, 25477, 25532 };
        break;
#endif
    }
    mounts[bot->getRace()][0].insert(mounts[bot->getRace()][0].end(), slow.begin(), slow.end());
    mounts[bot->getRace()][1].insert(mounts[bot->getRace()][1].end(), fast.begin(), fast.end());
    mounts[bot->getRace()][2].insert(mounts[bot->getRace()][2].end(), fslow.begin(), fslow.end());
    mounts[bot->getRace()][3].insert(mounts[bot->getRace()][3].end(), ffast.begin(), ffast.end());

    for (uint32 type = 0; type < 4; type++)
    {
        if (bot->GetLevel() < secondmount && type == 1)
            continue;

        if (bot->GetLevel() < thirdmount && type == 2)
            continue;

        if (bot->GetLevel() < fourthmount && type == 3)
            continue;

        uint32 index = urand(0, mounts[bot->getRace()][type].size() - 1);
        uint32 mountItem = mounts[bot->getRace()][type][index];
        uint32 mountSkill = ridingSkill[type];

        bot->learnSpell(mountSkill, false);

        if (!bot->HasItemCount(mountItem,1, true))
        {            
            Item* newItem = bot->StoreNewItemInInventorySlot(mountItem, 1);
            if (newItem)
                newItem->AddToUpdateQueueOf(bot);

            sLog.outDetail("Bot %d (%d) earned %s mount %d", bot->GetGUIDLow(), bot->GetLevel(), type == 0 ? "slow" : (type == 1 ? "fast" : "flying"), mountItem);
        }
    }
}

void PlayerbotFactory::InitPotions()
{
    uint32 effects[] = { SPELL_EFFECT_HEAL, SPELL_EFFECT_ENERGIZE };

    for (int i = 0; i < 2; ++i)
    {
        uint32 effect = effects[i];
        FindPotionVisitor visitor(bot, effect);
        IterateItems(&visitor);
        if (!visitor.GetResult().empty()) continue;

        uint32 itemId = sRandomItemMgr.GetRandomPotion(level, effect);
        if (!itemId)
        {
            sLog.outDetail("No potions (type %d) available for bot %s (%d level)", effect, bot->GetName(), bot->GetLevel());
            continue;
        }

        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
        if (!proto) continue;

        uint32 maxCount = proto->GetMaxStackSize();
        Item* newItem = bot->StoreNewItemInInventorySlot(itemId, maxCount);
        if (newItem)
            newItem->AddToUpdateQueueOf(bot);
    }
}

void PlayerbotFactory::InitFood()
{
    uint32 categories[] = { 11, 59 };

    for (int i = 0; i < 2; ++i)
    {
        uint32 category = categories[i];

        FindFoodVisitor visitor(bot, category);
        IterateItems(&visitor);
        if (!visitor.GetResult().empty()) continue;

        uint32 itemId = sRandomItemMgr.GetFood(level, category);
        if (!itemId)
        {
            sLog.outDetail("No food (category %d) available for bot %s (%d level)", category, bot->GetName(), bot->GetLevel());
            continue;
        }
        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
        if (!proto) continue;

        uint32 maxCount = proto->GetMaxStackSize();
        Item* newItem = bot->StoreNewItemInInventorySlot(itemId, maxCount);
        if (newItem)
            newItem->AddToUpdateQueueOf(bot);
   }
}

void PlayerbotFactory::InitReagents()
{
    list<uint32> items;
    uint32 regCount = 1;
    switch (bot->getClass())
    {
    case CLASS_MAGE:
        regCount = 2;
        if (bot->GetLevel() > 11)
            items = { 17056 };
        if (bot->GetLevel() > 19)
            items = { 17056, 17031 };
        if (bot->GetLevel() > 35)
            items = { 17056, 17031, 17032 };
        if (bot->GetLevel() > 55)
            items = { 17056, 17031, 17032, 17020 };
        break;
    case CLASS_DRUID:
        regCount = 2;
        if (bot->GetLevel() > 19)
            items = { 17034 };
        if (bot->GetLevel() > 29)
            items = { 17035 };
        if (bot->GetLevel() > 39)
            items = { 17036 };
        if (bot->GetLevel() > 49)
            items = { 17037, 17021 };
        if (bot->GetLevel() > 59)
            items = { 17038, 17026 };
        if (bot->GetLevel() > 69)
            items = { 22147, 22148 };
        break;
    case CLASS_PALADIN:
        regCount = 3;
        if (bot->GetLevel() > 50)
            items = { 21177 };
        break;
    case CLASS_SHAMAN:
        regCount = 1;
        if (bot->GetLevel() > 22)
            items = { 17057 };
        if (bot->GetLevel() > 28)
            items = { 17057, 17058 };
        if (bot->GetLevel() > 29)
            items = { 17057, 17058, 17030 };
        break;
    case CLASS_WARLOCK:
        regCount = 10;
        if (bot->GetLevel() > 9)
            items = { 6265 };
        break;
    case CLASS_PRIEST:
        regCount = 3;
        if (bot->GetLevel() > 48)
            items = { 17028 };
        if (bot->GetLevel() > 55)
            items = { 17028, 17029 };
        break;
    case CLASS_ROGUE:
        regCount = 1;
        if (bot->GetLevel() > 21)
            items = { 5140 };
        if (bot->GetLevel() > 33)
            items = { 5140, 5530 };
        break;
    }

    for (list<uint32>::iterator i = items.begin(); i != items.end(); ++i)
    {
        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(*i);
        if (!proto)
        {
            sLog.outError("No reagent (ItemId %d) found for bot %d (Class:%d)", i, bot->GetGUIDLow(), bot->getClass());
            continue;
        }

        uint32 maxCount = proto->GetMaxStackSize();

        QueryItemCountVisitor visitor(*i);
        IterateItems(&visitor);
        if (visitor.GetCount() > maxCount) continue;

        uint32 randCount = urand(maxCount / 2, maxCount * regCount);

        Item* newItem = bot->StoreNewItemInInventorySlot(*i, randCount);
        if (newItem)
            newItem->AddToUpdateQueueOf(bot);

        sLog.outDetail("Bot %d got reagent %s x%d", bot->GetGUIDLow(), proto->Name1, randCount);
    }


    for (PlayerSpellMap::iterator itr = bot->GetSpellMap().begin(); itr != bot->GetSpellMap().end(); ++itr)
    {
        uint32 spellId = itr->first;

        if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled || IsPassiveSpell(spellId))
            continue;

        const SpellEntry* pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
        if (!pSpellInfo)
            continue;

        if (pSpellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL)
            continue;

        for (const auto& totem : pSpellInfo->Totem)
        {
            if (totem && !bot->HasItemCount(totem, 1))
            {
                ItemPrototype const* proto = sObjectMgr.GetItemPrototype(totem);
                if (!proto)
                {
                    sLog.outError("No totem (ItemId %d) found for bot %d (Class:%d)", totem, bot->GetGUIDLow(), bot->getClass());
                    continue;
                }

                Item* newItem = bot->StoreNewItemInInventorySlot(totem, 1);
                if (newItem)
                    newItem->AddToUpdateQueueOf(bot);

                sLog.outDetail("Bot %d got totem %s x%d", bot->GetGUIDLow(), proto->Name1, 1);
            }
        }
    }
}


void PlayerbotFactory::CancelAuras()
{
    bot->RemoveAllAuras();
}

void PlayerbotFactory::InitInventory()
{    
    InitInventorySkill();
}

void PlayerbotFactory::InitInventorySkill()
{
    if (bot->HasSkill(SKILL_MINING)) {
        if(!bot->HasItemCount(2901, 1, true))
            StoreItem(2901, 1); // Mining Pick
    }
    if (bot->HasSkill(SKILL_BLACKSMITHING) || bot->HasSkill(SKILL_ENGINEERING)) {
        if(!bot->HasItemCount(5956, 1, true))
            StoreItem(5956, 1); // Blacksmith Hammer
    }
    if (bot->HasSkill(SKILL_ENGINEERING)) {
        if (!bot->HasItemCount(6219, 1, true))
            StoreItem(6219, 1); // Arclight Spanner
    }
    if (bot->HasSkill(SKILL_ENCHANTING)) {
        if (!bot->HasItemCount(16207, 1, true))
            StoreItem(16207, 1); // Runed Arcanite Rod
    }
    if (bot->HasSkill(SKILL_SKINNING)) {
        if (!bot->HasItemCount(7005, 1, true))
            StoreItem(7005, 1); // Skinning Knife
    }

    //add hearthstone
    if (!bot->HasItemCount(6948, 1, true))
        StoreItem(6948, 1);
}

Item* PlayerbotFactory::StoreItem(uint32 itemId, uint32 count)
{
    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
    ItemPosCountVec sDest;
    InventoryResult msg = bot->CanStoreNewItem(INVENTORY_SLOT_BAG_0, NULL_SLOT, sDest, itemId, count);
    if (msg != EQUIP_ERR_OK)
        return NULL;

    return bot->StoreNewItem(sDest, itemId, true, Item::GenerateItemRandomPropertyId(itemId));
}

void PlayerbotFactory::InitInventoryTrade()
{
    uint32 itemId = sRandomItemMgr.GetRandomTrade(level);
    if (!itemId)
    {
        sLog.outError("No trade items available for bot %s (%d level)", bot->GetName(), bot->GetLevel());
        return;
    }

    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
    if (!proto)
        return;

    uint32 count = 1, stacks = 1;
    switch (proto->Quality)
    {
    case ITEM_QUALITY_NORMAL:
        count = proto->GetMaxStackSize();
        stacks = urand(1, 3) / auctionbot.GetRarityPriceMultiplier(proto);
        break;
    case ITEM_QUALITY_UNCOMMON:
        stacks = 1;

		int max = proto->GetMaxStackSize() / 2;

		if(max > 0)
			count = urand(1, max);
        break;
    }

    for (uint32 i = 0; i < stacks; i++)
        StoreItem(itemId, count);
}

void PlayerbotFactory::InitGuild()
{
    bot->SaveToDB();

    // add guild tabard
    if (bot->GetGuildId() && !bot->HasItemCount(5976, 1))
        StoreItem(5976, 1);

    if (bot->GetGuildId())
        return;

    if (sPlayerbotAIConfig.randomBotGuilds.empty())
        RandomPlayerbotFactory::CreateRandomGuilds();

    vector<uint32> guilds;
    for(list<uint32>::iterator i = sPlayerbotAIConfig.randomBotGuilds.begin(); i != sPlayerbotAIConfig.randomBotGuilds.end(); ++i)
        guilds.push_back(*i);

    if (guilds.empty())
    {
        sLog.outError("No random guilds available");
        return;
    }

    int index = urand(0, guilds.size() - 1);
    uint32 guildId = guilds[index];
    Guild* guild = sGuildMgr.GetGuildById(guildId);
    if (!guild)
    {
        sLog.outError("Invalid guild %u", guildId);
        return;
    }

    if (guild->GetMemberSize() < urand(10, 15))
        guild->AddMember(bot->GetObjectGuid(), urand(GR_OFFICER, GR_INITIATE));

    // add guild tabard
    if (bot->GetGuildId() && bot->GetLevel() > 9 && urand(0, 4) && !bot->HasItemCount(5976, 1))
        StoreItem(5976, 1);

    bot->SaveToDB();
}

#ifndef MANGOSBOT_ZERO
void PlayerbotFactory::InitArenaTeam()
{
    if (!sPlayerbotAIConfig.IsInRandomAccountList(bot->GetSession()->GetAccountId()))
        return;

    if (sPlayerbotAIConfig.randomBotArenaTeams.empty())
        RandomPlayerbotFactory::CreateRandomArenaTeams();

    vector<uint32> arenateams;
    for (list<uint32>::iterator i = sPlayerbotAIConfig.randomBotArenaTeams.begin(); i != sPlayerbotAIConfig.randomBotArenaTeams.end(); ++i)
        arenateams.push_back(*i);

    if (arenateams.empty())
    {
        sLog.outError("No random arena team available");
        return;
    }

    int index = urand(0, arenateams.size() - 1);
    uint32 arenateamID = arenateams[index];
    ArenaTeam* arenateam = sObjectMgr.GetArenaTeamById(arenateamID);
    if (!arenateam)
    {
        sLog.outError("Invalid arena team %u", arenateamID);
        return;
    }

    if (arenateam->GetMembersSize() < ((uint32)arenateam->GetType() * 2) && bot->GetLevel() >= 70)
    {
        ObjectGuid capt = arenateam->GetCaptainGuid();
        Player* botcaptain = sObjectMgr.GetPlayer(capt);

        if (botcaptain && botcaptain->GetTeam() == bot->GetTeam()) //need?
        {
            arenateam->AddMember(bot->GetObjectGuid());

            uint32 backgroundColor = urand(0xFF000000, 0xFFFFFFFF), emblemStyle = urand(0, 5), emblemColor = urand(0xFF000000, 0xFFFFFFFF), borderStyle = urand(0, 5), borderColor = urand(0xFF000000, 0xFFFFFFFF);

            arenateam->SetEmblem(backgroundColor, emblemStyle, emblemColor, borderStyle, borderColor);
        }
    }
    bot->SaveToDB();
}
#endif

void PlayerbotFactory::ApplyEnchantTemplate()
{
   int tab = AiFactory::GetPlayerSpecTab(bot);

   switch (bot->getClass())
   {
   case CLASS_WARRIOR:
      if (tab == 2)   
      ApplyEnchantTemplate(12);
      if (tab == 1)
         ApplyEnchantTemplate(11);
      else
        ApplyEnchantTemplate(10);
      break;
   case CLASS_DRUID:
      if (tab == 2)    
      ApplyEnchantTemplate(112);
      if (tab == 0) 
      ApplyEnchantTemplate(110);
      else 
      ApplyEnchantTemplate(111);
      break;
   case CLASS_SHAMAN:
      if (tab == 0)
         ApplyEnchantTemplate(70);
      if (tab == 2)
         ApplyEnchantTemplate(71);
      else
         ApplyEnchantTemplate(72);
      break;
   case CLASS_PALADIN:
      if (tab == 0)
         ApplyEnchantTemplate(20);
      if (tab == 2)
         ApplyEnchantTemplate(22);
      else if (tab == 1)
         ApplyEnchantTemplate(21);
      break;
   case CLASS_HUNTER:
      ApplyEnchantTemplate(30);
   case CLASS_ROGUE:
      ApplyEnchantTemplate(40);
      break;
   case CLASS_MAGE:
      ApplyEnchantTemplate(80);
   case CLASS_WARLOCK:
      ApplyEnchantTemplate(90);
      break;
   case CLASS_PRIEST:
         ApplyEnchantTemplate(50);
      break;
   }
}

void PlayerbotFactory::ApplyEnchantTemplate(uint8 spec)
{
 
   for (EnchantContainer::const_iterator itr = GetEnchantContainerBegin(); itr != GetEnchantContainerEnd(); ++itr)
      if ((*itr)->ClassId == bot->getClass() && (*itr)->SpecId == spec)
         ai->EnchantItemT((*itr)->SpellId, (*itr)->SlotId);
}

void PlayerbotFactory::LoadEnchantContainer()
{
   for (EnchantContainer::const_iterator itr = m_EnchantContainer.begin(); itr != m_EnchantContainer.end(); ++itr)
      delete *itr;

   m_EnchantContainer.clear();

   uint32 count = 0;

   QueryResult* result = PlayerbotDatabase.PQuery("SELECT class, spec, spellid, slotid FROM ai_playerbot_enchants");
   if (result)
   {
      do
      {
         Field* fields = result->Fetch();

         EnchantTemplate* pEnchant = new EnchantTemplate;

         pEnchant->ClassId = fields[0].GetUInt8();
         pEnchant->SpecId = fields[1].GetUInt8();
         pEnchant->SpellId = fields[2].GetUInt32();
         pEnchant->SlotId = fields[3].GetUInt8();

         m_EnchantContainer.push_back(pEnchant);
         ++count;
      } while (result->NextRow());

      delete result;
   }
}


float PlayerbotFactory::CalculateItemValue(Item* item)
{
    ItemPrototype const* proto = item->GetProto();

    int strength = 0, agility = 0, intellect = 0, stamina = 0, spirit = 0;

    for (int i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
    {
        // for ItemStatValue != 0
        if (!proto->ItemStat[i].ItemStatValue)
            continue;

        switch (proto->ItemStat[i].ItemStatType)
        {
        case ITEM_MOD_STRENGTH:
            strength = proto->ItemStat[i].ItemStatValue; break;
        case ITEM_MOD_AGILITY:
            agility = proto->ItemStat[i].ItemStatValue; break;
        case ITEM_MOD_INTELLECT:
            intellect = proto->ItemStat[i].ItemStatValue; break;
        case ITEM_MOD_STAMINA:
            stamina = proto->ItemStat[i].ItemStatValue; break;
        case ITEM_MOD_SPIRIT:
            spirit = proto->ItemStat[i].ItemStatValue; break;
        }
    }



    //if (bot->getClass)












    return 0.0f;






}

