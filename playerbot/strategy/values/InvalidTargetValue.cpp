#include "botpch.h"
#include "../../playerbot.h"
#include "InvalidTargetValue.h"
#include "AttackersValue.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"

using namespace ai;

bool InvalidTargetValue::Calculate()
{
    Unit* target = AI_VALUE(Unit*, qualifier);
    Unit* enemy = AI_VALUE(Unit*, "enemy player target");

    if (target && enemy && target == enemy)
        return !sServerFacade.IsAlive(target);

    if (target && qualifier == "current target")
    {
        return !AttackersValue::IsPossibleTarget(target, bot);
    }

    return false;
}
