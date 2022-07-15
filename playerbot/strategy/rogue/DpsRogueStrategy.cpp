#include "botpch.h"
#include "../../playerbot.h"
#include "RogueMultipliers.h"
#include "DpsRogueStrategy.h"

using namespace ai;


GenericRogueStrategy::GenericRogueStrategy(PlayerbotAI* ai) : CombatStrategy(ai)
{
}

void GenericRogueStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    CombatStrategy::InitTriggers(triggers);

	triggers.push_back(new TriggerNode(
		"medium threat",
		NextAction::array(0, new NextAction("feint", ACTION_HIGH), NULL)));

	triggers.push_back(new TriggerNode(
		"low health",
		NextAction::array(0, new NextAction("evasion", ACTION_EMERGENCY), new NextAction("feint", ACTION_EMERGENCY), NULL)));

    triggers.push_back(new TriggerNode(
        "critical health",
        NextAction::array(0, new NextAction("blind", ACTION_EMERGENCY), new NextAction("vanish", ACTION_EMERGENCY), NULL)));

	triggers.push_back(new TriggerNode(
		"kick",
		NextAction::array(0, new NextAction("kick", ACTION_INTERRUPT + 2), NULL)));

	triggers.push_back(new TriggerNode(
		"kick on enemy healer",
		NextAction::array(0, new NextAction("kick on enemy healer", ACTION_INTERRUPT + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "player has flag",
        NextAction::array(0, new NextAction("sprint", ACTION_EMERGENCY + 2), NULL)));

    triggers.push_back(new TriggerNode(
        "enemy flagcarrier near",
        NextAction::array(0, new NextAction("sprint", ACTION_EMERGENCY + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "in stealth",
        NextAction::array(0, new NextAction("check stealth", ACTION_EMERGENCY), NULL)));

    triggers.push_back(new TriggerNode(
        "unstealth",
        NextAction::array(0, new NextAction("unstealth", ACTION_NORMAL), NULL)));

    triggers.push_back(new TriggerNode(
        "sprint",
        NextAction::array(0, new NextAction("sprint", ACTION_INTERRUPT), NULL)));

    triggers.push_back(new TriggerNode(
        "tricks of the trade on tank",
        NextAction::array(0, new NextAction("tricks of the trade on tank", ACTION_HIGH), NULL)));


}

DpsRogueStrategy::DpsRogueStrategy(PlayerbotAI* ai) : GenericRogueStrategy(ai)
{
}

void DpsRogueStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericRogueStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "slice and dice",
        NextAction::array(0, new NextAction("slice and dice", ACTION_HIGH + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "rupture",
        NextAction::array(0, new NextAction("rupture", ACTION_HIGH + 4), NULL)));

    triggers.push_back(new TriggerNode(
        "eviscerate",
        NextAction::array(0, new NextAction("eviscerate", ACTION_HIGH + 3), NULL)));

    triggers.push_back(new TriggerNode(
        "sinister strike",
        NextAction::array(0, new NextAction("sinister strike", ACTION_NORMAL + 5), NULL)));
}

class StealthedRogueStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    StealthedRogueStrategyActionNodeFactory()
    {
        creators["ambush"] = &ambush;
        creators["cheap shot"] = &cheap_shot;
        creators["garrote"] = &garrote;
        creators["sap"] = &sap;
        creators["sinister strike"] = &sinister_strike;
    }
private:
    static ActionNode* ambush(PlayerbotAI* ai)
    {
        return new ActionNode("ambush",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("garrote"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* cheap_shot(PlayerbotAI* ai)
    {
        return new ActionNode("cheap shot",
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* garrote(PlayerbotAI* ai)
    {
        return new ActionNode("garrote",
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* sap(PlayerbotAI* ai)
    {
        return new ActionNode("sap",
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* sinister_strike(PlayerbotAI* ai)
    {
        return new ActionNode("sinister strike",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("cheap shot"), NULL),
            /*C*/ NULL);
    }
};

StealthedRogueStrategy::StealthedRogueStrategy(PlayerbotAI* ai) : Strategy(ai)
{
    actionNodeFactories.Add(new StealthedRogueStrategyActionNodeFactory());
}

NextAction** StealthedRogueStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("ambush", ACTION_NORMAL + 4),
        new NextAction("backstab", ACTION_NORMAL + 3),
        new NextAction("cheap shot", ACTION_NORMAL + 2),
        new NextAction("sinister strike", ACTION_NORMAL + 1),
        new NextAction("melee", ACTION_NORMAL),
        NULL);
}

void StealthedRogueStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "combo points available",
        NextAction::array(0, new NextAction("eviscerate", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "kick",
        NextAction::array(0, new NextAction("cheap shot", ACTION_INTERRUPT), NULL)));

    triggers.push_back(new TriggerNode(
        "kick on enemy healer",
        NextAction::array(0, new NextAction("cheap shot", ACTION_INTERRUPT), NULL)));

    triggers.push_back(new TriggerNode(
        "behind target",
        NextAction::array(0, new NextAction("ambush", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "not behind target",
        NextAction::array(0, new NextAction("cheap shot", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "enemy flagcarrier near",
        NextAction::array(0, new NextAction("sprint", ACTION_EMERGENCY + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "unstealth",
        NextAction::array(0, new NextAction("unstealth", ACTION_NORMAL), NULL)));

    /*triggers.push_back(new TriggerNode(
        "low health",
        NextAction::array(0, new NextAction("food", ACTION_EMERGENCY + 1), NULL)));*/

    triggers.push_back(new TriggerNode(
        "no stealth",
        NextAction::array(0, new NextAction("check stealth", ACTION_EMERGENCY), NULL)));

    triggers.push_back(new TriggerNode(
        "sprint",
        NextAction::array(0, new NextAction("sprint", ACTION_INTERRUPT), NULL)));
}

void StealthStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "stealth",
        NextAction::array(0, new NextAction("stealth", ACTION_INTERRUPT), NULL)));
}

void RogueCcStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "sap",
        NextAction::array(0, new NextAction("stealth", ACTION_INTERRUPT), new NextAction("sap", ACTION_INTERRUPT), NULL)));
}

// AOE
void RogueAoeStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "medium aoe",
        NextAction::array(0, new NextAction("adrenalin rush", ACTION_HIGH), new NextAction("fan of knives", ACTION_HIGH), NULL)));
}

// boost
void RogueBoostStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "adrenaline rush",
        NextAction::array(0, new NextAction("adrenaline rush", ACTION_NORMAL + 6), NULL)));

    triggers.push_back(new TriggerNode(
        "blade flurry",
        NextAction::array(0, new NextAction("blade flurry", ACTION_NORMAL + 9), NULL)));
}

// Assassination
AssassinationRogueStrategy::AssassinationRogueStrategy(PlayerbotAI* ai) : GenericRogueStrategy(ai)
{
}

void AssassinationRogueStrategy::InitTriggers(std::list<TriggerNode*>& triggers)
{
    GenericRogueStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "combo points available",
        NextAction::array(0, new NextAction("rupture", ACTION_HIGH + 2), NULL)));

    triggers.push_back(new TriggerNode(
        "behind target",
        NextAction::array(0, new NextAction("mutilate", ACTION_HIGH + 2), NULL)));   
    
    triggers.push_back(new TriggerNode(
        "mutilate",
        NextAction::array(0, new NextAction("mutilate", ACTION_HIGH + 2), NULL)));
}

// Combat
CombatRogueStrategy::CombatRogueStrategy(PlayerbotAI* ai) : GenericRogueStrategy(ai)
{
}

void CombatRogueStrategy::InitTriggers(std::list<TriggerNode*>& triggers)
{
    GenericRogueStrategy::InitTriggers(triggers);

    //triggers.push_back(new TriggerNode(
    //    "blade flurry with killing spree",
    //    NextAction::array(0, new NextAction("blade flurry", ACTION_NORMAL + 9), new NextAction("killing spree", ACTION_NORMAL + 8), NULL)));
    
    triggers.push_back(new TriggerNode(
        "killing spree",
        NextAction::array(0, new NextAction("killing spree", ACTION_NORMAL + 8), NULL)));

    triggers.push_back(new TriggerNode(
        "sinister strike",
        NextAction::array(0, new NextAction("sinister strike", ACTION_NORMAL + 4), NULL)));

    triggers.push_back(new TriggerNode(
        "slice and dice",
        NextAction::array(0, new NextAction("slice and dice", ACTION_HIGH + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "rupture",
        NextAction::array(0, new NextAction("rupture", ACTION_HIGH + 4), NULL)));

    triggers.push_back(new TriggerNode(
        "eviscerate",
        NextAction::array(0, new NextAction("eviscerate", ACTION_HIGH + 3), NULL)));
}

// TODO subtlety
