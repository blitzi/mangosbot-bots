#include "botpch.h"
#include "../../playerbot.h"
#include "PartyMemberValue.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"

using namespace ai;
using namespace std;

Unit* PartyMemberValue::FindPartyMember(list<Player*>* party, FindPlayerPredicate &predicate)
{
    for (list<Player*>::iterator i = party->begin(); i != party->end(); ++i)
    {
        Player* player = *i;

        if (!player)
            continue;

        if (Check(player) && predicate.Check(player))
            return player;

        Pet* pet = player->GetPet();
        if (pet && Check(pet) && predicate.Check(pet))
            return pet;
    }

    return NULL;
}

Unit* PartyMemberValue::FindPartyMember(FindPlayerPredicate &predicate, bool ignoreOutOfGroup)
{
    Player* master = GetMaster();
    list<ObjectGuid> nearestPlayers = AI_VALUE(list<ObjectGuid>, "nearest friendly players");

    list<ObjectGuid> nearestGroupPlayers;

    Group* group = bot->GetGroup();
    if (group)
    {
        for (GroupReference *ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            if (ref->getSource() != bot)
            {
                nearestGroupPlayers.push_back(ref->getSource()->GetObjectGuid());
            }
        }
    }
    
    if (!ignoreOutOfGroup && !nearestPlayers.empty())
        nearestGroupPlayers.insert(nearestGroupPlayers.end(), nearestPlayers.begin(), nearestPlayers.end());

    nearestPlayers = nearestGroupPlayers;
    nearestPlayers.sort();
    nearestPlayers.unique();

    list<Player*> healers, tanks, others, masters;
    if (master) masters.push_back(master);
    for (list<ObjectGuid>::iterator i = nearestPlayers.begin(); i != nearestPlayers.end(); ++i)
    {
        Player* player = dynamic_cast<Player*>(ai->GetUnit(*i));
        if (!player || player == bot) continue;

        if (ai->IsHeal(player))
            healers.push_back(player);
        else if (ai->IsTank(player))
            tanks.push_back(player);
        else if (player != master)
            others.push_back(player);
    }

    list<list<Player*>* > lists;
    lists.push_back(&healers);
    lists.push_back(&tanks);
    lists.push_back(&masters);
    lists.push_back(&others);

    for (list<list<Player*>* >::iterator i = lists.begin(); i != lists.end(); ++i)
    {
        list<Player*>* party = *i;
        Unit* target = FindPartyMember(party, predicate);
        if (target)
            return target;
    }

    return NULL;
}

bool PartyMemberValue::Check(Unit* player)
{
    return player && player != bot && player->GetMapId() == bot->GetMapId() &&
        bot->IsWithinDistInMap(player, sPlayerbotAIConfig.sightDistance, false);
}

bool PartyMemberValue::IsTargetOfSpellCast(Player* target, SpellEntryPredicate &predicate)
{
    if (!target)
        return false;

    list<ObjectGuid> nearestPlayers = AI_VALUE(list<ObjectGuid>, "nearest friendly players");
    ObjectGuid targetGuid = target->GetObjectGuid();
    ObjectGuid corpseGuid = target->GetCorpse() ? target->GetCorpse()->GetObjectGuid() : ObjectGuid();

    for (list<ObjectGuid>::iterator i = nearestPlayers.begin(); i != nearestPlayers.end(); ++i)
    {
        Player* player = dynamic_cast<Player*>(ai->GetUnit(*i));
        if (!player || player == bot)
            continue;

        if (player->IsNonMeleeSpellCasted(false))
        {
            for (int type = CURRENT_GENERIC_SPELL; type < CURRENT_MAX_SPELL; type++) {
                Spell* spell = player->GetCurrentSpell((CurrentSpellTypes)type);
                if (spell && predicate.Check(spell->m_spellInfo)) {
                    ObjectGuid unitTarget = spell->m_targets.getUnitTargetGuid();
                    if (unitTarget == targetGuid)
                        return true;

                    ObjectGuid corpseTarget = spell->m_targets.getCorpseTargetGuid();
                    if (corpseGuid && corpseTarget == corpseGuid)
                        return true;
                }
            }
        }
    }

    return false;
}

bool PartyMemberValue::IsTargetOfMySpellCast(Player* target, SpellEntryPredicate& predicate)
{
    if (!target)
        return false;

    ObjectGuid targetGuid = target->GetObjectGuid();
    ObjectGuid corpseGuid = target->GetCorpse() ? target->GetCorpse()->GetObjectGuid() : ObjectGuid();

    if (bot->IsNonMeleeSpellCasted(false))
    {
        if (targetGuid == bot->GetObjectGuid())
            return IsTargetOfSpellCast(bot, predicate);

        for (int type = CURRENT_GENERIC_SPELL; type < CURRENT_MAX_SPELL; type++) {
            Spell* spell = bot->GetCurrentSpell((CurrentSpellTypes)type);
            if (spell && predicate.Check(spell->m_spellInfo)) {
                ObjectGuid unitTarget = spell->m_targets.getUnitTargetGuid();
                if (unitTarget == targetGuid)
                    return true;

                ObjectGuid corpseTarget = spell->m_targets.getCorpseTargetGuid();
                if (corpseGuid && corpseTarget == corpseGuid)
                    return true;
            }
        }
    }  

    return false;
}
