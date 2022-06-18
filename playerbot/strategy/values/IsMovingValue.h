#pragma once
#include "../Value.h"
#include "MotionGenerators/TargetedMovementGenerator.h"

namespace ai
{
    class IsMovingValue : public BoolCalculatedValue, public Qualified
	{
	public:
        IsMovingValue(PlayerbotAI* ai) : BoolCalculatedValue(ai) {}

        virtual bool Calculate()
        {
            Unit* target = AI_VALUE(Unit*, qualifier);

            if (!target)
                return false;

            return sServerFacade.isMoving(target);
        }
    };

    class IsSwimmingValue : public BoolCalculatedValue, public Qualified
	{
	public:
        IsSwimmingValue(PlayerbotAI* ai) : BoolCalculatedValue(ai) {}

        virtual bool Calculate()
        {
            Unit* target = AI_VALUE(Unit*, qualifier);

            if (!target)
                return false;

            return sServerFacade.IsUnderwater(target) || target->IsInSwimmableWater();
        }
    };

	class GoTargetReachedValue : public BoolCalculatedValue
	{
	public:
		GoTargetReachedValue(PlayerbotAI* ai) : BoolCalculatedValue(ai) {}

		virtual bool Calculate()
		{
			Unit* target = AI_VALUE(Unit*, "go target");

			if (target == NULL)
				return true;

			return sServerFacade.IsDistanceLessOrEqualThan(AI_VALUE2(float, "distance", "go target"), CONTACT_DISTANCE);
		}
	};
}
