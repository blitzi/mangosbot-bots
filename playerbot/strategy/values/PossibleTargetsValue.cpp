#include "botpch.h"
#include "../../playerbot.h"
#include "PossibleTargetsValue.h"
#include "AttackersValue.h"

#include "../../ServerFacade.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

using namespace ai;
using namespace MaNGOS;

void PossibleTargetsValue::FindUnits(list<Unit*> &targets)
{
    MaNGOS::AnyEnemyInObjectRangeCheck u_check(bot, range);
    MaNGOS::UnitListSearcher<MaNGOS::AnyEnemyInObjectRangeCheck> searcher(targets, u_check);
    Cell::VisitAllObjects(bot, searcher, range);
}

bool PossibleTargetsValue::AcceptUnit(Unit* unit)
{
    return AttackersValue::IsPossibleTarget(unit, bot);
}
