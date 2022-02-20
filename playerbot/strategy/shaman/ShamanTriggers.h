#pragma once
#include "../triggers/GenericTriggers.h"

namespace ai
{
    class ShamanWeaponTrigger : public BuffTrigger {
    public:
        ShamanWeaponTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "rockbiter weapon") {}
        virtual bool IsActive();
    private:
        static list<string> spells;
    };

    class TotemTrigger : public Trigger {
    public:
        TotemTrigger(PlayerbotAI* ai, string spell, int attackerCount = 0) : Trigger(ai, spell), attackerCount(attackerCount) {}

        virtual bool IsActive()
		{
            return AI_VALUE(uint8, "attacker count") >= attackerCount && !AI_VALUE2(bool, "has totem", name);
        }

    protected:
        int attackerCount;
    };

    class WrathTotemTrigger : public TotemTrigger {
    public:
        WrathTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "totem of wrath") {}
    };

    class WindfuryTotemTrigger : public TotemTrigger {
    public:
        WindfuryTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "windfury totem") {}
		virtual bool IsActive()
		{
			return TotemTrigger::IsActive() && !AI_VALUE2(bool, "has totem", "grounding totem");
		}
    };

    class GraceOfAirTotemTrigger : public TotemTrigger
    {
    public:
        GraceOfAirTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "grace of air totem") {}
		virtual bool IsActive()
		{
			return TotemTrigger::IsActive() && !AI_VALUE2(bool, "has totem", "grounding totem");
		}
    };

    class WrathOfAirTotemTrigger : public TotemTrigger
    {
    public:
        WrathOfAirTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "wrath of air totem") {}
		virtual bool IsActive()
		{
			return TotemTrigger::IsActive() && !AI_VALUE2(bool, "has totem", "grounding totem");
		}
    };

    class ManaSpringTotemTrigger : public TotemTrigger {
    public:
        ManaSpringTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "mana spring totem") {}
        virtual bool IsActive()
        {
			return TotemTrigger::IsActive() && !AI_VALUE2(bool, "has totem", "mana tide totem");
                    
        }
    };

    class FlametongueTotemTrigger : public TotemTrigger {
    public:
        FlametongueTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "flametongue totem") {}
    };

    class StrengthOfEarthTotemTrigger : public TotemTrigger {
    public:
        StrengthOfEarthTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "strength of earth totem") {}
    };

	class TremorTotemTrigger : public TotemTrigger {
	public:
		TremorTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "tremor totem") {}
	};

    class MagmaTotemTrigger : public TotemTrigger {
    public:
        MagmaTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "magma totem", 3) {}
    };

    class SearingTotemTrigger : public TotemTrigger {
    public:
        SearingTotemTrigger(PlayerbotAI* ai) : TotemTrigger(ai, "searing totem", 1) {}

        virtual bool IsActive()
        {
            bool isDungeon = ai->GetBot()->GetMap()->IsDungeon();

            return !isDungeon && TotemTrigger::IsActive();
        }
    };

    class WindShearInterruptSpellTrigger : public InterruptSpellTrigger
    {
    public:
        WindShearInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "wind shear") {}
    };

    class WaterShieldTrigger : public BuffTrigger
    {
    public:
        WaterShieldTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "water shield") {}
    };

    class ClearCastingTrigger : public HasAuraTrigger
    {
    public:
        ClearCastingTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "clearcasting") {}
    };


    class ShamanisticRageTrigger : public BuffTrigger
    {
    public:
        ShamanisticRageTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "shamanistic rage") {}

        virtual bool IsActive()
        {
            return 
                AI_VALUE(uint8, "attacker count") >= 1 &&
                AI_VALUE2(bool, "has mana", "self target") &&
                AI_VALUE2(uint8, "mana", "self target") < sPlayerbotAIConfig.lowMana &&
                BuffTrigger::IsActive();
        }
    };

    class LightningShieldTrigger : public BuffTrigger
    {
    public:
        LightningShieldTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "lightning shield") {}
    };

    class PurgeTrigger : public TargetAuraDispelTrigger
    {
    public:
        PurgeTrigger(PlayerbotAI* ai) : TargetAuraDispelTrigger(ai, "purge", DISPEL_MAGIC) {}
    };

    class WaterWalkingTrigger : public BuffTrigger {
    public:
        WaterWalkingTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "water walking", 7) {}

        virtual bool IsActive()
        {
            return BuffTrigger::IsActive() && AI_VALUE2(bool, "swimming", "self target");
        }
    };

    class WaterBreathingTrigger : public BuffTrigger {
    public:
        WaterBreathingTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "water breathing", 5) {}

        virtual bool IsActive()
        {
            return BuffTrigger::IsActive() && AI_VALUE2(bool, "swimming", "self target");
        }
    };

    class WaterWalkingOnPartyTrigger : public BuffOnPartyTrigger {
    public:
        WaterWalkingOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "water walking on party", 7) {}

        virtual bool IsActive()
        {
            return BuffOnPartyTrigger::IsActive() && AI_VALUE2(bool, "swimming", "self target");
        }
    };

    class WaterBreathingOnPartyTrigger : public BuffOnPartyTrigger {
    public:
        WaterBreathingOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "water breathing on party", 2) {}

        virtual bool IsActive()
        {
            return BuffOnPartyTrigger::IsActive() && AI_VALUE2(bool, "swimming", "self target");
        }
    };

    class CleanseSpiritPoisonTrigger : public NeedCureTrigger
    {
    public:
        CleanseSpiritPoisonTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cleanse spirit", DISPEL_POISON) {}
    };

    class PartyMemberCleanseSpiritPoisonTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        PartyMemberCleanseSpiritPoisonTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cleanse spirit", DISPEL_POISON) {}
    };

    class CleanseSpiritCurseTrigger : public NeedCureTrigger
    {
    public:
        CleanseSpiritCurseTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cleanse spirit", DISPEL_CURSE) {}
    };

    class PartyMemberCleanseSpiritCurseTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        PartyMemberCleanseSpiritCurseTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cleanse spirit", DISPEL_CURSE) {}
    };

    class CleanseSpiritDiseaseTrigger : public NeedCureTrigger
    {
    public:
        CleanseSpiritDiseaseTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cleanse spirit", DISPEL_DISEASE) {}
    };

    class PartyMemberCleanseSpiritDiseaseTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        PartyMemberCleanseSpiritDiseaseTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cleanse spirit", DISPEL_DISEASE) {}
    };

#ifndef MANGOSBOT_TWO
    class ShockTrigger : public InterruptSpellTrigger {
    public:
        ShockTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "earth shock") {}
    };
#else
	class ShockTrigger : public InterruptSpellTrigger {
	public:
		ShockTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "wind shear") {}
	};
#endif

    class FrostShockSnareTrigger : public SnareTargetTrigger {
    public:
        FrostShockSnareTrigger(PlayerbotAI* ai) : SnareTargetTrigger(ai, "frost shock") {}
    };

    MY_DEBUFF_TRIGGER(FlameshockTrigger, "flame shock");
	DEBUFF_IMMEDIATE_TRIGGER(EarthshockTrigger, "earth shock");

    class HeroismTrigger : public BoostBuffTrigger
    {
    public:
        HeroismTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "heroism") {}
    };

    class FeralSpiritTrigger : public BoostCastTrigger
    {
    public:
        FeralSpiritTrigger(PlayerbotAI* ai) : BoostCastTrigger(ai, "feral spirit", 10.0f) {}
    };

    class BloodlustTrigger : public BoostBuffTrigger
    {
    public:
        BloodlustTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "bloodlust") {}
    };

    class WindShearInterruptEnemyHealerSpellTrigger : public InterruptEnemyHealerTrigger
    {
    public:
        WindShearInterruptEnemyHealerSpellTrigger(PlayerbotAI* ai) : InterruptEnemyHealerTrigger(ai, "wind shear") {}
    };

    class CurePoisonTrigger : public NeedCureTrigger
    {
    public:
        CurePoisonTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cure poison", DISPEL_POISON) {}
    };

    class PartyMemberCurePoisonTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        PartyMemberCurePoisonTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cure poison", DISPEL_POISON) {}
    };

    class CureDiseaseTrigger : public NeedCureTrigger
    {
    public:
        CureDiseaseTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cure disease", DISPEL_DISEASE) {}
    };

    class PartyMemberCureDiseaseTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        PartyMemberCureDiseaseTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cure disease", DISPEL_DISEASE) {}
    };

    class EarthShieldOnTankTrigger : public BuffOnTankTrigger {
    public:
        EarthShieldOnTankTrigger(PlayerbotAI* ai) : BuffOnTankTrigger(ai, "earth shield", 1) {}

        virtual bool IsActive() {
            return BuffOnTankTrigger::IsActive() &&
                GetTarget() &&
                !ai->HasAura("earth shield", GetTarget()) &&
#ifdef MANGOS
                (ai->GetBot()->IsInSameGroupWith((Player*)GetTarget()) || ai->GetBot()->IsInSameRaidWith((Player*)GetTarget())) &&
#endif
#ifdef CMANGOS
                (ai->GetBot()->IsInGroup((Player*)GetTarget(), true) || ai->GetBot()->IsInGroup((Player*)GetTarget()))
#endif               
                ;
        }
    };
}
