#include "botpch.h"
#include "../../playerbot.h"
#include "WarlockMultipliers.h"
#include "GenericWarlockNonCombatStrategy.h"

using namespace ai;

GenericWarlockNonCombatStrategy::GenericWarlockNonCombatStrategy(PlayerbotAI* ai) : NonCombatStrategy(ai)
{
}

void GenericWarlockNonCombatStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "fel armor",
        NextAction::array(0, new NextAction("fel armor", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "demon armor",
        NextAction::array(0, new NextAction("demon armor", ACTION_NORMAL + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "demon skin",
        NextAction::array(0, new NextAction("demon skin", ACTION_NORMAL), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "no spellstone",
    //    NextAction::array(0, new NextAction("create spellstone", 15.0f), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "spellstone",
    //    NextAction::array(0, new NextAction("spellstone", 14.0f), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "no soulstone",
    //    NextAction::array(0, new NextAction("create soulstone", 12.0f), NULL)));  
    //
    //triggers.push_back(new TriggerNode(
    //    "use soulstone",
    //    NextAction::array(0, new NextAction("use soulstone on master", 11.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "no healthstone",
        NextAction::array(0, new NextAction("create healthstone", 13.0f), NULL)));


    /*

	triggers.push_back(new TriggerNode(
		"no firestone",
		NextAction::array(0, new NextAction("create firestone", 14.0f), NULL)));
        */
    // TODO Fix Spellstone / Firestone
}

void WarlockPetStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "no pet",
        NextAction::array(0, new NextAction(bestPet, ACTION_HIGH), new NextAction("summon succubus", ACTION_NORMAL + 2), new NextAction("summon imp", ACTION_NORMAL + 1), NULL)));
}
