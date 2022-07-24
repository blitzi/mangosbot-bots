#include "botpch.h"
#include "../../playerbot.h"
#include "WarlockMultipliers.h"
#include "GenericWarlockStrategy.h"

using namespace ai;

NextAction** GenericWarlockStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("shadow bolt", 5.0f), NULL);
}

void GenericWarlockStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    CombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "high threat",
        NextAction::array(0, new NextAction("soulshatter", ACTION_EMERGENCY +5), NULL)));

    triggers.push_back(new TriggerNode(
        "medium mana",
        NextAction::array(0, new NextAction("life tap", ACTION_EMERGENCY), NULL)));

    triggers.push_back(new TriggerNode(
        "low health",
        NextAction::array(0, new NextAction("drain life", ACTION_EMERGENCY + 1), NULL)));
}

void WarlockBoostStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
#ifndef MANGOSBOT_TWO
    triggers.push_back(new TriggerNode(
        "amplify curse",
        NextAction::array(0, new NextAction("amplify curse", 41.0f), NULL)));
#endif
}

void WarlockCcStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "banish",
        NextAction::array(0, new NextAction("banish on cc", 32.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "fear",
        NextAction::array(0, new NextAction("fear on cc", 33.0f), NULL)));
}
