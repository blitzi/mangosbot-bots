#pragma once
#include "../Value.h"
#include "PartyMemberValue.h"

namespace ai
{
    class PartyMemberToHeal : public PartyMemberValue
	{
	public:
        PartyMemberToHeal(PlayerbotAI* ai) : 
          PartyMemberValue(ai) {}
    
    protected:
        virtual Unit* Calculate();
        bool CanHealPet(Pet* pet);
        virtual bool Check(Unit* player);
	};

    class PartyMemberToProtect : public PartyMemberValue
    {
    public:
        PartyMemberToProtect(PlayerbotAI* ai) :
            PartyMemberValue(ai) {}

    protected:
        virtual Unit* Calculate();
    };
}
