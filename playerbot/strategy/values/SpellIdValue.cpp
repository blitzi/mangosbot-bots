#include "botpch.h"
#include "../../playerbot.h"
#include "SpellIdValue.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"
#include <regex>

using namespace ai;

SpellIdValue::SpellIdValue(PlayerbotAI* ai) :
        CalculatedValue<uint32>(ai, "spell id")
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

    if (Utf8FitTo("consecration rank 1", wnamepart))
    {
        int x = 0;
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
