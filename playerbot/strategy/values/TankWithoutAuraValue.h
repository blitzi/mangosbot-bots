#pragma once
#include "../Value.h"
#include "PartyMemberWithoutAuraValue.h"
#include "../../PlayerbotAIConfig.h"

namespace ai
{
    class TankWithoutAuraValue : public PartyMemberWithoutAuraValue
	{
	public:
        TankWithoutAuraValue(PlayerbotAI* ai, bool onlyMainTank, float range = sPlayerbotAIConfig.sightDistance) :
            PartyMemberWithoutAuraValue(ai), onlyMainTank(onlyMainTank) {}

    protected:
        virtual Unit* Calculate();

        bool onlyMainTank = false;
	};
}
