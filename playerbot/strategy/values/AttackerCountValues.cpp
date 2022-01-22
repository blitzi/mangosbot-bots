#include "botpch.h"
#include "../../playerbot.h"
#include "AttackerCountValues.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"

using namespace ai;

uint8 MyAttackerCountValue::Calculate()
{
    uint8 size = 0;

    if (bot->GetPet())
        size = bot->GetPet()->getAttackers().size();

    size += bot->getAttackers().size();

    return size;
}

bool HasAggroValue::Calculate()
{
    Unit* target = GetTarget();
    if (!target)
        return true;

    HostileReference *ref = sServerFacade.GetHostileRefManager(bot).getFirst();
    if (!ref)
        return true; // simulate as target is not atacking anybody yet

    while( ref )
    {
        ThreatManager *threatManager = ref->getSource();
        Unit *attacker = threatManager->getOwner();
#ifdef CMANGOS
        Unit *victim = attacker->GetVictim();
#endif
#ifdef MANGOS
        Unit *victim = attacker->getVictim();
#endif
        if (victim == bot && target == attacker)
            return true;
        ref = ref->next();
    }

    ref = sServerFacade.GetThreatManager(target).getCurrentVictim();
    if (ref)
    {
        Unit* victim = ref->getTarget();
        if (victim)
        {
            Player* pl = dynamic_cast<Player*>(victim);
            if (pl && ai->IsTank(pl)) return true;
        }
    }

    return false;
}

uint8 AttackerCountValue::Calculate()
{
    int count = 0;
    float range = sPlayerbotAIConfig.sightDistance;

    list<ObjectGuid> attackers = context->GetValue<list<ObjectGuid> >("attackers")->Get();
    for (list<ObjectGuid>::iterator i = attackers.begin(); i != attackers.end(); i++)
    {
        Unit* unit = ai->GetUnit(*i);
        if (!unit || !sServerFacade.IsAlive(unit))
            continue;

        float distance = bot->GetDistance(unit);
        if (distance <= range)
            count++;
    }

    return count;
}

float BalanceThreatValue::Calculate()
{
    float playerValue = 0,
        attackerValue = 0;

    Group* group = bot->GetGroup();
    if (group)
    {
        Group::MemberSlotList const& groupSlot = group->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
        {
            Player *player = sObjectMgr.GetPlayer(itr->guid);
            if(!player || !sServerFacade.IsAlive(player))
                continue;

            playerValue += player->GetHealth();
        }
    }

    if (playerValue <= 0)
        return 0;

    list<ObjectGuid> v = context->GetValue<list<ObjectGuid>>("attackers")->Get();

    for (list<ObjectGuid>::iterator i = v.begin(); i!=v.end(); i++)
    {
        Creature* creature = ai->GetCreature((*i));
        if (!creature || !sServerFacade.IsAlive(creature))
            continue;        

        attackerValue += creature->GetHealth();
    }

    return attackerValue * 100.0f / playerValue;
}

