#include "botpch.h"
#include "../../playerbot.h"
#include "PossibleRpgTargetsValue.h"

#include "../../ServerFacade.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "NearestUnitsValue.h"
#include "../../Travelmgr.h"

using namespace ai;
using namespace MaNGOS;

vector<uint32> PossibleRpgTargetsValue::allowedNpcFlags;

PossibleRpgTargetsValue::PossibleRpgTargetsValue(PlayerbotAI* ai, float range) :
        NearestUnitsValue(ai, "possible rpg targets", range, true)
{
    if (!allowedNpcFlags.size())
    {
        allowedNpcFlags.push_back(UNIT_NPC_FLAG_INNKEEPER);
        allowedNpcFlags.push_back(UNIT_NPC_FLAG_VENDOR);
        allowedNpcFlags.push_back(UNIT_NPC_FLAG_REPAIR);
    }
}

void PossibleRpgTargetsValue::FindUnits(list<Unit*> &targets)
{
    AnyUnitInObjectRangeCheck u_check(bot, range);
    UnitListSearcher<AnyUnitInObjectRangeCheck> searcher(targets, u_check);
    Cell::VisitAllObjects(bot, searcher, range);
}

bool PossibleRpgTargetsValue::AcceptUnit(Unit* unit)
{
    if (sServerFacade.IsHostileTo(unit, bot) || dynamic_cast<Player*>(unit))
        return false;

	if (unit && unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER))
		return false;

    TravelTarget* travelTarget = context->GetValue<TravelTarget*>("travel target")->Get();

    if (travelTarget->getDestination() && travelTarget->getDestination()->getEntry() == unit->GetEntry())
        return true;

    return false;
}
