#include "botpch.h"
#include "../../playerbot.h"
#include "DruidMultipliers.h"
#include "HealDruidStrategy.h"

using namespace ai;

class HealDruidStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    HealDruidStrategyActionNodeFactory()
    {
    }
private:
};

HealDruidStrategy::HealDruidStrategy(PlayerbotAI* ai) : GenericDruidStrategy(ai)
{
    actionNodeFactories.Add(new HealDruidStrategyActionNodeFactory());
}

void HealDruidStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericDruidStrategy::InitTriggers(triggers);

    // low section
    {
        triggers.push_back(new TriggerNode(
            "enemy out of spell",
            NextAction::array(0, new NextAction("reach spell", ACTION_NORMAL + 9), NULL)));

        triggers.push_back(new TriggerNode(
            "party member almost full health",
            NextAction::array(0, new NextAction("rejuvenation on party", ACTION_LIGHT_HEAL + 1), NULL)));

        triggers.push_back(new TriggerNode(
            "almost full health",
            NextAction::array(0, new NextAction("rejuvenation", ACTION_LIGHT_HEAL + 2), NULL)));
    }

    // normal section
    {
        triggers.push_back(new TriggerNode(
            "party member medium health",
            NextAction::array(0, new NextAction("regrowth on party", ACTION_MEDIUM_HEAL + 1), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("regrowth", ACTION_MEDIUM_HEAL + 2), NULL)));

        //  apply nourish, whenever anyone is at medium health and has a druidHoT (regrowth, rejuv, lifebloom, wildgrowth) available
        triggers.push_back(new TriggerNode(
            "nourish on party",
            NextAction::array(0, new NextAction("nourish on party", ACTION_MEDIUM_HEAL + 3), NULL)));

        triggers.push_back(new TriggerNode(
            "nourish",
            NextAction::array(0, new NextAction("nourish", ACTION_MEDIUM_HEAL + 4), NULL)));

        // TODO: cast lifebloom on free isntant cast on medium or low health unit
    }

    // critical section
    {
        triggers.push_back(new TriggerNode(
            "party member to heal out of spell range",
            NextAction::array(0, new NextAction("reach party member to heal", ACTION_CRITICAL_HEAL + 1), NULL)));

        // cast wildgrowth when: in a party & whenever available & anyone lost any portion of hp
        triggers.push_back(new TriggerNode(
            "party member almost full health",
            NextAction::array(0, new NextAction("wild growth on party", ACTION_CRITICAL_HEAL + 2), NULL)));

        // add swiftmend, whenever anyone is at medium health and has a druid (regrowth, rejuv, wildgrowth)
        triggers.push_back(new TriggerNode(
            "swiftmend on party",
            NextAction::array(0, new NextAction("swiftmend on party", ACTION_CRITICAL_HEAL + 4), NULL)));
        triggers.push_back(new TriggerNode(
            "swiftmend",
            NextAction::array(0, new NextAction("swiftmend", ACTION_CRITICAL_HEAL + 5), NULL)));

        triggers.push_back(new TriggerNode(
            "critical aoe heal",
            NextAction::array(0, new NextAction("barkskin"), new NextAction("tranquility", ACTION_CRITICAL_HEAL + 8), NULL)));


        // TODO: cast nature's swiftness + healing touch on critical health of any player
    }

    // emergencies
    {
        triggers.push_back(new TriggerNode(
            "tree form",
            NextAction::array(0, new NextAction("tree form", ACTION_EMERGENCY), NULL)));
    }
}
