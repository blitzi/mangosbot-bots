#include "botpch.h"
#include "../../playerbot.h"
#include "PartyMemberToHeal.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"
#include <playerbot/strategy/values/Stances.h>

using namespace ai;

class IsTargetOfHealingSpell : public SpellEntryPredicate
{
public:
    virtual bool Check(SpellEntry const* spell) {
        for (int i=0; i<3; i++) {
            if (spell->Effect[i] == SPELL_EFFECT_HEAL ||
                spell->Effect[i] == SPELL_EFFECT_HEAL_MAX_HEALTH ||
                spell->Effect[i] == SPELL_EFFECT_HEAL_MECHANICAL)
                return true;
        }
        return false;
    }

};

bool compareByHealth(const Unit *u1, const Unit *u2)
{
    return u1->GetHealthPercent() < u2->GetHealthPercent();
}

Unit* PartyMemberToHeal::Calculate()
{

    IsTargetOfHealingSpell predicate;

    vector<Unit*> needHeals;
    vector<Unit*> tankTargets;

    if (bot->GetSelectionGuid())
    {
        Unit* target = ai->GetUnit(bot->GetSelectionGuid());
        if (target && sServerFacade.IsFriendlyTo(bot, target) && 
#ifdef MANGOS
            target->HealthBelowPct(100))
#endif
#ifdef CMANGOS
            target->GetHealthPercent() < 100)
#endif
            needHeals.push_back(target);
    }

    Group* group = bot->GetGroup();
    if (!group && needHeals.empty())
        return NULL;

    if (group)
    {
        for (GroupReference* gref = group->GetFirstMember(); gref; gref = gref->next())
        {
            Player* player = gref->getSource();
            if (!Check(player) || !sServerFacade.IsAlive(player))
                continue;

            uint8 health = player->GetHealthPercent();
            if ((health < sPlayerbotAIConfig.almostFullHealth && !IsTargetOfSpellCast(player, predicate)) ||
                (health < sPlayerbotAIConfig.criticalHealth))
                needHeals.push_back(player);

            Pet* pet = player->GetPet();
            if (pet && CanHealPet(pet))
            {
                health = pet->GetHealthPercent();
                if (health < sPlayerbotAIConfig.almostFullHealth && !IsTargetOfSpellCast(player, predicate))
                    needHeals.push_back(pet);
            }

            if (health < sPlayerbotAIConfig.almostFullHealth && ai->IsTank(player) && bot->IsInGroup(player, false))
                tankTargets.push_back(player);
        }
    }

    if (needHeals.empty() && tankTargets.empty())
        return NULL;

    if (!tankTargets.empty())
    {
        sort(tankTargets.begin(), tankTargets.end(), compareByHealth);
        return tankTargets[0];
    }

    sort(needHeals.begin(), needHeals.end(), compareByHealth);   
    return needHeals[0];
}

bool PartyMemberToHeal::CanHealPet(Pet* pet)
{
    return MINI_PET != pet->getPetType();
}

bool PartyMemberToHeal::Check(Unit* player)
{
    if (!player)
        return false;

    if (player == bot)
        return false;
        
    if (player->GetMapId() != bot->GetMapId())
        return false;

    Stance* stance = ai->GetAiObjectContext()->GetValue<Stance*>("stance")->Get();

    if(stance && stance->getName() != "near")
    {
        return sServerFacade.GetDistance2d(bot, player) <= ai->GetRange("spell");
    }
    else
    {
        return sServerFacade.GetDistance2d(bot, player) < (player->IsPlayer() && ai->IsTank((Player*)player)) ? 50.0f : 40.0f;
    }        
}

Unit* PartyMemberToProtect::Calculate()
{
    Group* group = bot->GetGroup();
    if (!group)
        return NULL;

    vector<Unit*> needProtect;

    list<ObjectGuid> attackers = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("attackers")->Get();
    for (list<ObjectGuid>::iterator i = attackers.begin(); i != attackers.end(); ++i)
    {
        Unit* unit = ai->GetUnit(*i);
        if (!unit)
            continue;

        bool isRanged = false;

        if (unit->AI())
        {
            if (unit->AI()->IsRangedUnit())
                isRanged = true;
        }

        Unit* pVictim = unit->GetVictim();
        if (!pVictim || !pVictim->IsPlayer())
            continue;

        if (pVictim == bot)
            continue;

        float attackDistance = isRanged ? 30.0f : 10.0f;
        if (sServerFacade.GetDistance2d(pVictim, unit) > attackDistance)
            continue;

        if (ai->IsTank((Player*)pVictim) && pVictim->GetHealthPercent() > 10)
            continue;
        else if (pVictim->GetHealthPercent() > 30)
            continue;

        if (find(needProtect.begin(), needProtect.end(), pVictim) == needProtect.end())
            needProtect.push_back(pVictim);
    }

    if (needProtect.empty())
        return NULL;

    sort(needProtect.begin(), needProtect.end(), compareByHealth);

    return needProtect[0];
}



Unit* PartyMemberToCancelHeal::Calculate()
{
    IsTargetOfHealingSpell predicate;
    vector<Unit*> needToCancelHeals;    

    Group* group = bot->GetGroup();
    if (!group)
        return NULL;

    if (group)
    {
        for (GroupReference* gref = group->GetFirstMember(); gref; gref = gref->next())
        {
            Player* player = gref->getSource();

            if (!sServerFacade.IsAlive(player))
                continue;

            uint8 health = player->GetHealthPercent();
            if (health > sPlayerbotAIConfig.almostFullHealth && IsTargetOfMySpellCast(player, predicate))
                return player;
        }
    }

    return NULL;
}