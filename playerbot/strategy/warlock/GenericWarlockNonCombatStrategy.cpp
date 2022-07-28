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
        "no armor",
        NextAction::array(0, new NextAction("fel armor", ACTION_HIGH), new NextAction("demon armor", ACTION_HIGH - 1), new NextAction("demon skin", ACTION_HIGH - 2), NULL)));

    triggers.push_back(new TriggerNode(
        "no spellstone",
        NextAction::array(0, new NextAction("create spellstone", 5.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "spellstone",
        NextAction::array(0, new NextAction("use spellstone", 4.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "no soulstone",
        NextAction::array(0, new NextAction("create soulstone", 4.0f), NULL)));  
    
    triggers.push_back(new TriggerNode(
        "use soulstone",
        NextAction::array(0, new NextAction("use soulstone on master", 3.0f), NULL)));    
    
    triggers.push_back(new TriggerNode(
        "remove soul shard",
        NextAction::array(0, new NextAction("remove soul shard", 2.0f), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "no healthstone",
    //    NextAction::array(0, new NextAction("create healthstone", 3.0f), NULL)));


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
        NextAction::array(0, new NextAction("summon felhunter", ACTION_NORMAL + 1), NULL)));// new NextAction(bestPet, ACTION_HIGH), new NextAction("summon succubus", ACTION_NORMAL + 2), new NextAction("summon imp", ACTION_NORMAL + 1), NULL)));
}
