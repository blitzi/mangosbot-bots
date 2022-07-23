#include "botpch.h"
#include "../../playerbot.h"
#include "CombatStrategy.h"

using namespace ai;

void CombatStrategy::InitTriggers(list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "enemy out of spell",
        NextAction::array(0, new NextAction("reach spell", ACTION_NORMAL + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "invalid target",
        NextAction::array(0, new NextAction("drop target", 55), NULL)));

	triggers.push_back(new TriggerNode(
		"no group member in combat",
		NextAction::array(0, new NextAction("leave combat state", 54), NULL)));

    triggers.push_back(new TriggerNode(
        "mounted",
        NextAction::array(0, new NextAction("check mount state", 53), NULL)));

    triggers.push_back(new TriggerNode(
        "out of react range",
        NextAction::array(0, new NextAction("flee to master", 52), NULL)));

    triggers.push_back(new TriggerNode(
        "party member to cancel health",
        NextAction::array(0, new NextAction("cancel heal", 200.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "use trinket",
        NextAction::array(0, new NextAction("use trinket", ACTION_EMERGENCY), NULL)));

    triggers.push_back(new TriggerNode(
        "damage stop",
        NextAction::array(0, new NextAction("damage stop", 200.0f), NULL)));
}
