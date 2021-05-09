#include "botpch.h"
#include "../../playerbot.h"
#include "../PassiveMultiplier.h"
#include "PullStrategy.h"

using namespace ai;

class PullMultiplier : public PassiveMultiplier
{
public:
    PullMultiplier(PlayerbotAI* ai) : PassiveMultiplier(ai) {}

public:
    virtual float GetValue(Action* action);

private:
};

float PullMultiplier::GetValue(Action* action)
{
    if (!action)
        return 1.0f;

    string name = action->getName();
    if (ai->GetPullAction() == name ||
        name == "reach spell" ||
        name == "follow" ||
        name == "-pull")
        return 1.0f;

    return PassiveMultiplier::GetValue(action);
}

class PullStrategyActionNodeFactoryInternal : public NamedObjectFactory<ActionNode>
{
public:
    PullStrategyActionNodeFactoryInternal(string action)
    {
        creators[action] = &pull_action;
        creators["follow"] = &pull_finish;
    }

private:

    static ActionNode* pull_action(PlayerbotAI* ai)
    {
        return new ActionNode(ai->GetPullAction(),
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NextAction::array(0, new NextAction("end pull", 102), NULL));
    }
    static ActionNode* pull_finish(PlayerbotAI* ai)
    {
        return new ActionNode("follow",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("end pull", 102), NULL),
            /*C*/ NextAction::array(0, new NextAction("end pull", 102), NULL));
    }    
};

void PullStrategy::InitMultipliers(std::list<Multiplier*>& multipliers)
{
    CombatStrategy::InitMultipliers(multipliers);
    multipliers.push_back(new PullMultiplier(ai));
}

PullStrategy::PullStrategy(PlayerbotAI* ai, string action) : CombatStrategy(ai)
{
    ai->SetPullAction(action);        

    actionNodeFactories.Add(new PullStrategyActionNodeFactoryInternal(action));
}

void PullStrategy::InitTriggers(std::list<TriggerNode*>& triggers)
{
    CombatStrategy::InitTriggers(triggers);
}

NextAction** PullStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction(ai->GetPullAction(), 104), NULL);
}

void PossibleAdsStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    Strategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "possible ads",
        NextAction::array(0, new NextAction("flee with pet", 60), NULL)));
}