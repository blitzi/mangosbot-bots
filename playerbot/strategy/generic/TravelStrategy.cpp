#include "botpch.h"
#include "../../playerbot.h"
#include "TravelStrategy.h"

using namespace ai;

TravelStrategy::TravelStrategy(PlayerbotAI* ai) : Strategy(ai)
{
}

void TravelStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
}
