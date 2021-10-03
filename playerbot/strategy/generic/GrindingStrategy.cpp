#include "botpch.h"
#include "../../playerbot.h"
#include "GrindingStrategy.h"

using namespace ai;


NextAction** GrindingStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("choose travel target", 1.0f), NULL);
}

void GrindingStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "no target",
        NextAction::array(0,
        new NextAction("attack anything", 10.0f), NULL))); 

    triggers.push_back(new TriggerNode(
        "no possible grind target",
        NextAction::array(0, new NextAction("move to travel target", 1.0f), NULL)));
}

