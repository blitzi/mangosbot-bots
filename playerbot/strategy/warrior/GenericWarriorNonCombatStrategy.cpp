#include "botpch.h"
#include "../../playerbot.h"
#include "WarriorMultipliers.h"
#include "GenericWarriorNonCombatStrategy.h"

using namespace ai;

void GenericWarriorNonCombatStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
   NonCombatStrategy::InitTriggers(triggers);

   triggers.push_back(new TriggerNode(
      "often",
      NextAction::array(0, new NextAction("apply stone", 1.0f), NULL)));

#ifndef MANGOSBOT_TWO
   triggers.push_back(new TriggerNode(
       "battle stance",
       NextAction::array(0, new NextAction("battle stance", 1.0f), NULL)));
#endif

   triggers.push_back(new TriggerNode(
       "enemy out of melee",
       NextAction::array(0, new NextAction("charge", ACTION_MOVE + 9), NULL)));
}
