#include "botpch.h"
#include "../../playerbot.h"
#include "PriestMultipliers.h"
#include "ShadowPriestStrategy.h"
#include "ShadowPriestStrategyActionNodeFactory.h"

using namespace ai;

ShadowPriestStrategy::ShadowPriestStrategy(PlayerbotAI* ai) : GenericPriestStrategy(ai)
{
    //actionNodeFactories.Add(new ShadowPriestStrategyActionNodeFactory());
}

NextAction** ShadowPriestStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("shoot", 7.0f), new NextAction("melee", 6.0f), NULL);
}

void ShadowPriestStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericPriestStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "enemy out of spell",
        NextAction::array(0, new NextAction("reach spell", ACTION_MOVE + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "shadowform",
        NextAction::array(0, new NextAction("shadowform", ACTION_HIGH + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "vampiric embrace",
        NextAction::array(0, new NextAction("vampiric embrace", ACTION_HIGH), NULL)));

    /*triggers.push_back(new TriggerNode(
        "low mana",
        NextAction::array(0, new NextAction("dispersion", ACTION_EMERGENCY + 5), NULL)));*/

    //new NextAction("mind blast", 10.0f), new NextAction("starshards", 8.0f),

    triggers.push_back(new TriggerNode(
        "mind blast",
        NextAction::array(0, new NextAction("inner focus", ACTION_NORMAL + 4), new NextAction("mind blast", ACTION_NORMAL + 4), NULL)));

    triggers.push_back(new TriggerNode(
        "mind flay",
        NextAction::array(0, new NextAction("mind flay", ACTION_NORMAL + 1), NULL)));
    
    triggers.push_back(new TriggerNode(
        "silence",
        NextAction::array(0, new NextAction("silence", ACTION_INTERRUPT + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "silence on enemy healer",
        NextAction::array(0, new NextAction("silence on enemy healer", ACTION_INTERRUPT), NULL)));

    triggers.push_back(new TriggerNode(
        "shadowfiend",
        NextAction::array(0, new NextAction("shadowfiend", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "medium mana",
        NextAction::array(0, new NextAction("shadowfiend", ACTION_HIGH), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "low mana",
    //    NextAction::array(0, new NextAction("mana burn", ACTION_HIGH), NULL)));
}

void ShadowPriestAoeStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    // TBC
    /*triggers.push_back(new TriggerNode(
        "shadow word: pain on attacker",
        NextAction::array(0, new NextAction("shadow word: pain on attacker", 11.0f), NULL)));*/

    triggers.push_back(new TriggerNode(
        "vampiric touch",
        NextAction::array(0, new NextAction("vampiric touch", 11.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "devouring plague",
        NextAction::array(0, new NextAction("devouring plague", 11.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "mind sear",
        NextAction::array(0, new NextAction("mind sear", 11.0f), NULL)));
}

void ShadowPriestDebuffStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "vampiric touch",
        NextAction::array(0, new NextAction("vampiric touch", ACTION_NORMAL + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "devouring plague",
        NextAction::array(0, new NextAction("devouring plague", ACTION_NORMAL + 3), NULL)));

    triggers.push_back(new TriggerNode(
        "shadow word: pain",
        NextAction::array(0, new NextAction("shadow word: pain", ACTION_NORMAL + 2), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "feedback",
    //    NextAction::array(0, new NextAction("feedback", 80.0f), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "hex of weakness",
    //    NextAction::array(0, new NextAction("hex of weakness", 10.0f), NULL)));
}
