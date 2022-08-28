#pragma once
#include "../Value.h"

class Unit;

namespace ai
{
    class HealthValue : public FloatCalculatedValue, public Qualified
    {
    public:
        HealthValue(PlayerbotAI* ai, string name = "health") : FloatCalculatedValue(ai) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual float Calculate();
    };

    class IsDeadValue : public BoolCalculatedValue, public Qualified
    {
    public:
        IsDeadValue(PlayerbotAI* ai, string name = "dead") : BoolCalculatedValue(ai, name) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual bool Calculate();
    };

    class PetIsDeadValue : public BoolCalculatedValue
    {
    public:
        PetIsDeadValue(PlayerbotAI* ai, string name = "pet dead") : BoolCalculatedValue(ai, name) {}
        virtual bool Calculate();
    };

    class PetIsHappyValue : public BoolCalculatedValue
    {
    public:
        PetIsHappyValue(PlayerbotAI* ai, string name = "pet happy") : BoolCalculatedValue(ai, name) {}
        virtual bool Calculate();
    };

    class RageValue : public Uint32CalculatedValue, public Qualified
    {
    public:
        RageValue(PlayerbotAI* ai, string name = "rage") : Uint32CalculatedValue(ai) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual uint32 Calculate();
    };

    class EnergyValue : public Uint32CalculatedValue, public Qualified
    {
    public:
        EnergyValue(PlayerbotAI* ai, string name = "energy") : Uint32CalculatedValue(ai) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual uint32 Calculate();
    };

    class ManaValue : public FloatCalculatedValue, public Qualified
    {
    public:
        ManaValue(PlayerbotAI* ai, string name = "mana") : FloatCalculatedValue(ai) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual float Calculate();
    };

    class HasManaValue : public BoolCalculatedValue, public Qualified
    {
    public:
        HasManaValue(PlayerbotAI* ai, string name = "has mana") : BoolCalculatedValue(ai, name) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual bool Calculate();
    };

    class ComboPointsValue : public Uint8CalculatedValue, public Qualified
    {
    public:
        ComboPointsValue(PlayerbotAI* ai, string name = "combo points") : Uint8CalculatedValue(ai, name) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual uint8 Calculate();
    };

    class IsMountedValue : public BoolCalculatedValue, public Qualified
    {
    public:
        IsMountedValue(PlayerbotAI* ai, string name = "mounted") : BoolCalculatedValue(ai, name) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual bool Calculate();
    };

    class IsInCombatValue : public MemoryCalculatedValue<bool>, public Qualified
    {
    public:
        IsInCombatValue(PlayerbotAI* ai, string name = "combat") : MemoryCalculatedValue(ai, name) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }

        virtual bool EqualToLast(bool value) { return value == lastValue; }

        virtual bool Calculate() ;
    };

    class BagSpaceValue : public Uint8CalculatedValue
    {
    public:
        BagSpaceValue(PlayerbotAI* ai, string name = "bag space") : Uint8CalculatedValue(ai, name) {}

        virtual uint8 Calculate();
    };

    class DurabilityValue : public FloatCalculatedValue
    {
    public:
        DurabilityValue(PlayerbotAI* ai, string name = "durability") : FloatCalculatedValue(ai) {}

        virtual float Calculate();
    };

     class SpeedValue : public Uint8CalculatedValue, public Qualified
    {
    public:
        SpeedValue(PlayerbotAI* ai, string name = "speed") : Uint8CalculatedValue(ai, name) {}

        Unit* GetTarget()
        {
            AiObjectContext* ctx = AiObject::context;
            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual uint8 Calculate();
    };

    class IsInGroupValue : public BoolCalculatedValue
    {
    public:
        IsInGroupValue(PlayerbotAI* ai, string name = "in group") : BoolCalculatedValue(ai, name) {}

        virtual bool Calculate() { return bot->GetGroup(); }
    };

    class DeathCountValue : public ManualSetValue<uint32>
    {
    public:
        DeathCountValue(PlayerbotAI* ai, string name = "death count") : ManualSetValue<uint32>(ai, 0, name) {}
    };


    class ExperienceValue : public MemoryCalculatedValue<uint32>
    {
    public:
        ExperienceValue(PlayerbotAI* ai, string name = "experience", uint32 checkInterval = 60) : MemoryCalculatedValue<uint32>(ai, name, checkInterval) {}

        virtual bool EqualToLast(uint32 value) { return value != lastValue; }

        virtual uint32 Calculate() { return bot->GetUInt32Value(PLAYER_XP);}
    };
}
