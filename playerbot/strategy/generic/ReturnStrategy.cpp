//#include "ReturnStrategy.h"

#include "botpch.h"
#include "../../playerbot.h"
#include "ReturnStrategy.h"

using namespace ai;

void ReturnStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "return",
        NextAction::array(0, new NextAction("return", 250.0f), NULL)));
}

