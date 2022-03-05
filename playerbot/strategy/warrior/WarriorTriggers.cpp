#include "botpch.h"
#include "../../playerbot.h"
#include "WarriorTriggers.h"
#include "WarriorActions.h"

using namespace ai;

bool BloodrageBuffTrigger::IsActive()
{
    return AI_VALUE2(float, "health", "self target") >= sPlayerbotAIConfig.mediumHealth &&
        AI_VALUE2(uint32, "rage", "self target") < 20;
}