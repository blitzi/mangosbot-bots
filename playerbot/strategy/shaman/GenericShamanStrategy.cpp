#include "botpch.h"
#include "../../playerbot.h"
#include "ShamanMultipliers.h"
#include "HealShamanStrategy.h"

using namespace ai;

class GenericShamanStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericShamanStrategyActionNodeFactory()
    {
        creators["flametongue weapon"] = &flametongue_weapon;
        creators["frostbrand weapon"] = &frostbrand_weapon;
        creators["windfury weapon"] = &windfury_weapon;
        creators["lesser healing wave"] = &lesser_healing_wave;
        creators["lesser healing wave on party"] = &lesser_healing_wave_on_party;
        creators["chain heal"] = &chain_heal;
        creators["riptide"] = &riptide;
        creators["chain heal on party"] = &chain_heal_on_party;
        creators["riptide on party"] = &riptide_on_party;
        creators["earth shock"] = &earth_shock;
    }
private:
    static ActionNode* earth_shock(PlayerbotAI* ai)
    {
        return new ActionNode ("earth shock",
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* flametongue_weapon(PlayerbotAI* ai)
    {
        return new ActionNode ("flametongue weapon",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("frostbrand weapon"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* frostbrand_weapon(PlayerbotAI* ai)
    {
        return new ActionNode ("frostbrand weapon",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("rockbiter weapon"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* windfury_weapon(PlayerbotAI* ai)
    {
        return new ActionNode ("windfury weapon",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("rockbiter weapon"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* lesser_healing_wave(PlayerbotAI* ai)
    {
        return new ActionNode ("lesser healing wave",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("healing wave"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* lesser_healing_wave_on_party(PlayerbotAI* ai)
    {
        return new ActionNode ("lesser healing wave on party",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("healing wave on party"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* chain_heal(PlayerbotAI* ai)
    {
        return new ActionNode ("chain heal",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("lesser healing wave"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* riptide(PlayerbotAI* ai)
    {
        return new ActionNode ("riptide",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("healing wave"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* chain_heal_on_party(PlayerbotAI* ai)
    {
        return new ActionNode ("chain heal on party",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("lesser healing wave on party"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* riptide_on_party(PlayerbotAI* ai)
    {
        return new ActionNode ("riptide on party",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("healing wave on party"), NULL),
            /*C*/ NULL);
    }
};

GenericShamanStrategy::GenericShamanStrategy(PlayerbotAI* ai) : CombatStrategy(ai)
{
    actionNodeFactories.Add(new GenericShamanStrategyActionNodeFactory());
}

void GenericShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    CombatStrategy::InitTriggers(triggers);

	triggers.push_back(new TriggerNode(
        "purge",
		NextAction::array(0, new NextAction("purge", ACTION_DISPEL), NULL)));

    triggers.push_back(new TriggerNode(
        "low health",
        NextAction::array(0, new NextAction("healing wave", ACTION_CRITICAL_HEAL + 2), NULL)));

    triggers.push_back(new TriggerNode(
        "party member low health",
        NextAction::array(0, new NextAction("healing wave on party", ACTION_CRITICAL_HEAL + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "critical health",
        NextAction::array(0, new NextAction("lesser healing wave", ACTION_CRITICAL_HEAL + 4), NULL)));

    triggers.push_back(new TriggerNode(
        "party member critical health",
        NextAction::array(0, new NextAction("lesser healing wave party", ACTION_CRITICAL_HEAL + 3), NULL)));

    triggers.push_back(new TriggerNode(
        "critical aoe heal",
        NextAction::array(0, new NextAction("chain heal", ACTION_CRITICAL_HEAL), NULL)));

	triggers.push_back(new TriggerNode(
		"heroism",
		NextAction::array(0, new NextAction("heroism", 31.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"bloodlust",
		NextAction::array(0, new NextAction("bloodlust", 30.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "cure poison",
        NextAction::array(0, new NextAction("cure poison", 21.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cure poison",
        NextAction::array(0, new NextAction("cure poison on party", 21.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "cure disease",
        NextAction::array(0, new NextAction("cure disease", 31.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cure disease",
        NextAction::array(0, new NextAction("cure disease on party", 30.0f), NULL)));
}

void ShamanBuffDpsStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "water shield",
        NextAction::array(0, new NextAction("water shield", 22.0f), NULL)));
}

void ShamanBuffManaStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "water shield",
        NextAction::array(0, new NextAction("water shield", 22.0f), NULL)));
}

void ShamanCureStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "cure poison",
        NextAction::array(0, new NextAction("cure poison", 21.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cure poison",
        NextAction::array(0, new NextAction("cure poison on party", 21.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "cleanse spirit poison",
        NextAction::array(0, new NextAction("cleanse spirit", 24.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cleanse spirit poison",
        NextAction::array(0, new NextAction("cleanse spirit poison on party", 23.0f), NULL)));


    triggers.push_back(new TriggerNode(
        "cure disease",
        NextAction::array(0, new NextAction("cure disease", 31.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cure disease",
        NextAction::array(0, new NextAction("cure disease on party", 30.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "cleanse spirit disease",
        NextAction::array(0, new NextAction("cleanse spirit", 24.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cleanse spirit disease",
        NextAction::array(0, new NextAction("cleanse spirit disease on party", 23.0f), NULL)));


    triggers.push_back(new TriggerNode(
        "cleanse spirit curse",
        NextAction::array(0, new NextAction("cleanse spirit", 24.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "party member cleanse spirit curse",
        NextAction::array(0, new NextAction("cleanse spirit curse on party", 23.0f), NULL)));

}
