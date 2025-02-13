#include "botpch.h"
#include "../../playerbot.h"
#include "DruidMultipliers.h"
#include "GenericDruidNonCombatStrategy.h"

using namespace ai;

class GenericDruidNonCombatStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericDruidNonCombatStrategyActionNodeFactory()
    {
        creators["thorns"] = &thorns;
        creators["thorns on party"] = &thorns_on_party;
        creators["mark of the wild"] = &mark_of_the_wild;
        creators["mark of the wild on party"] = &mark_of_the_wild_on_party;
        creators["innervate"] = &innervate;
    }
private:
    static ActionNode* thorns(PlayerbotAI* ai)
    {
        return new ActionNode("thorns",
            /*P*/ NextAction::array(0, new NextAction("caster form"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* thorns_on_party(PlayerbotAI* ai)
    {
        return new ActionNode("thorns on party",
            /*P*/ NextAction::array(0, new NextAction("caster form"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* mark_of_the_wild(PlayerbotAI* ai)
    {
        return new ActionNode ("mark of the wild",
            /*P*/ NextAction::array(0, new NextAction("caster form"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* mark_of_the_wild_on_party(PlayerbotAI* ai)
    {
        return new ActionNode ("mark of the wild on party",
            /*P*/ NextAction::array(0, new NextAction("caster form"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* innervate(PlayerbotAI* ai)
    {
        return new ActionNode ("innervate",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("drink"), NULL),
            /*C*/ NULL);
    }
};

GenericDruidNonCombatStrategy::GenericDruidNonCombatStrategy(PlayerbotAI* ai) : NonCombatStrategy(ai)
{
    actionNodeFactories.Add(new GenericDruidNonCombatStrategyActionNodeFactory());
}

void GenericDruidNonCombatStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "mark of the wild",
        NextAction::array(0, new NextAction("mark of the wild", 14.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "thorns",
        NextAction::array(0, new NextAction("thorns", 12.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "cure poison",
        NextAction::array(0, new NextAction("abolish poison", 21.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cure poison",
        NextAction::array(0, new NextAction("abolish poison on party", 20.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"party member dead",
		NextAction::array(0, new NextAction("revive", 22.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "low mana",
        NextAction::array(0, new NextAction("innervate", ACTION_EMERGENCY + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "swimming",
        NextAction::array(0, new NextAction("aquatic form", 1.0f), NULL)));

    triggers.push_back(new TriggerNode(
       "often",
       NextAction::array(0, new NextAction("apply oil", 1.0f), NULL)));


    // healing
    {
        triggers.push_back(new TriggerNode(
            "party member almost full health",
            NextAction::array(0, new NextAction("rejuvenation on party", ACTION_LIGHT_HEAL + 1), NULL)));

        // rejuv ourself on any dmg
        triggers.push_back(new TriggerNode(
            "almost full health",
            NextAction::array(0, new NextAction("rejuvenation", ACTION_LIGHT_HEAL + 2), NULL)));

        triggers.push_back(new TriggerNode(
            "party member medium health",
            NextAction::array(0, new NextAction("regrowth on party", ACTION_LIGHT_HEAL + 3), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("regrowth", ACTION_LIGHT_HEAL + 4), NULL)));

        triggers.push_back(new TriggerNode(
            "party member medium health",
            NextAction::array(0, new NextAction("healing touch on party", ACTION_LIGHT_HEAL + 3), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("healing touch", ACTION_LIGHT_HEAL + 4), NULL)));

        triggers.push_back(new TriggerNode(
            "medium aoe heal",
            NextAction::array(0, new NextAction("wild growth", ACTION_LIGHT_HEAL + 5), NULL)));

        triggers.push_back(new TriggerNode(
            "party member low or critical health",
            NextAction::array(0, new NextAction("healing touch on party", ACTION_LIGHT_HEAL + 7), new NextAction("regrowth on party", ACTION_LIGHT_HEAL + 6), NULL)));

        triggers.push_back(new TriggerNode(
            "low or critical health",
            NextAction::array(0, new NextAction("healing touch", ACTION_LIGHT_HEAL + 9), new NextAction("regrowth", ACTION_LIGHT_HEAL + 8), NULL)));

        triggers.push_back(new TriggerNode(
            "low aoe heal",
            NextAction::array(0, new NextAction("wild growth", ACTION_LIGHT_HEAL + 10), NULL)));

        triggers.push_back(new TriggerNode(
            "critical aoe heal",
            NextAction::array(0, new NextAction("tranquility", ACTION_MEDIUM_HEAL + 10), NULL)));
    }
}

GenericDruidBuffStrategy::GenericDruidBuffStrategy(PlayerbotAI* ai) : NonCombatStrategy(ai)
{
    actionNodeFactories.Add(new GenericDruidNonCombatStrategyActionNodeFactory());
}

void GenericDruidBuffStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "mark of the wild on party",
        NextAction::array(0, new NextAction("mark of the wild on party", 13.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "thorns on party",
        NextAction::array(0, new NextAction("thorns on party", 11.0f), NULL)));
}
