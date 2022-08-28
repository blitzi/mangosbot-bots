#pragma once
#include "../Value.h"
#include "RtiTargetValue.h"
#include "TargetValue.h"

namespace ai
{
    class DpsTargetValue : public RtiTargetValue
	{
	public:
        DpsTargetValue(PlayerbotAI* ai, string type = "rti", string name = "dps target") : RtiTargetValue(ai, type, name) {}

    public:
        Unit* Calculate();
    };

	class StarTargetValue : public RtiTargetValue
	{
	public:
		StarTargetValue(PlayerbotAI* ai) : RtiTargetValue(ai) {}

	public:
		Unit* Calculate();
	};

    class DpsAoeTargetValue : public RtiTargetValue
    {
    public:
        DpsAoeTargetValue(PlayerbotAI* ai, string type = "rti", string name = "dps aoe target") : RtiTargetValue(ai, type, name) {}

    public:
        Unit* Calculate();
    };

    class DamageStopValue : public TargetValue
    {
    public:
        DamageStopValue(PlayerbotAI* ai) : TargetValue(ai) {}

    protected:
        Unit* Calculate();
    };
}
