#include "botpch.h"
#include "../../playerbot.h"
#include "ShamanMultipliers.h"
#include "MeleeShamanStrategy.h"

using namespace ai;

class MeleeShamanStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    MeleeShamanStrategyActionNodeFactory()
    {
        creators["shamanistic rage"] = &shamanistic_rage;
        creators["magma totem"] = &magma_totem;
    }
private:

    static ActionNode* shamanistic_rage(PlayerbotAI* ai)
    {
        return new ActionNode("shamanistic rage",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("mana potion"), NULL),
            /*C*/ NULL);
    }

    static ActionNode* magma_totem(PlayerbotAI* ai)
    {
        return new ActionNode ("magma totem",
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NextAction::array(0, new NextAction("fire nova"), NULL));
    }
};

MeleeShamanStrategy::MeleeShamanStrategy(PlayerbotAI* ai) : GenericShamanStrategy(ai)
{
    actionNodeFactories.Add(new MeleeShamanStrategyActionNodeFactory());
}

NextAction** MeleeShamanStrategy::getDefaultActions()
{
	return NextAction::array(0, new NextAction("melee", ACTION_NORMAL), NULL);
}

void MeleeShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericShamanStrategy::InitTriggers(triggers);

	triggers.push_back(new TriggerNode(
		"stormstrike",
		NextAction::array(0, new NextAction("stormstrike", ACTION_HIGH + 2), NULL)));

	triggers.push_back(new TriggerNode(
		"lava lash",
		NextAction::array(0, new NextAction("lava lash", ACTION_HIGH + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "feral spirit",
        NextAction::array(0, new NextAction("feral spirit", ACTION_EMERGENCY + 5), NULL)));

	triggers.push_back(new TriggerNode(
		"instant chain lightning",
		NextAction::array(0, new NextAction("chain lightning", ACTION_EMERGENCY + 10), NULL)));	

    triggers.push_back(new TriggerNode(
        "shamanistic rage",
        NextAction::array(0, new NextAction("shamanistic rage", ACTION_EMERGENCY + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "shaman weapon",
        NextAction::array(0, new NextAction("windfury weapon", 22.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "searing totem",
        NextAction::array(0, new NextAction("reach melee", 22.0f), new NextAction("searing totem", 22.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "earth shock",
        NextAction::array(0, new NextAction("earth shock", 20.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "not facing target",
        NextAction::array(0, new NextAction("set facing", ACTION_NORMAL + 7), NULL)));

    triggers.push_back(new TriggerNode(
        "enemy too close for melee",
        NextAction::array(0, new NextAction("move out of enemy contact", ACTION_NORMAL + 8), NULL)));

	if (sRandomPlayerbotMgr.IsRandomBot(ai->GetBot()))
	{
		triggers.push_back(new TriggerNode(
			"windfury totem",
			NextAction::array(0, new NextAction("windfury totem", 19.0f), NULL)));

		triggers.push_back(new TriggerNode(
			"strength of earth totem",
			NextAction::array(0, new NextAction("strength of earth totem", 19.0f), NULL)));
	}
	else
	{
		triggers.push_back(new TriggerNode(
			"windfury totem",
			NextAction::array(0, new NextAction("call of the elements", 50), NULL)));
	}

#ifndef MANGOSBOT_TWO
	triggers.push_back(new TriggerNode(
		"shock",
		NextAction::array(0, new NextAction("earth shock", 200), NULL)));
#else
	triggers.push_back(new TriggerNode(
		"shock",
		NextAction::array(0, new NextAction("wind shear", 200), NULL)));
#endif
}

void MeleeAoeShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "enemy out of melee",
        NextAction::array(0, new NextAction("reach melee", ACTION_NORMAL + 8), NULL)));

    triggers.push_back(new TriggerNode(
        "magma totem",
        NextAction::array(0, new NextAction("magma totem", 26.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "medium aoe",
        NextAction::array(0, new NextAction("fire nova", 25.0f), NULL)));
}
