#include "botpch.h"
#include "../../playerbot.h"
#include "ShamanMultipliers.h"
#include "TotemsShamanStrategy.h"

using namespace ai;

TotemsShamanStrategy::TotemsShamanStrategy(PlayerbotAI* ai) : GenericShamanStrategy(ai)
{
}

void TotemsShamanStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericShamanStrategy::InitTriggers(triggers);


	triggers.push_back(new TriggerNode(
		"windfury totem",
		NextAction::array(0, new NextAction("windfury totem", 19.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "mana spring totem",
        NextAction::array(0, new NextAction("mana spring totem", 19.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "tremor totem",
        NextAction::array(0, new NextAction("tremor totem", 18.0f), NULL)));
}
