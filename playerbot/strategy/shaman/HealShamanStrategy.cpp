#include "botpch.h"
#include "../../playerbot.h"
#include "ShamanMultipliers.h"
#include "HealShamanStrategy.h"

using namespace ai;

class HealShamanStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    HealShamanStrategyActionNodeFactory()
    {
        creators["earthliving weapon"] = &earthliving_weapon;
        creators["mana tide totem"] = &mana_tide_totem;
    }
private:
    static ActionNode* earthliving_weapon(PlayerbotAI* ai)
    {
        return new ActionNode ("earthliving weapon",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("flametongue weapon"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* mana_tide_totem(PlayerbotAI* ai)
    {
        return new ActionNode ("mana tide totem",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("mana potion"), NULL),
            /*C*/ NULL);
    }
};

HealShamanStrategy::HealShamanStrategy(PlayerbotAI* ai) : GenericShamanStrategy(ai)
{
    actionNodeFactories.Add(new HealShamanStrategyActionNodeFactory());
}

void HealShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericShamanStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "earth shield on tank",
        NextAction::array(0, new NextAction("earth shield on party", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "shaman weapon",
        NextAction::array(0, new NextAction("earthliving weapon", 22.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "low mana",
        NextAction::array(0, new NextAction("mana tide totem", ACTION_EMERGENCY + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "party member to heal out of spell range",
        NextAction::array(0, new NextAction("reach party member to heal", ACTION_CRITICAL_HEAL + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "wrath of air totem",
        NextAction::array(0, new NextAction("wrath of air totem", 20.0f), NULL)));   

	//AOE heals
    triggers.push_back(new TriggerNode(
        "almost full aoe heal",
        NextAction::array(0, new NextAction("chain heal", ACTION_LIGHT_HEAL), NULL)));

    triggers.push_back(new TriggerNode(
        "medium aoe heal",
        NextAction::array(0, new NextAction("chain heal", ACTION_MEDIUM_HEAL), NULL)));

    triggers.push_back(new TriggerNode(
        "low aoe heal",
        NextAction::array(0, new NextAction("chain heal", ACTION_MEDIUM_HEAL + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "critical aoe heal",
        NextAction::array(0, new NextAction("chain heal", ACTION_CRITICAL_HEAL + 10), NULL)));

	//Group and Self Heals
	triggers.push_back(new TriggerNode(
		"almost full health",
		NextAction::array(0, new NextAction("lesser healing wave", 18.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"party member almost full health",
		NextAction::array(0, new NextAction("lesser healing wave on party", 17.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"medium health",
		NextAction::array(0, new NextAction("healing wave", 31.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"party member medium health",
		NextAction::array(0, new NextAction("healing wave on party", 30.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"low or critical health",
		NextAction::array(0, new NextAction("riptide", 101), NULL)));

	triggers.push_back(new TriggerNode(
		"party member low or critical health",
		NextAction::array(0, new NextAction("riptide on party", 100), NULL)));

	triggers.push_back(new TriggerNode(
		"low or critical health",
		NextAction::array(0, new NextAction("lesser healing wave", ACTION_CRITICAL_HEAL + 2), NULL)));

	triggers.push_back(new TriggerNode(
		"party member low or critical health",
		NextAction::array(0, new NextAction("lesser healing wave on party", ACTION_CRITICAL_HEAL + 1), NULL)));
}
