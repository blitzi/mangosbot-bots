#include "botpch.h"
#include "../../playerbot.h"
#include "WarriorMultipliers.h"
#include "ArmsWarriorStrategy.h"

using namespace ai;

class ArmsWarriorStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    ArmsWarriorStrategyActionNodeFactory()
    {
        creators["charge"] = &charge;
        creators["death wish"] = &death_wish;
        creators["piercing howl"] = &piercing_howl;
        creators["mocking blow"] = &mocking_blow;
    }
private:
    ACTION_NODE_A(charge, "charge", "reach melee");
    ACTION_NODE_A(death_wish, "death wish", "bloodrage");
    ACTION_NODE_A(piercing_howl, "piercing howl", "mocking blow");
    ACTION_NODE_A(mocking_blow, "mocking blow", "hamstring");
};

ArmsWarriorStrategy::ArmsWarriorStrategy(PlayerbotAI* ai) : GenericWarriorStrategy(ai)
{
    actionNodeFactories.Add(new ArmsWarriorStrategyActionNodeFactory());
    //actionNodeFactories.Add(new WarriorStanceRequirementActionNodeFactory());
}

NextAction** ArmsWarriorStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("melee", ACTION_NORMAL), NULL);
}

void ArmsWarriorStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericWarriorStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "enemy out of melee",
        NextAction::array(0, new NextAction("charge", ACTION_MOVE + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "battle stance",
        NextAction::array(0, new NextAction("battle stance", ACTION_HIGH + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "battle shout",
        NextAction::array(0, new NextAction("battle shout", ACTION_HIGH + 8), NULL)));

    triggers.push_back(new TriggerNode(
        "mortal strike",
        NextAction::array(0, new NextAction("mortal strike", ACTION_HIGH + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "target critical health",
        NextAction::array(0, new NextAction("execute", ACTION_HIGH + 4), NULL)));

	triggers.push_back(new TriggerNode(
		"hamstring",
		NextAction::array(0, new NextAction("piercing howl", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "overpower",
        NextAction::array(0, new NextAction("overpower", ACTION_HIGH + 3), NULL)));

	triggers.push_back(new TriggerNode(
		"victory rush",
		NextAction::array(0, new NextAction("victory rush", ACTION_HIGH + 3), NULL)));

    triggers.push_back(new TriggerNode(
        "high rage available",
        NextAction::array(0, new NextAction("heroic strike", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "bloodrage",
        NextAction::array(0, new NextAction("bloodrage", ACTION_HIGH + 2), NULL)));

    triggers.push_back(new TriggerNode(
        "boost",
        NextAction::array(0, new NextAction("death wish", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "rend",
        NextAction::array(0, new NextAction("rend", ACTION_NORMAL + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "critical health",
        NextAction::array(0, new NextAction("intimidating shout", ACTION_EMERGENCY), NULL)));
}


void ArmsWarrirorAoeStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    /*triggers.push_back(new TriggerNode(
        "rend on attacker",
        NextAction::array(0, new NextAction("rend on attacker", ACTION_HIGH + 1), NULL)));*/

    triggers.push_back(new TriggerNode(
        "thunder clap",
        NextAction::array(0, new NextAction("thunder clap on snare target", ACTION_HIGH + 3), NULL)));

    triggers.push_back(new TriggerNode(
        "medium aoe",
        NextAction::array(0, new NextAction("thunder clap", ACTION_HIGH + 3), new NextAction("demoralizing shout", ACTION_HIGH + 2), NULL)));

    triggers.push_back(new TriggerNode(
        "light aoe",
        NextAction::array(0, new NextAction("cleave", ACTION_HIGH + 2), NULL)));

    triggers.push_back(new TriggerNode(
        "light aoe",
        NextAction::array(0, new NextAction("sweeping strikes", ACTION_HIGH + 3), NULL)));
}
