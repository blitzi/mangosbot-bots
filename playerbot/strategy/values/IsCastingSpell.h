#pragma once
#include "../Value.h"

namespace ai
{
    class IsCastingSpell : public CalculatedValue<bool>
    {
    public:
        IsCastingSpell(PlayerbotAI* ai) : CalculatedValue<bool>(ai) {}

        virtual bool Calculate();
    };
}
