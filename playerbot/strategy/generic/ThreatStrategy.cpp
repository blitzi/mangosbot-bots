#include "botpch.h"
#include "../../playerbot.h"
#include "ThreatStrategy.h"
#include "../../PlayerbotAIConfig.h"
#include "../actions/GenericSpellActions.h"

using namespace ai;

float ThreatMultiplier::GetValue(Action* action)
{
    return 1.0f;
}

void ThreatStrategy::InitMultipliers(std::list<Multiplier*> &multipliers)
{
    multipliers.push_back(new ThreatMultiplier(ai));
}
