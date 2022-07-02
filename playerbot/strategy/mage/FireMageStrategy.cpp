#include "botpch.h"
#include "../../playerbot.h"
#include "MageMultipliers.h"
#include "FireMageStrategy.h"

using namespace ai;

void FireMageStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericMageStrategy::InitTriggers(triggers);

    // cast fireball as default
    triggers.push_back(new TriggerNode(
        "cast fireball",
        NextAction::array(0, new NextAction("fireball", 10.0f), NULL)));

    // initiate with scorch or reapply it, when the advanced scorch effect is not applied
    triggers.push_back(new TriggerNode(
        "improved scorch",
        NextAction::array(0, new NextAction("scorch", 20.0f), NULL)));

    // always use pyroblast, when hotstreak is available
    triggers.push_back(new TriggerNode(
        "hot streak",
        NextAction::array(0, new NextAction("pyroblast", 25.0f), NULL)));

    // use combustion boost, when required
    triggers.push_back(new TriggerNode(
        "combustion",
        NextAction::array(0, new NextAction("combustion", 50.0f), NULL)));

    // flee action (applies disorientation), when enemy is 2 close 2 you
    triggers.push_back(new TriggerNode(
        "enemy too close for spell",
        NextAction::array(0, new NextAction("dragon's breath", 70.0f), NULL)));
}

void FireMageAoeStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "medium aoe",
        NextAction::array(0, new NextAction("flamestrike", 20.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "living bomb",
        NextAction::array(0, new NextAction("living bomb", 25.0f), NULL)));
}

