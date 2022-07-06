#include "botpch.h"
#include "../../playerbot.h"
#include "PaladinMultipliers.h"
#include "HealPaladinStrategy.h"

using namespace ai;

HealPaladinStrategy::HealPaladinStrategy(PlayerbotAI* ai) : GenericPaladinStrategy(ai)
{
}

void HealPaladinStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericPaladinStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "seal",
        NextAction::array(0, new NextAction("seal of wisdom", ACTION_HIGH + 10), NULL)));

    triggers.push_back(new TriggerNode(
        "medium health",
        NextAction::array(0, new NextAction("holy shock", ACTION_MEDIUM_HEAL + 2), new NextAction("holy light", ACTION_MEDIUM_HEAL + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "party member medium health",
        NextAction::array(0, new NextAction("holy shock on party", ACTION_MEDIUM_HEAL + 1), new NextAction("holy light on party", ACTION_MEDIUM_HEAL + 0), NULL)));

    triggers.push_back(new TriggerNode(
        "blessing",
        NextAction::array(0, new NextAction("blessing of sanctuary", ACTION_HIGH + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "party member to heal out of spell range",
        NextAction::array(0, new NextAction("reach party member to heal", ACTION_CRITICAL_HEAL + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "beacon of light on tank",
        NextAction::array(0, new NextAction("beacon of light on party", ACTION_HIGH + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "sacred shield on tank",
        NextAction::array(0, new NextAction("sacred shield", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "low mana",
        NextAction::array(0, new NextAction("divine illumination", ACTION_HIGH + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "party member low health",
        NextAction::array(0, new NextAction("divine favor", ACTION_CRITICAL_HEAL + 1), new NextAction("holy shock on party", ACTION_CRITICAL_HEAL + 1), NULL)));


}
