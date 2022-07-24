#pragma once

#include "../generic/NonCombatStrategy.h"

namespace ai
{
    class GenericWarlockNonCombatStrategy : public NonCombatStrategy
    {
    public:
        GenericWarlockNonCombatStrategy(PlayerbotAI* ai);
        virtual string getName() { return "nc"; }

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
    };

    class WarlockPetStrategy : public Strategy
    {
    private:
        string bestPet;
    public:
        WarlockPetStrategy(PlayerbotAI* ai, string pet) : Strategy(ai)
        {
            bestPet = "summon " + pet;
        }
        virtual void InitTriggers(std::list<TriggerNode*>& triggers);
    };

    class AfflictionWarlockPetStrategy : public WarlockPetStrategy
    {
    public:
        AfflictionWarlockPetStrategy(PlayerbotAI* ai) : WarlockPetStrategy(ai, "imp") {}
        virtual string getName() { return "pet"; }
    };

    class DeamonWarlockPetStrategy : public WarlockPetStrategy
    {
    public:
        DeamonWarlockPetStrategy(PlayerbotAI* ai) : WarlockPetStrategy(ai, "felguard") {}
        virtual string getName() { return "pet_deamon"; }
    };

    class DestroWarlockPetStrategy : public WarlockPetStrategy
    {
    public:
        DestroWarlockPetStrategy(PlayerbotAI* ai) : WarlockPetStrategy(ai, "succubus") {}
        virtual string getName() { return "pet_destro"; }
    };
}
