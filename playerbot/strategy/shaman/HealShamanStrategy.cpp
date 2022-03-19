#include "botpch.h"
#include "../../playerbot.h"
#include "ShamanMultipliers.h"
#include "HealShamanStrategy.h"
#include "../actions/ActionUtils.h"

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

	AddGroupSpell(triggers, "cleanse spirit", 21.0f);

	triggers.push_back(TRIGGER_AND_CAST("purge", ACTION_DISPEL));

    triggers.push_back(new TriggerNode(
        "shaman weapon",
        NextAction::array(0, new NextAction("earthliving weapon", 22.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "earth shield on tank",
        NextAction::array(0, new NextAction("earth shield on party", ACTION_HIGH), NULL)));

    triggers.push_back(new TriggerNode(
        "low mana",
        NextAction::array(0, new NextAction("mana tide totem", ACTION_EMERGENCY + 5), NULL)));

    triggers.push_back(new TriggerNode(
        "wrath of air totem",
        NextAction::array(0, new NextAction("wrath of air totem", 20.0f), NULL)));   
}
