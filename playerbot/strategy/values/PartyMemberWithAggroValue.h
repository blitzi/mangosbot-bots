#pragma once
#include "../Value.h"
#include "PartyMemberValue.h"

namespace ai
{
    class PartyMemberWithAggroValue : public PartyMemberValue, public Qualified
	{
	public:
        PartyMemberWithAggroValue(PlayerbotAI* ai) :
            PartyMemberValue(ai), Qualified() {}

    protected:
        virtual Unit* Calculate();
	};
}
