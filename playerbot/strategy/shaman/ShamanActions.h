#pragma once

#include "../actions/GenericActions.h"

namespace ai
{
	BUFF_ACTION(COEAction, "call of the elements");

    class CastLesserHealingWaveAction : public CastHealingSpellAction {
    public:
        CastLesserHealingWaveAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "lesser healing wave") {}
    };

    class CastLesserHealingWaveOnPartyAction : public HealPartyMemberAction
    {
    public:
        CastLesserHealingWaveOnPartyAction(PlayerbotAI* ai) : HealPartyMemberAction(ai, "lesser healing wave") {}
    };


    class CastHealingWaveAction : public CastHealingSpellAction {
    public:
        CastHealingWaveAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "healing wave") {}
    };

    class CastHealingWaveOnPartyAction : public HealPartyMemberAction
    {
    public:
        CastHealingWaveOnPartyAction(PlayerbotAI* ai) : HealPartyMemberAction(ai, "healing wave") {}
    };

    class CastChainHealAction : public CastAoeHealSpellAction {
    public:
        CastChainHealAction(PlayerbotAI* ai) : CastAoeHealSpellAction(ai, "chain heal") {}
    };

    class CastRiptideAction : public CastHealingSpellAction {
    public:
        CastRiptideAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "riptide") {}
    };

    class CastRiptideOnPartyAction : public HealPartyMemberAction
    {
    public:
        CastRiptideOnPartyAction(PlayerbotAI* ai) : HealPartyMemberAction(ai, "riptide") {}
    };


    class CastEarthShieldAction : public CastBuffSpellAction {
    public:
        CastEarthShieldAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "earth shield") {}
    };

	BUFF_TANK_ACTION(CastEarthShieldOnTankAction, "earth shield");

    class CastWaterShieldAction : public CastBuffSpellAction {
    public:
        CastWaterShieldAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "water shield") {}
    };

	class CastLightningShieldAction : public CastBuffSpellAction {
	public:
		CastLightningShieldAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "lightning shield") {}
	};

    class CastEarthlivingWeaponAction : public CastEnchantItemAction {
    public:
        CastEarthlivingWeaponAction(PlayerbotAI* ai) : CastEnchantItemAction(ai, "earthliving weapon") {}
    };

    class CastRockbiterWeaponAction : public CastEnchantItemAction {
    public:
        CastRockbiterWeaponAction(PlayerbotAI* ai) : CastEnchantItemAction(ai, "rockbiter weapon") {}
    };

    class CastFlametongueWeaponAction : public CastEnchantItemAction {
    public:
        CastFlametongueWeaponAction(PlayerbotAI* ai) : CastEnchantItemAction(ai, "flametongue weapon") {}
    };

    class CastFrostbrandWeaponAction : public CastEnchantItemAction {
    public:
        CastFrostbrandWeaponAction(PlayerbotAI* ai) : CastEnchantItemAction(ai, "frostbrand weapon") {}
    };

    class CastWindfuryWeaponAction : public CastEnchantItemAction {
    public:
        CastWindfuryWeaponAction(PlayerbotAI* ai) : CastEnchantItemAction(ai, "windfury weapon") {}
    };

    class CastTotemAction : public CastBuffSpellAction
    {
    public:
        CastTotemAction(PlayerbotAI* ai, string spell) : CastBuffSpellAction(ai, spell) {}
        virtual bool isUseful() { return CastBuffSpellAction::isUseful() && !AI_VALUE2(bool, "has totem", name); }

        bool HasFireDPSTotem()
        {
            return AI_VALUE2(bool, "has totem", "fire elemental totem") ||
                AI_VALUE2(bool, "has totem", "fire nova totem") ||
                AI_VALUE2(bool, "has totem", "searing totem") ||
                AI_VALUE2(bool, "has totem", "magma totem");
        }

    };

    class CastStoneskinTotemAction : public CastTotemAction
    {
    public:
        CastStoneskinTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "stoneskin totem") {}
    };

    class CastEarthbindTotemAction : public CastTotemAction
    {
    public:
        CastEarthbindTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "earthbind totem") {}
    };

    class CastStrengthOfEarthTotemAction : public CastTotemAction
    {
    public:
        CastStrengthOfEarthTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "strength of earth totem") {}
    };

	class CastTremorTotemAction : public CastTotemAction
	{
	public:
		CastTremorTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "tremor totem") {}
	};

    class CastManaSpringTotemAction : public CastTotemAction
    {
    public:
        CastManaSpringTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "mana spring totem") {}
		virtual bool isUseful() { return CastTotemAction::isUseful() && !AI_VALUE2(bool, "has totem", "cleansing totem") && !AI_VALUE2(bool, "has totem", "healing stream totem"); }
    };

	class CastManaTideTotemAction : public CastTotemAction
	{
	public:
		CastManaTideTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "mana tide totem") {}
		virtual string GetTargetName() { return "self target"; }
		virtual bool isUseful() { return CastTotemAction::isUseful() && !AI_VALUE2(bool, "has totem", "cleansing totem"); }
	};

	class CastHealingStreamTotemAction : public CastTotemAction
	{
	public:
		CastHealingStreamTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "healing stream totem") {}

		virtual bool isUseful() { return CastTotemAction::isUseful() && !AI_VALUE2(bool, "has totem", "cleansing totem"); }
	};

    class CastCleansingTotemAction : public CastTotemAction
    {
    public:
        CastCleansingTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "cleansing totem") {}
    };

    class CastFlametongueTotemAction : public CastTotemAction
    {
    public:
        CastFlametongueTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "flametongue totem") {}
        virtual bool isUseful() { return CastTotemAction::isUseful() && !CastTotemAction::HasFireDPSTotem(); }
    };

    class CastWindfuryTotemAction : public CastTotemAction
    {
    public:
        CastWindfuryTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "windfury totem") {}
    };   
    
    class CastWrathTotemAction : public CastTotemAction
    {
    public:
        CastWrathTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "totem of wrath") {}
        virtual bool isUseful() { return CastTotemAction::isUseful() && !CastTotemAction::HasFireDPSTotem(); }
    };

    class CastWrathOfAirTotemAction : public CastTotemAction
    {
    public:
        CastWrathOfAirTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "wrath of air totem") {}
    };

    class CastGraceOfAirTotemAction : public CastTotemAction
    {
    public:
        CastGraceOfAirTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "grace of air totem") {}
    };

    class CastSearingTotemAction : public CastTotemAction
    {
    public:
        CastSearingTotemAction(PlayerbotAI* ai) : CastTotemAction(ai, "searing totem") {}
        virtual string GetTargetName() { return "self target"; }
        virtual bool isUseful() { return CastTotemAction::isUseful() && !CastTotemAction::HasFireDPSTotem(); }
    };

    class CastMagmaTotemAction : public CastMeleeSpellAction
    {
    public:
        CastMagmaTotemAction(PlayerbotAI* ai) : CastMeleeSpellAction(ai, "magma totem") {}
        virtual string GetTargetName() { return "self target"; }
        virtual bool isUseful() { return CastMeleeSpellAction::isUseful() && !AI_VALUE2(bool, "has totem", name); }
    };

    class CastFireNovaTotemAction : public CastRangeSpellAction {
    public:
        CastFireNovaTotemAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "fire nova totem") {}
    };

    class CastWindShearAction : public CastRangeSpellAction {
    public:
        CastWindShearAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "wind shear") {}
    };

	class CastAncestralSpiritAction : public ResurrectPartyMemberAction
	{
	public:
		CastAncestralSpiritAction(PlayerbotAI* ai) : ResurrectPartyMemberAction(ai, "ancestral spirit") {}
	};

    class CastFeralSpiritAction : public CastSpellAction
    {
    public:
        CastFeralSpiritAction(PlayerbotAI* ai) : CastSpellAction(ai, "feral spirit") {}
    };

	class CastPurgeAction : public CastRangeSpellAction
	{
	public:
		CastPurgeAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "purge") {}
	};

    class CastShamanisticRageAction : public CastBuffSpellAction {
    public:
        CastShamanisticRageAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "shamanistic rage") {}
    };

	class CastStormstrikeAction : public CastMeleeSpellAction {
	public:
		CastStormstrikeAction(PlayerbotAI* ai) : CastMeleeSpellAction(ai, "stormstrike") {}
	};

	class CastLavaLashAction : public CastMeleeSpellAction {
	public:
		CastLavaLashAction(PlayerbotAI* ai) : CastMeleeSpellAction(ai, "lava lash") {}
	};

    class CastWaterBreathingAction : public CastBuffSpellAction {
    public:
        CastWaterBreathingAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "water breathing") {}
    };

    class CastWaterWalkingAction : public CastBuffSpellAction {
    public:
        CastWaterWalkingAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "water walking") {}
    };

    class CastWaterBreathingOnPartyAction : public BuffOnPartyAction {
    public:
        CastWaterBreathingOnPartyAction(PlayerbotAI* ai) : BuffOnPartyAction(ai, "water breathing") {}
    };

    class CastWaterWalkingOnPartyAction : public BuffOnPartyAction {
    public:
        CastWaterWalkingOnPartyAction(PlayerbotAI* ai) : BuffOnPartyAction(ai, "water walking") {}
    };


    class CastCleanseSpiritAction : public CastCureSpellAction {
    public:
        CastCleanseSpiritAction(PlayerbotAI* ai) : CastCureSpellAction(ai, "cleanse spirit") {}
    };

    class CastCleanseSpiritPoisonOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastCleanseSpiritPoisonOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "cleanse spirit", DISPEL_POISON) {}

        virtual string getName() { return "cleanse spirit poison on party"; }
    };
    class CastCleanseSpiritCurseOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastCleanseSpiritCurseOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "cleanse spirit", DISPEL_CURSE) {}

        virtual string getName() { return "cleanse spirit curse on party"; }
    };
    class CastCleanseSpiritDiseaseOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastCleanseSpiritDiseaseOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "cleanse spirit", DISPEL_DISEASE) {}

        virtual string getName() { return "cleanse spirit disease on party"; }
    };

    class CastFlameShockAction : public CastDebuffSpellAction
    {
    public:
        CastFlameShockAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "flame shock", true) {}
    };

	class CastLavaBurstAction : public CastRangeSpellAction
	{
	public:
		CastLavaBurstAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "lava burst") {}
	};

    class CastEarthShockAction : public CastDebuffSpellAction
    {
    public:
        CastEarthShockAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "earth shock", true) {}
    };

    class CastFrostShockAction : public CastSnareSpellAction
    {
    public:
        CastFrostShockAction(PlayerbotAI* ai) : CastSnareSpellAction(ai, "frost shock") {}
    };

    class CastChainLightningAction : public CastRangeSpellAction
    {
    public:
        CastChainLightningAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "chain lightning") {}
    };

    class CastLightningBoltAction : public CastRangeSpellAction
    {
    public:
        CastLightningBoltAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "lightning bolt") {}
    };

    class CastThunderstormAction : public CastSpellAction
    {
    public:
        CastThunderstormAction(PlayerbotAI* ai) : CastSpellAction(ai, "thunderstorm") {}
    };

    class CastHeroismAction : public CastBuffSpellAction
    {
    public:
        CastHeroismAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "heroism") {}
    };

    class CastBloodlustAction : public CastBuffSpellAction
    {
    public:
        CastBloodlustAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "bloodlust") {}
    };

    class CastWindShearOnEnemyHealerAction : public CastSpellOnEnemyHealerAction
    {
    public:
        CastWindShearOnEnemyHealerAction(PlayerbotAI* ai) : CastSpellOnEnemyHealerAction(ai, "wind shear") {}
    };

    class CastCurePoisonAction : public CastCureSpellAction
    {
    public:
        CastCurePoisonAction(PlayerbotAI* ai) : CastCureSpellAction(ai, "cure poison") {}
    };

    class CastCurePoisonOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastCurePoisonOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "cure poison", DISPEL_POISON) {}
    };

    class CastCureDiseaseAction : public CastCureSpellAction {
    public:
        CastCureDiseaseAction(PlayerbotAI* ai) : CastCureSpellAction(ai, "cure disease") {}
    };

    class CastCureDiseaseOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastCureDiseaseOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "cure disease", DISPEL_DISEASE) {}
        virtual string getName() { return "cure disease on party"; }
    };
}
