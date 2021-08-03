#include "botpch.h"
#include "../../playerbot.h"
#include "../../ServerFacade.h"
#include "PartyMemberWithAggroValue.h"

#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

using namespace MaNGOS;
using namespace ai;

class PlayerWithAggroPredicate : public FindPlayerPredicate, public PlayerbotAIAware
{
public:
    PlayerWithAggroPredicate(PlayerbotAI* ai) :
        PlayerbotAIAware(ai), FindPlayerPredicate() {}

public:
    virtual bool Check(Unit* player)
    {
        if (!sServerFacade.IsAlive(player))
            return false;

        Pet* pet = dynamic_cast<Pet*>(player);
        if (pet && (pet->getPetType() == MINI_PET || pet->getPetType() == SUMMON_PET))
            return false;

        Player* p = dynamic_cast<Player*>(player);
        if (p && ai->IsTank(p))
            return false;

        list<Unit*> units;
        AnyEnemyInObjectRangeCheck u_check(player, sPlayerbotAIConfig.sightDistance);
        UnitListSearcher<AnyEnemyInObjectRangeCheck> searcher(units, u_check);
        Cell::VisitAllObjects(player, searcher, sPlayerbotAIConfig.sightDistance);

        for (list<Unit*>::iterator i = units.begin(); i != units.end(); i++)
        {
            Unit* unit = *i;

            if (unit->getThreatManager().getThreat(player) > 0 && unit->getThreatManager().getThreat(player) >= unit->getThreatManager().GetHighestThreat())
            {
                return true;               
            }            
        }

        return false;
    }
};

Unit* PartyMemberWithAggroValue::Calculate()
{
    PlayerWithAggroPredicate predicate(ai);
    return FindPartyMember(predicate);
}
