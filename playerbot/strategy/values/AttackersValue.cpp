#include "botpch.h"
#include "../../playerbot.h"
#include "AttackersValue.h"

#include "../../ServerFacade.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include <algorithm>
#include "RtiTargetValue.h"

using namespace ai;
using namespace MaNGOS;

set<int> focusTargets = { 17096 /*astral-flare*/ };

list<ObjectGuid> AttackersValue::Calculate()
{
    set<Unit*> targets;

    list<ObjectGuid> result;

    AddAttackersOf(bot, targets);

    if(bot->GetPet())
        AddAttackersOf(bot->GetPet(), targets);


    Group* group = bot->GetGroup();
    if (group)
        AddAttackersOf(group, targets);

    RemoveNonThreating(targets);

    //tanks split over all targets
    if (ai->IsTank(ai->GetBot()) && targets.size() > 1)
        RemoveAlreadyTankedTargets(targets, bot);

    //if there is an elite mob between all the other mobs, tanks should focus them
    if (ai->IsTank(ai->GetBot()) && ListContainsElite(targets))
        RemoveNonEliteTargets(targets);

    //if there are totems between all the other targets, the group should focus them
    /*if (!ai->IsTank(ai->GetBot()) && ListContainsTotem(targets))
        RemoveNonTotemTargets(targets);*/

    if (!ai->IsTank(ai->GetBot()) && ListContainsFocusTarget(targets))
        RemoveNonFocusTargets(targets);

    if(ListContainsRti(targets))
        RemoveNonRtiTargets(targets);
    
	for (set<Unit*>::iterator i = targets.begin(); i != targets.end(); i++)
		result.push_back((*i)->GetObjectGuid());

    if (bot->duel && bot->duel->opponent)
        result.push_back(bot->duel->opponent->GetObjectGuid());

	return result;
}

void AttackersValue::AddAttackersOf(Group* group, set<Unit*>& targets)
{
    Group::MemberSlotList const& groupSlot = group->GetMemberSlots();
    for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
    {
        Player *member = sObjectMgr.GetPlayer(itr->guid);
        if (!member || !sServerFacade.IsAlive(member) || member == bot)
            continue;

        AddAttackersOf(member, targets);
    }
}

struct AddGuardiansHelper
{
    explicit AddGuardiansHelper(list<Unit*> &units) : units(units) {}
    void operator()(Unit* target) const
    {
        units.push_back(target);
    }

    list<Unit*> &units;
};

void AttackersValue::AddAttackersOf(Player* player, set<Unit*>& targets)
{
    if (player->IsBeingTeleported())
        return;

	list<Unit*> units;
	AnyEnemyInObjectRangeCheck u_check(player, sPlayerbotAIConfig.sightDistance);
    UnitListSearcher<AnyEnemyInObjectRangeCheck> searcher(units, u_check);
    Cell::VisitAllObjects(player, searcher, sPlayerbotAIConfig.sightDistance);
	for (list<Unit*>::iterator i = units.begin(); i != units.end(); i++)
    {
        Unit* unit = *i;
		if (!player->GetGroup())
		{
#ifdef CMANGOS
			if (!unit->getThreatManager().getThreat(player) && (!unit->getThreatManager().getCurrentVictim() || unit->getThreatManager().getCurrentVictim()->getTarget() != player))
#endif
#ifdef MANGOS
			if (!unit->GetThreatManager().getThreat(player))
#endif
				continue;
		}
        
        targets.insert(unit);
        unit->CallForAllControlledUnits(AddGuardiansHelper(units), CONTROLLED_PET | CONTROLLED_GUARDIANS | CONTROLLED_CHARM | CONTROLLED_MINIPET | CONTROLLED_TOTEMS);        
    }
}

void AttackersValue::AddAttackersOf(Unit* pet, set<Unit*>& targets)
{
    list<Unit*> units;
    AnyEnemyInObjectRangeCheck u_check(pet, sPlayerbotAIConfig.sightDistance);
    UnitListSearcher<AnyEnemyInObjectRangeCheck> searcher(units, u_check);
    Cell::VisitAllObjects(pet, searcher, sPlayerbotAIConfig.sightDistance);
    for (list<Unit*>::iterator i = units.begin(); i != units.end(); i++)
    {
        Unit* unit = *i;
#ifdef CMANGOS
            if (!unit->getThreatManager().getThreat(pet) && (!unit->getThreatManager().getCurrentVictim() || unit->getThreatManager().getCurrentVictim()->getTarget() != pet))
#endif
#ifdef MANGOS
            if (!unit->GetThreatManager().getThreat(player))
#endif
                    continue;

        targets.insert(unit);
        unit->CallForAllControlledUnits(AddGuardiansHelper(units), CONTROLLED_PET | CONTROLLED_GUARDIANS | CONTROLLED_CHARM | CONTROLLED_MINIPET | CONTROLLED_TOTEMS);                
    }
}

void AttackersValue::RemoveNonThreating(set<Unit*>& targets)
{
    for(set<Unit *>::iterator tIter = targets.begin(); tIter != targets.end();)
    {
        Unit* unit = *tIter;
        if (!IsValidTarget(unit, bot))
        {
            set<Unit *>::iterator tIter2 = tIter;
            ++tIter;
            targets.erase(tIter2);
        }
        else
            ++tIter;
    }
}

void AttackersValue::RemoveNonEliteTargets(set<Unit*>& targets)
{
    for (set<Unit*>::iterator tIter = targets.begin(); tIter != targets.end();)
    {
        Unit* unit = *tIter;
        Creature* c = dynamic_cast<Creature*>(unit);
        if (c && !c->IsElite())
        {
            set<Unit*>::iterator tIter2 = tIter;
            ++tIter;
            targets.erase(tIter2);
        }
        else
            ++tIter;
    }
}


void AttackersValue::RemoveAlreadyTankedTargets(set<Unit*>& targets, Player* bot)
{
    PlayerbotAI* ai = bot->GetPlayerbotAI();
    Group* group = bot->GetGroup();

    if (group)
    {
        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            Player* p = ref->getSource();

            if (p == bot)
                continue;
        
            if (ai->IsTank(p) && p->IsAlive())        
            {
                Unit* target = p->GetTarget();

                if (target)
                    targets.erase(target);
            }
        }
    }
}

void AttackersValue::RemoveNonTotemTargets(set<Unit*>& targets)
{
    for (set<Unit*>::iterator tIter = targets.begin(); tIter != targets.end();)
    {
        Unit* unit = *tIter;
        Creature* c = dynamic_cast<Creature*>(unit);
        if (c && !c->IsTotem())
        {
            set<Unit*>::iterator tIter2 = tIter;
            ++tIter;
            targets.erase(tIter2);
        }
        else
            ++tIter;
    }
}


void AttackersValue::RemoveNonFocusTargets(set<Unit*>& targets)
{
    for (set<Unit*>::iterator tIter = targets.begin(); tIter != targets.end();)
    {
        Unit* unit = *tIter;
        Creature* c = dynamic_cast<Creature*>(unit);

        if (c)
        {
            CreatureData const* data = sObjectMgr.GetCreatureData(c->GetDbGuid());

            if (data && focusTargets.find(data->id) == focusTargets.end())
            {
                set<Unit*>::iterator tIter2 = tIter;
                ++tIter;
                targets.erase(tIter2);
            }
            else
                ++tIter;
        }
        else
        {
            ++tIter;
        }
    }
}

void AttackersValue::RemoveNonRtiTargets(set<Unit*>& targets)
{
    for (set<Unit*>::iterator tIter = targets.begin(); tIter != targets.end();)
    {
        Unit* unit = *tIter;
        if (!IsRti(unit, ai->GetBot()))
        {
            set<Unit*>::iterator tIter2 = tIter;
            ++tIter;
            targets.erase(tIter2);
        }
        else
            ++tIter;
    }
}



bool AttackersValue::ListContainsElite(set<Unit*>& targets) const
{
    for (auto t : targets)
    {
        Creature* c = dynamic_cast<Creature*>(t);

        if (c && c->IsElite())
            return true;
    }

    return false;
}


bool AttackersValue::ListContainsFocusTarget(set<Unit*>& targets) const
{
    for (auto t : targets)
    {
        Creature* c = dynamic_cast<Creature*>(t);

        if (c)
        {
            CreatureData const* data = sObjectMgr.GetCreatureData(c->GetDbGuid());
            if (data && focusTargets.find(data->id) != focusTargets.end())
                return true;
        }
    }

    return false;
}

bool AttackersValue::ListContainsTotem(set<Unit*>& targets) const
{
    for (auto t : targets)
    {
        Creature* c = dynamic_cast<Creature*>(t);

        if (c && c->IsTotem())
            return true;
    }

    return false;
}

bool AttackersValue::ListContainsRti(set<Unit*>& targets) const
{
    for (auto t : targets)
    {
        if (IsRti(t, bot))
            return true;
    }

    return false;
}

bool AttackersValue::IsPossibleTarget(Unit *attacker, Player *bot)
{
    Creature *c = dynamic_cast<Creature*>(attacker);
    Group* group = bot->GetGroup();

    bool basicConditions = attacker &&
        attacker->IsInWorld() &&
        attacker->GetMapId() == bot->GetMapId() &&
        !sServerFacade.UnitIsDead(attacker) &&
#ifdef MANGOSBOT_ONE
        !attacker->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SPAWNING) &&
#else
        !attacker->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) &&       
#endif
        !attacker->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE) &&
        attacker->IsVisibleForOrDetect(bot, attacker, false) &&
        !sServerFacade.IsFriendlyTo(attacker, bot) &&
        bot->IsWithinDistInMap(attacker, sPlayerbotAIConfig.sightDistance) &&
        !(sPlayerbotAIConfig.IsInPvpProhibitedZone(attacker->GetAreaId()) && (attacker->GetObjectGuid().IsPlayer() || attacker->GetObjectGuid().IsPet())) &&
        (!c || !c->GetCombatManager().IsInEvadeMode());

    bool rti = IsRti(attacker, bot);

    if (rti)
        return basicConditions;

    if (!group && bot->GetPet() && bot->GetPet()->IsAttackedBy(attacker))
        return basicConditions;

    if (c)
    {
        CreatureData const* data = sObjectMgr.GetCreatureData(c->GetDbGuid());
        if (data && focusTargets.find(data->id) != focusTargets.end())
            return basicConditions;
    }


    PlayerbotAI* ai = bot->GetPlayerbotAI();
    	
    bool isRaid = sMapStore.LookupEntry(bot->GetMapId())->IsRaid();
	bool tankHasAggro = false;
	bool targetIsNonElite = isRaid ? false : (!c || !c->IsElite());//normal mobs in raids count as "elites"
    bool targetIsAlmostDead = false;//!c || c->GetHealthPercent() < 50;
	float highestThreat = 0;
	float myThreat = 0;  
	float tankThreat = 0;
	bool waitForTankAggro = true;    
	bool iAmTank = ai->IsTank(ai->GetBot());
    bool carefulTanking = ai->HasStrategy("careful tanking", BOT_STATE_COMBAT);

	if (attacker)
	{
		highestThreat = attacker->getThreatManager().GetHighestThreat();
		myThreat = attacker->getThreatManager().getThreat(bot);
		float myMaxDamage = bot->GetFloatValue(UNIT_FIELD_MAXDAMAGE) * (carefulTanking ? 3.0f : 1.5f);
		uint32 maxSpellDmg = 0;

		for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
		{
			uint32 spellDmg = bot->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i) * (carefulTanking ? 3.0f : 1.5f);

			if (spellDmg > maxSpellDmg)
				maxSpellDmg = spellDmg;
		}

		myMaxDamage += maxSpellDmg;//fantasy aggro value for testing
        
		if (highestThreat > 0)
		{			
			float myAggroInPct = ((100.0f / highestThreat) * (myThreat + myMaxDamage));			

            if(carefulTanking)
			    waitForTankAggro = myAggroInPct > 90;
            else
			    waitForTankAggro = c->GetHealthPercent() > 90 && myAggroInPct > 90; 
		}
	}

    int tanks = 0;

	if (group)
	{

		for (GroupReference *ref = group->GetFirstMember(); ref; ref = ref->next())
		{
			Player* p = ref->getSource();

 			if (ai->GetBot()->IsPlayer() && ai->IsTank(p) && p->IsAlive() && 
                !(ai->GetBot()->IsStunned() || ai->GetBot()->isFeared() || ai->GetBot()->IsPolymorphed()))
			{
                tanks++;
			}
		}
	}

    bool groupHasTank = tanks > 0;

    bool leaderHasThreat = false;
    if (attacker && bot->GetGroup() && ai->GetMaster())
        leaderHasThreat = attacker->getThreatManager().getThreat(ai->GetMaster());

    bool isMemberBotGroup = false;
    if (bot->GetGroup() && ai->GetMaster() && ai->GetMaster()->GetPlayerbotAI() && !ai->GetMaster()->GetPlayerbotAI()->IsRealPlayer())
        isMemberBotGroup = true;

    return basicConditions &&
        !(attacker->IsStunned() && ai->HasAura("shackle undead", attacker)) &&
        !((attacker->IsPolymorphed() ||
        bot->GetPlayerbotAI()->HasAura("sap", attacker) ||
        sServerFacade.IsCharmed(attacker) ||
        sServerFacade.IsFeared(attacker)) && !rti) &&
        //!sServerFacade.IsInRoots(attacker) &&                
        !(attacker->GetCreatureType() == CREATURE_TYPE_CRITTER) &&        
        (!groupHasTank || ((groupHasTank && !waitForTankAggro) || iAmTank || targetIsNonElite || targetIsAlmostDead)) &&
        (!c ||
            (
#ifdef CMANGOS          
                (!isMemberBotGroup && ai->HasStrategy("attack tagged", BOT_STATE_NON_COMBAT)) || leaderHasThreat ||
                
                (c->IsTappedBy(bot) || (!c->HasLootRecipient() && (!c->GetVictim() || c->GetVictim() && ((bot->IsInGroup(c->GetVictim())) || (ai->GetMaster() &&
                c->GetVictim() == ai->GetMaster())))))
#endif
#ifndef MANGOSBOT_TWO
#ifdef MANGOS
                !attacker->HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_TAPPED) || bot->IsTappedByMeOrMyGroup(c)
#endif
#endif
                 )            
            );
}

bool AttackersValue::IsRti(Unit* enemy, Player* bot)
{
    bool isRti = false;

    if (enemy && bot->GetGroup())
    {
        string rtiStr = bot->GetPlayerbotAI()->GetAiObjectContext()->GetValue<string>("rti")->Get();
        int index = RtiTargetValue::GetRtiIndex(rtiStr);

        isRti = bot->GetGroup()->GetTargetIcon(index) == enemy->GetObjectGuid();
    }

    return isRti;
}

bool AttackersValue::IsValidTarget(Unit *attacker, Player *bot)
{
	bool possibleTarget = IsPossibleTarget(attacker, bot);

    return possibleTarget &&
        (sServerFacade.GetThreatManager(attacker).getCurrentVictim() ||
            attacker->GetGuidValue(UNIT_FIELD_TARGET) || attacker->GetObjectGuid().IsPlayer() || attacker->GetCreatureType() == CREATURE_TYPE_TOTEM ||
            attacker->GetObjectGuid() == bot->GetPlayerbotAI()->GetAiObjectContext()->GetValue<ObjectGuid>("pull target")->Get());
}

bool PossibleAddsValue::Calculate()
{
    PlayerbotAI *ai = bot->GetPlayerbotAI();
    list<ObjectGuid> possible = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("possible targets no los")->Get();
    list<ObjectGuid> attackers = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("attackers")->Get();

    for (list<ObjectGuid>::iterator i = possible.begin(); i != possible.end(); ++i)
    {
        ObjectGuid guid = *i;
        if (find(attackers.begin(), attackers.end(), guid) != attackers.end()) continue;

        Unit* add = ai->GetUnit(guid);
        if (add && !add->GetGuidValue(UNIT_FIELD_TARGET) && !sServerFacade.GetThreatManager(add).getCurrentVictim() && sServerFacade.IsHostileTo(add, bot))
        {
            for (list<ObjectGuid>::iterator j = attackers.begin(); j != attackers.end(); ++j)
            {
                Unit* attacker = ai->GetUnit(*j);
                if (!attacker) continue;

                float dist = sServerFacade.GetDistance2d(attacker, add);
                if (sServerFacade.IsDistanceLessOrEqualThan(dist, sPlayerbotAIConfig.aoeRadius * 1.5f)) continue;
                if (sServerFacade.IsDistanceLessOrEqualThan(dist, sPlayerbotAIConfig.aggroDistance)) return true;
            }
        }
    }
    return false;
}
