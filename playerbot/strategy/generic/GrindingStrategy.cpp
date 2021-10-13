#include "botpch.h"
#include "../../playerbot.h"
#include "GrindingStrategy.h"

using namespace ai;


class GrindingStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GrindingStrategyActionNodeFactory()
    {
        creators["attack anything"] = &attack_anything;
    }

private:
    static ActionNode* attack_anything(PlayerbotAI* ai)
    {
        return new ActionNode("attack anything",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("choose travel target"), NULL),
            /*C*/ NULL);
    }
};

GrindingStrategy::GrindingStrategy(PlayerbotAI* ai) : NonCombatStrategy(ai)
{
    actionNodeFactories.Add(new GrindingStrategyActionNodeFactory());
}

void GrindingStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "no target",
        NextAction::array(0,
        new NextAction("attack anything", 10.0f), NULL))); 

    triggers.push_back(new TriggerNode(
        "far from travel target",
        NextAction::array(0, new NextAction("move to travel target", 8.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "no rpg target",
        NextAction::array(0, new NextAction("choose rpg target", 5.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "far from rpg target",
        NextAction::array(0, new NextAction("rpg", 8), NULL)));



}

