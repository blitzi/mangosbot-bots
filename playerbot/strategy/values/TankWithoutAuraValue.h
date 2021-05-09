#pragma once
#include "../Value.h"
#include "PartyMemberWithoutAuraValue.h"
#include "../../PlayerbotAIConfig.h"

namespace ai
{
    class TankWithoutAuraValue : public PartyMemberWithoutAuraValue
	{
	public:
        TankWithoutAuraValue(PlayerbotAI* ai, float range = sPlayerbotAIConfig.sightDistance) :
            PartyMemberWithoutAuraValue(ai) {}

    protected:
        virtual Unit* Calculate();
	};
}
