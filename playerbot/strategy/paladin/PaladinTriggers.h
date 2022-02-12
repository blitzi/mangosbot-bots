#pragma once
#include "../triggers/GenericTriggers.h"

namespace ai
{
    BUFF_TRIGGER(HolyShieldTrigger, "holy shield");
    BUFF_TRIGGER(RighteousFuryTrigger, "righteous fury");

    BUFF_TRIGGER(RetributionAuraTrigger, "retribution aura");
    BUFF_TRIGGER(SanctityAuraTrigger, "sanctity aura");

	class CrusaderAuraTrigger : public BuffTrigger
	{
	public:
		CrusaderAuraTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "crusader aura") {}
		virtual bool IsActive();
	};

	class SealTrigger : public BuffTrigger
	{
	public:
		SealTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "any seal placeholder") {}
		virtual bool IsActive();
	};

    class SealOfWisdomTrigger : public SealTrigger
    {
    public:
        SealOfWisdomTrigger(PlayerbotAI* ai) : SealTrigger(ai) {}
        virtual bool IsActive();
    };

    DEBUFF_IMMEDIATE_TRIGGER(JudgementOfLightTrigger, "judgement of light");
    DEBUFF_IMMEDIATE_TRIGGER(JudgementOfWisdomTrigger, "judgement of wisdom");
    DEBUFF_TRIGGER(JudgementTrigger, "judgement");

    class BlessingOfSalvationOnPartyTrigger : public BuffOnPartyTrigger
    {
    public:
        BlessingOfSalvationOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "blessing of salvation", 1) {}
    };

    class BlessingOnPartyTrigger : public BuffOnPartyTrigger
    {
    public:
        BlessingOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "blessing of might,blessing of wisdom", 1) {}
    };

	class BlessingOfKingsOnPartyTrigger : public BuffOnPartyTrigger
	{
	public:
		BlessingOfKingsOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "blessing of kings", 1) {}
	};

    class BlessingTrigger : public BuffTrigger
    {
    public:
        BlessingTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "blessing of sanctuary", 1) {}
        virtual bool IsActive();
    };

	class BlessingOfKingsTrigger : public BuffTrigger
	{
	public:
		BlessingOfKingsTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "blessing of kings", 1) {}
	};

    class HammerOfJusticeInterruptSpellTrigger : public InterruptSpellTrigger
    {
    public:
        HammerOfJusticeInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "hammer of justice") {}
    };

    class HammerOfJusticeSnareTrigger : public SnareTargetTrigger
    {
    public:
        HammerOfJusticeSnareTrigger(PlayerbotAI* ai) : SnareTargetTrigger(ai, "hammer of justice") {}
    };

    class ArtOfWarTrigger : public HasAuraTrigger
    {
    public:
        ArtOfWarTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "the art of war") {}
    };

    class ShadowResistanceAuraTrigger : public BuffTrigger
    {
    public:
        ShadowResistanceAuraTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "shadow resistance aura") {}
    };

    class FrostResistanceAuraTrigger : public BuffTrigger
    {
    public:
        FrostResistanceAuraTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "frost resistance aura") {}
    };

    class FireResistanceAuraTrigger : public BuffTrigger
    {
    public:
        FireResistanceAuraTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "fire resistance aura") {}
    };

    class DevotionAuraTrigger : public BuffTrigger
    {
    public:
        DevotionAuraTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "devotion aura") {}
    };

    class CleanseCureDiseaseTrigger : public NeedCureTrigger
    {
    public:
        CleanseCureDiseaseTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cleanse", DISPEL_DISEASE) {}
    };

    class CleanseCurePartyMemberDiseaseTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        CleanseCurePartyMemberDiseaseTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cleanse", DISPEL_DISEASE) {}
    };

    class CleanseCurePoisonTrigger : public NeedCureTrigger
    {
    public:
        CleanseCurePoisonTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cleanse", DISPEL_POISON) {}
    };

    class CleanseCurePartyMemberPoisonTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        CleanseCurePartyMemberPoisonTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cleanse", DISPEL_POISON) {}
    };

    class CleanseCureMagicTrigger : public NeedCureTrigger
    {
    public:
        CleanseCureMagicTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "cleanse", DISPEL_MAGIC) {}
    };

    class CleanseCurePartyMemberMagicTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        CleanseCurePartyMemberMagicTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "cleanse", DISPEL_MAGIC) {}
    };

    class HammerOfJusticeEnemyHealerTrigger : public InterruptEnemyHealerTrigger
    {
    public:
        HammerOfJusticeEnemyHealerTrigger(PlayerbotAI* ai) : InterruptEnemyHealerTrigger(ai, "hammer of justice") {}
    };

    class DivineFavorTrigger : public BoostBuffTrigger
    {
    public:
        DivineFavorTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "divine favor") {}
    };

    class TurnUndeadTrigger : public HasCcTargetTrigger
    {
    public:
        TurnUndeadTrigger(PlayerbotAI* ai) : HasCcTargetTrigger(ai, "turn undead") {}
    };

    class AvengingWrathTrigger : public BoostBuffTrigger
    {
    public:
        AvengingWrathTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "avenging wrath", 150.0f) {}
    };

    class BeaconofLightOnTankTrigger : public BuffOnTankTrigger {
    public:
        BeaconofLightOnTankTrigger(PlayerbotAI* ai) : BuffOnTankTrigger(ai, "beacon of light", 1) {}

        virtual bool IsActive() {
            return BuffOnTankTrigger::IsActive() &&
                GetTarget() &&
                !ai->HasAura("beacon of light", GetTarget()) &&
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
