#include "botpch.h"
#include "../../playerbot.h"
#include "ShamanMultipliers.h"
#include "HealShamanStrategy.h"
#include "../AiObject.h"
#include "../actions/ActionUtils.h"

using namespace ai;

class GenericShamanStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
#define CREATE(name) creators[name] = 

public:
    GenericShamanStrategyActionNodeFactory()
    {
        creators["flametongue weapon"] = &flametongue_weapon;
        creators["frostbrand weapon"] = &frostbrand_weapon;
        creators["windfury weapon"] = &windfury_weapon;
    }
private:

	ACTION_NODE_A(flametongue_weapon, "flametongue weapon", "frostbrand weapon");
	ACTION_NODE_A(frostbrand_weapon, "frostbrand weapon", "rockbiter weapon");
	ACTION_NODE_A(windfury_weapon, "windfury weapon", "rockbiter weapon");
};

GenericShamanStrategy::GenericShamanStrategy(PlayerbotAI* ai) : CombatStrategy(ai)
{
    actionNodeFactories.Add(new GenericShamanStrategyActionNodeFactory());
}

void GenericShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    CombatStrategy::InitTriggers(triggers);

	triggers.push_back(TRIGGER_AND_CAST("purge", ACTION_DISPEL));
	triggers.push_back(TRIGGER_AND_CAST("heroism", 31.0f));
	triggers.push_back(TRIGGER_AND_CAST("bloodlust", 31.0f));
	triggers.push_back(TRIGGER_AND_CAST("mana spring totem", 19.0f));

	FillAOEHealPipeline(triggers, "chain heal on party");
	FillHealPipeline(triggers, "lesser healing wave", "riptide", "healing wave");
}

void ShamanBuffDpsStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	triggers.push_back(TRIGGER_AND_CAST("lightning shield", 22.0f));
}

void ShamanBuffManaStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	triggers.push_back(TRIGGER_AND_CAST("water shield", 22.0f));
}

void ShamanCureStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	AddGroupSpell(triggers, "cure poison", "cleanse spirit", 21);
	AddGroupSpell(triggers, "cleanse spirit curse", "cleanse spirit", 23);
	AddGroupSpell(triggers, "cure disease", "cleanse spirit", 31);
}
