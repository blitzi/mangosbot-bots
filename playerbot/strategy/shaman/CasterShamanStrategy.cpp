#include "botpch.h"
#include "../../playerbot.h"
#include "ShamanMultipliers.h"
#include "CasterShamanStrategy.h"

using namespace ai;

class CasterShamanStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    CasterShamanStrategyActionNodeFactory()
    {
        creators["magma totem"] = &magma_totem;
    }
private:
    static ActionNode* magma_totem(PlayerbotAI* ai)
    {
        return new ActionNode ("magma totem",
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NextAction::array(0, new NextAction("fire nova"), NULL));
    }
};

CasterShamanStrategy::CasterShamanStrategy(PlayerbotAI* ai) : GenericShamanStrategy(ai)
{
    actionNodeFactories.Add(new CasterShamanStrategyActionNodeFactory());
}

void CasterShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericShamanStrategy::InitTriggers(triggers);

	triggers.push_back(new TriggerNode(
		"enemy out of spell",
		NextAction::array(0, new NextAction("reach spell", ACTION_NORMAL + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "shaman weapon",
        NextAction::array(0, new NextAction("flametongue weapon", 23.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"crit lava burst",
		NextAction::array(0, new NextAction("lava burst", ACTION_EMERGENCY + 11), NULL)));

    triggers.push_back(new TriggerNode(
        "flame shock",
        NextAction::array(0, new NextAction("flame shock", 24), NULL)));

	triggers.push_back(new TriggerNode(
		"medium aoe",
		NextAction::array(0, new NextAction("fire nova", 25.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "tremor totem",
        NextAction::array(0, new NextAction("tremor totem", 19.0f), NULL)));     

	triggers.push_back(new TriggerNode(
		"totem of wrath",
		NextAction::array(0, new NextAction("totem of wrath", 24.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "lightning bolt",
        NextAction::array(0, new NextAction("lightning bolt", 15.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "clearcasting",
        NextAction::array(0, new NextAction("chain lightning", 30), NULL)));
	
	triggers.push_back(new TriggerNode(
        "elemental mastery",
        NextAction::array(0, new NextAction("elemental mastery", 31.0f), NULL)));
	
	triggers.push_back(new TriggerNode(
        "high mana",
        NextAction::array(0, new NextAction("thunderstorm", 31.0f), NULL)));

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

void CasterAoeShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "light aoe",
        NextAction::array(0, new NextAction("chain lightning", 26.0f), NULL)));
}
