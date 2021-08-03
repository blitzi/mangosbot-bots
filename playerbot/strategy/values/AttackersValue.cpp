#include "botpch.h"
#include "../../playerbot.h"
#include "AttackersValue.h"

#include "../../ServerFacade.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

using namespace ai;
using namespace MaNGOS;

list<ObjectGuid> AttackersValue::Calculate()
{
    set<Unit*> targets;

    list<ObjectGuid> result;

    AddAttackersOf(bot, targets);

    if (!ai->AllowActive(ALL_ACTIVITY))
        return result;

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
    if (!ai->IsTank(ai->GetBot()) && ListContainsTotem(targets))
        RemoveNonTotemTargets(targets);
    
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
		if (!player->GetGroup())
		{
			Unit* unit = *i;
#ifdef CMANGOS
			if (!unit->getThreatManager().getThreat(player) && (!unit->getThreatManager().getCurrentVictim() || unit->getThreatManager().getCurrentVictim()->getTarget() != player))
#endif
#ifdef MANGOS
			if (!unit->GetThreatManager().getThreat(player))
#endif
				continue;
		}
        {
            Unit* unit = *i;
            targets.insert(unit);
            unit->CallForAllControlledUnits(AddGuardiansHelper(units), CONTROLLED_PET | CONTROLLED_GUARDIANS | CONTROLLED_CHARM | CONTROLLED_MINIPET | CONTROLLED_TOTEMS);
        }
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

bool AttackersValue::IsPossibleTarget(Unit *attacker, Player *bot)
{
    Creature *c = dynamic_cast<Creature*>(attacker);

    bool rti = false;
    if (attacker && bot->GetGroup())
        rti = bot->GetGroup()->GetTargetIcon(7) == attacker->GetObjectGuid();

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

	if (attacker)
	{
		highestThreat = attacker->getThreatManager().GetHighestThreat();
		myThreat = attacker->getThreatManager().getThreat(bot);
		float myMaxDamage = bot->GetFloatValue(UNIT_FIELD_MAXDAMAGE);
		uint32 maxSpellDmg = 0;

		for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
		{
			uint32 spellDmg = bot->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i) * 1.5f;

			if (spellDmg > maxSpellDmg)
				maxSpellDmg = spellDmg;
		}

		myMaxDamage += maxSpellDmg;//fantasy aggro value for testing

		if (highestThreat > 0)
		{			
			float myAggroInPct = ((100.0f / highestThreat) * (myThreat + myMaxDamage));			
			waitForTankAggro = c->GetHealthPercent() > 90 && myAggroInPct > 90;
		}
	}

	Group* group = bot->GetGroup();

    int tanks = 0;

	if (group)
	{

		for (GroupReference *ref = group->GetFirstMember(); ref; ref = ref->next())
		{
			Player* p = ref->getSource();

 			if (ai->IsTank(p) && p->IsAlive())
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

    return attacker &&
        attacker->IsInWorld() &&
        attacker->GetMapId() == bot->GetMapId() &&
        !sServerFacade.UnitIsDead(attacker) &&
        !attacker->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) &&
        !attacker->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE) &&        
        attacker->IsVisibleForOrDetect(bot, attacker, false) &&
        !((attacker->IsPolymorphed() ||
        bot->GetPlayerbotAI()->HasAura("sap", attacker) ||
        sServerFacade.IsCharmed(attacker) ||
        sServerFacade.IsFeared(attacker)) && !rti) &&
        //!sServerFacade.IsInRoots(attacker) &&
        !sServerFacade.IsFriendlyTo(attacker, bot) &&
        bot->IsWithinDistInMap(attacker, sPlayerbotAIConfig.sightDistance) &&
        !(attacker->GetCreatureType() == CREATURE_TYPE_CRITTER) &&
        !(sPlayerbotAIConfig.IsInPvpProhibitedZone(attacker->GetAreaId()) && (attacker->GetObjectGuid().IsPlayer() || attacker->GetObjectGuid().IsPet())) &&
        (!groupHasTank || ((groupHasTank && !waitForTankAggro) || iAmTank || targetIsNonElite || targetIsAlmostDead)) &&
        (!c ||
            (
                !c->GetCombatManager().IsInEvadeMode() &&            
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
