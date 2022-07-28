#include "botpch.h"
#include "../../playerbot.h"
#include "WarlockTriggers.h"
#include "WarlockMultipliers.h"
#include "DpsWarlockStrategy.h"
#include "WarlockActions.h"

using namespace ai;

// affliction
AfflictionWarlockStrategy::AfflictionWarlockStrategy(PlayerbotAI* ai) : GenericWarlockStrategy(ai)
{
}

void AfflictionWarlockStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericWarlockStrategy::InitTriggers(triggers);
    
    int rotationBaseValue = ACTION_NORMAL;
    // free cast
    /*triggers.push_back(new TriggerNode(
		"free shadow bold",
		NextAction::array(0, new NextAction("shadow bolt", ACTION_EMERGENCY), NULL)));*/

    triggers.push_back(new TriggerNode(
        "shadow trance",
        NextAction::array(0, new NextAction("shadow bolt", 200.0f), NULL)));

    // TODO
    // 0. livetap with glyphe ACTION_NORMAL + 9
    triggers.push_back(new TriggerNode(
        "life tap glyph",
        NextAction::array(0, new NextAction("life tap rank one", rotationBaseValue + 9), NULL)));
    
    // 1. first shadowbold for buff
	triggers.push_back(new TriggerNode(
		"improved shadow bolt",
		NextAction::array(0, new NextAction("shadow bolt", rotationBaseValue + 8), NULL)));
    // 2.
    triggers.push_back(new TriggerNode(
        "corruption",
        NextAction::array(0, new NextAction("corruption", rotationBaseValue + 7), NULL)));
    // 3.
    triggers.push_back(new TriggerNode(
        "unstable affliction",
        NextAction::array(0, new NextAction("unstable affliction", rotationBaseValue + 6), NULL)));
    // 4.
    triggers.push_back(new TriggerNode(
        "haunt",
        NextAction::array(0, new NextAction("haunt", rotationBaseValue + 5), NULL)));
    // 5.
    triggers.push_back(new TriggerNode(
        "curse of agony",
        NextAction::array(0, new NextAction("curse of agony", rotationBaseValue + 4), NULL)));
    // 6.
    triggers.push_back(new TriggerNode(
        "shadow bold",
        NextAction::array(0, new NextAction("shadow bolt", rotationBaseValue + 3), NULL)));

    triggers.push_back(new TriggerNode(
    	"drain soul",
    	NextAction::array(0, new NextAction("drain soul", rotationBaseValue + 15), NULL)));
    /*triggers.push_back(new TriggerNode(
        "low hp drain soul",
        NextAction::array(0, new NextAction("drain soul", ACTION_HIGH), NULL)));*/
}


// destro
class DpsWarlockStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    DpsWarlockStrategyActionNodeFactory()
    {
        creators["shadow bolt"] = &shadow_bolt;
    }
private:
    static ActionNode* shadow_bolt(PlayerbotAI* ai)
    {
        return new ActionNode("shadow bolt",
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NULL);
    }
};

DpsWarlockStrategy::DpsWarlockStrategy(PlayerbotAI* ai) : GenericWarlockStrategy(ai)
{
    actionNodeFactories.Add(new DpsWarlockStrategyActionNodeFactory());
}

NextAction** DpsWarlockStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("incinirate", 10.0f), new NextAction("shadow bolt", 10.0f), NULL);
}

void DpsWarlockStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericWarlockStrategy::InitTriggers(triggers);

	triggers.push_back(new TriggerNode(
		"backlash", // free cast
		NextAction::array(0, new NextAction("shadow bolt", 20.0f), NULL)));
}


// aoe
void DpsAoeWarlockStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    //triggers.push_back(new TriggerNode(
    //    "medium aoe",
    //    NextAction::array(0, new NextAction("rain of fire", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "medium aoe",
        NextAction::array(0, new NextAction("seed of corruption", ACTION_HIGH +1), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "corruption on attacker",
    //    NextAction::array(0, new NextAction("corruption on attacker", 27.0f), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "curse of agony on attacker",
    //    NextAction::array(0, new NextAction("curse of agony on attacker", 28.0f), NULL)));

    //triggers.push_back(new TriggerNode(
    //    "siphon life on attacker",
    //    NextAction::array(0, new NextAction("siphon life on attacker", 29.0f), NULL)));
}

// debuff
void DpsWarlockDebuffStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "corruption",
        NextAction::array(0, new NextAction("corruption", 22.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "curse of agony",
        NextAction::array(0, new NextAction("curse of agony", 21.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "siphon life",
        NextAction::array(0, new NextAction("siphon life", 23.0f), NULL)));
}
