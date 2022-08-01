#include "botpch.h"
#include "../../playerbot.h"
#include "PriestMultipliers.h"
#include "ShadowPriestStrategy.h"
#include "ShadowPriestStrategyActionNodeFactory.h"

using namespace ai;

void ShadowPriestStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericPriestStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "shadowform",
        NextAction::array(0, new NextAction("shadowform", ACTION_HIGH + 1), NULL)));

#ifdef MANGOSBOT_TWO
    triggers.push_back(new TriggerNode(
        "vampiric embrace",
        NextAction::array(0, new NextAction("vampiric embrace", ACTION_HIGH), NULL)));
#endif
    /*triggers.push_back(new TriggerNode(
        "low mana",
        NextAction::array(0, new NextAction("dispersion", ACTION_EMERGENCY + 5), NULL)));*/

    triggers.push_back(new TriggerNode(
        "mind blast",
        NextAction::array(0, new NextAction("inner focus", ACTION_NORMAL + 8), new NextAction("mind blast", ACTION_NORMAL + 8), NULL)));

    triggers.push_back(new TriggerNode(
        "mind flay",
        NextAction::array(0, new NextAction("mind flay", ACTION_NORMAL + 5), NULL)));
    
    triggers.push_back(new TriggerNode(
        "silence",
        NextAction::array(0, new NextAction("silence", ACTION_INTERRUPT + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "silence on enemy healer",
        NextAction::array(0, new NextAction("silence on enemy healer", ACTION_INTERRUPT), NULL)));

    triggers.push_back(new TriggerNode(
        "medium mana",
        NextAction::array(0, new NextAction("shadowfiend", ACTION_HIGH), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "low mana",
    //    NextAction::array(0, new NextAction("mana burn", ACTION_HIGH), NULL)));
}

void ShadowPriestAoeStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "high aoe",
        NextAction::array(0, new NextAction("mind sear", ACTION_HIGH), NULL)));

#ifndef MANGOSBOT_TWO
    triggers.push_back(new TriggerNode(
        "shadow word: pain on attacker",
        NextAction::array(0, new NextAction("shadow word: pain on attacker", 11.0f), NULL)));   
#endif
}

void ShadowPriestDebuffStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "vampiric touch",
        NextAction::array(0, new NextAction("vampiric touch", ACTION_NORMAL + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "devouring plague",
        NextAction::array(0, new NextAction("devouring plague", ACTION_NORMAL + 7), NULL)));

    triggers.push_back(new TriggerNode(
        "shadow word: pain",
        NextAction::array(0, new NextAction("shadow word: pain", ACTION_NORMAL + 6), NULL)));

#ifndef MANGOSBOT_TWO
    triggers.push_back(new TriggerNode(
        "feedback",
        NextAction::array(0, new NextAction("feedback", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "hex of weakness",
        NextAction::array(0, new NextAction("hex of weakness", ACTION_NORMAL), NULL)));
#endif
}
