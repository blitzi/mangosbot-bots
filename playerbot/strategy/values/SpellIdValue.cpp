#include "botpch.h"
#include "../../playerbot.h"
#include "SpellIdValue.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"
#ifdef MANGOSBOT_TWO
#include "Entities/Vehicle.h"
#endif
#include <regex>

using namespace ai;

SpellIdValue::SpellIdValue(PlayerbotAI* ai) :
        CalculatedValue<uint32>(ai, "spell id")
{
}

VehicleSpellIdValue::VehicleSpellIdValue(PlayerbotAI* ai) :
    CalculatedValue<uint32>(ai, "vehicle spell id")
{
}

uint32 SpellIdValue::Calculate()
{
    string namepart = qualifier;
    ItemIds itemIds = ChatHelper::parseItems(namepart);

    PlayerbotChatHandler handler(bot);
    uint32 extractedSpellId = handler.extractSpellId(namepart);
    if (extractedSpellId)
    {
        const SpellEntry* pSpellInfo = sServerFacade.LookupSpellInfo(extractedSpellId);
        if (pSpellInfo) namepart = pSpellInfo->SpellName[0];
    }

    wstring wnamepart;

    if (!Utf8toWStr(namepart, wnamepart))
        return 0;

    wstrToLower(wnamepart);
    char firstSymbol = tolower(namepart[0]);
    int spellLength = wnamepart.length();

    std::vector<uint32> spellIds;
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

        bool useByItem = false;
        for (int i = 0; i < 3; ++i)
        {
            if (pSpellInfo->Effect[i] == SPELL_EFFECT_CREATE_ITEM && itemIds.find(pSpellInfo->EffectItemType[i]) != itemIds.end())
            {
                useByItem = true;
                break;
            }
        }

        char* spellName = pSpellInfo->SpellName[0];
        char spellNameWithRank[256];
        
        if (pSpellInfo->Rank[0])
        {
            sprintf(spellNameWithRank, "%s %s", spellName, pSpellInfo->Rank[0]);
        }
        
        if (!useByItem && (tolower(spellName[0]) != firstSymbol || strlen(spellName) != spellLength || !Utf8FitTo(spellName, wnamepart)))
        {       
            if (!spellNameWithRank || (!useByItem && (tolower(spellNameWithRank[0]) != firstSymbol || strlen(spellNameWithRank) != spellLength || !Utf8FitTo(spellNameWithRank, wnamepart))))
                continue;

        }

        spellIds.push_back(spellId);
    }

    Pet* pet = bot->GetPet();
    if (spellIds.empty() && pet)
    {
        for (PetSpellMap::const_iterator itr = pet->m_spells.begin(); itr != pet->m_spells.end(); ++itr)
        {
            if(itr->second.state == PETSPELL_REMOVED)
                continue;

            uint32 spellId = itr->first;
            const SpellEntry* pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
            if (!pSpellInfo)
                continue;

            if (pSpellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL)
                continue;

            char* spellName = pSpellInfo->SpellName[0];
            if (tolower(spellName[0]) != firstSymbol || strlen(spellName) != spellLength || !Utf8FitTo(spellName, wnamepart))
                continue;

            spellIds.push_back(spellId);
        }
    }

    if (spellIds.empty()) return 0;

    sort(spellIds.begin(), spellIds.end(), [](uint32 spellIdA, uint32 spellIdB) -> bool {
        const SpellEntry* pSpellInfoA = sServerFacade.LookupSpellInfo(spellIdA);
        const SpellEntry* pSpellInfoB = sServerFacade.LookupSpellInfo(spellIdB);

        string rankA = regex_replace(pSpellInfoA->Rank[0], regex("[^0-9]*([0-9]+).*"), std::string("$1"));
        string rankB = regex_replace(pSpellInfoB->Rank[0], regex("[^0-9]*([0-9]+).*"), std::string("$1"));

        return std::atoi(rankA.c_str()) < std::atoi(rankB.c_str());
    });


    int saveMana = (int) round(AI_VALUE(double, "mana save level"));
    int rank = 1;
    int highest = 0;
    int lowest = 0;
    for (std::vector<uint32>::reverse_iterator i = spellIds.rbegin(); i != spellIds.rend(); ++i)
    {
        const SpellEntry* pSpellInfo = sServerFacade.LookupSpellInfo(*i);

        if (!highest) highest = *i;
        if (saveMana == rank) return *i;
        lowest = *i;
        rank++;
    }

    return saveMana > 1 ? lowest : highest;
}

uint32 VehicleSpellIdValue::Calculate()
{
#ifdef MANGOSBOT_TWO
    TransportInfo* transportInfo = bot->GetTransportInfo();
    if (!transportInfo || !transportInfo->IsOnVehicle())
        return 0;

    Unit* vehicle = (Unit*)transportInfo->GetTransport();
    if (!vehicle || !vehicle->IsAlive())
        return 0;

    // do not allow if no spells
    VehicleSeatEntry const* seat = vehicle->GetVehicleInfo()->GetSeatEntry(transportInfo->GetTransportSeat());
    if (!seat || !seat->HasFlag(SEAT_FLAG_CAN_CAST))
        return 0;

    string namepart = qualifier;

    PlayerbotChatHandler handler(bot);
    uint32 extractedSpellId = handler.extractSpellId(namepart);
    if (extractedSpellId)
    {
        const SpellEntry* pSpellInfo = sServerFacade.LookupSpellInfo(extractedSpellId);
        if (pSpellInfo) namepart = pSpellInfo->SpellName[0];
    }

    wstring wnamepart;

    if (!Utf8toWStr(namepart, wnamepart))
        return 0;

    wstrToLower(wnamepart);
    char firstSymbol = tolower(namepart[0]);
    int spellLength = wnamepart.length();

    int loc = bot->GetSession()->GetSessionDbcLocale();

    Creature* creature = static_cast<Creature*>(vehicle);

    for (uint32 x = 0; x < CREATURE_MAX_SPELLS; ++x)
    {
        CharmSpellEntry* cspell = creature->GetCharmInfo()->GetCharmSpell(x);
        if (!cspell)
            continue;

        uint32 spellId = cspell->GetAction();

        if (spellId == 2)
            continue;
        if (IsPassiveSpell(spellId))
            continue;
        else
        {
            const SpellEntry* pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
            if (!pSpellInfo)
                continue;

            char* spellName = pSpellInfo->SpellName[loc];
            if (tolower(spellName[0]) != firstSymbol || strlen(spellName) != spellLength || !Utf8FitTo(spellName, wnamepart))
                continue;

            return spellId;
        }
    }
#endif

    return 0;
}
