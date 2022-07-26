#pragma once
#include "../triggers/GenericTriggers.h"

namespace ai
{
	//class DemonArmorTrigger : public BuffTrigger
	//{
	//public:
	//	DemonArmorTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "demon armor") {}
	//	virtual bool IsActive();
	//};

    class SpellstoneTrigger : public BuffTrigger
    {
    public:
        SpellstoneTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "spellstone") {}
        virtual bool IsActive();
    };

    class NoArmorTrigger : public BuffTrigger
    {
    public:
        NoArmorTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "fel armor") {}
        virtual bool IsActive();
    };

    DEBUFF_TRIGGER(CurseOfAgonyTrigger, "curse of agony");
    DEBUFF_TRIGGER(CorruptionTrigger, "corruption");
    DEBUFF_TRIGGER(SiphonLifeTrigger, "siphon life");
    DEBUFF_TRIGGER(UnstableAfflictionTrigger, "unstable affliction");
    DEBUFF_TRIGGER(HauntTrigger, "haunt");

    //BUFF_TRIGGER(FelArmorTrigger, "fel armor");

    class SoulstoneOnTankTrigger : public BuffOnTankTrigger {
    public:
        SoulstoneOnTankTrigger(PlayerbotAI* ai) : BuffOnTankTrigger(ai, "soulstone resurrection", 1) {}

        virtual bool IsActive() {
            bool hasItem = AI_VALUE2(uint32, "item count", "demonic soulstone") > 0;
            bool hasTarget = GetTarget();
            //bool targetHasAura = !ai->HasAura("soulstone resurrection", GetTarget());
            //bool isInGroup = (ai->GetBot()->IsInGroup((Player*)GetTarget(), true) || ai->GetBot()->IsInGroup((Player*)GetTarget()));
            return hasItem &&//BuffOnTankTrigger::IsActive() &&
                GetTarget() &&
                !ai->HasAura("soulstone resurrection", GetTarget()) &&
#ifdef MANGOS
                (ai->GetBot()->IsInSameGroupWith((Player*)GetTarget()) || ai->GetBot()->IsInSameRaidWith((Player*)GetTarget())) &&
#endif
#ifdef CMANGOS
                (ai->GetBot()->IsInGroup((Player*)GetTarget(), true) || ai->GetBot()->IsInGroup((Player*)GetTarget()))
#endif               
                ;
        }
    };

    class ImprovedShadowBoltTrigger : public DebuffTrigger
    {
    public:
        ImprovedShadowBoltTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "shadow bolt") {}
        virtual bool IsActive()
        {
            // improved shadow bolt aura
            if (!ai->HasAura(17800, GetTarget()))
                return BuffTrigger::IsActive();

            return false;
        }
    };

    class CorruptionOnAttackerTrigger : public DebuffOnAttackerTrigger
    {
    public:
        CorruptionOnAttackerTrigger(PlayerbotAI* ai) : DebuffOnAttackerTrigger(ai, "corruption") {}
    };

    class CastCurseOfAgonyOnAttackerTrigger : public DebuffOnAttackerTrigger
    {
    public:
        CastCurseOfAgonyOnAttackerTrigger(PlayerbotAI* ai) : DebuffOnAttackerTrigger(ai, "curse of agony") {}
    };

    class SiphonLifeOnAttackerTrigger : public DebuffOnAttackerTrigger
    {
    public:
        SiphonLifeOnAttackerTrigger(PlayerbotAI* ai) : DebuffOnAttackerTrigger(ai, "siphon life") {}
    };

    DEBUFF_TRIGGER(ImmolateTrigger, "immolate");

    class ShadowTranceTrigger : public HasAuraTrigger
    {
    public:
        ShadowTranceTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "shadow trance") {}
    };

    class BacklashTrigger : public HasAuraTrigger
    {
    public:
        BacklashTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "backlash") {}
    };

    class BanishTrigger : public HasCcTargetTrigger
    {
    public:
        BanishTrigger(PlayerbotAI* ai) : HasCcTargetTrigger(ai, "banish") {}
    };


    class FearTrigger : public HasCcTargetTrigger
    {
    public:
        FearTrigger(PlayerbotAI* ai) : HasCcTargetTrigger(ai, "fear") {}
    };

    class AmplifyCurseTrigger : public BuffTrigger
    {
    public:
        AmplifyCurseTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "amplify curse") {}
    };

    class LifeTapBoostTrigger : public BuffTrigger
    {
    public:
        LifeTapBoostTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "life tap") {}
        virtual bool IsActive()
        {
            // Glyph of Life tap                and glyphe effect
            return ai->HasAura(63320, GetTarget()) && !ai->HasAura(63321, GetTarget()) && SpellTrigger::IsActive();
        }
    };


    class DrainSoulTrigger : public TargetLowHealthTrigger
    {
    public:
        DrainSoulTrigger(PlayerbotAI* ai) : TargetLowHealthTrigger(ai, 25) {}
        virtual bool IsActive()
        {
            return TargetLowHealthTrigger::IsActive() && AI_VALUE2(uint32, "item count", "soul shard") < 28;// int(AI_VALUE(uint8, "bag space") * 0.2);
        }
    };



    //class TargetLowHealthTrigger : public HealthInRangeTrigger {
    //public:
    //    TargetLowHealthTrigger(PlayerbotAI* ai, float value, float minValue = 0) :
    //        HealthInRangeTrigger(ai, "target low health", value, minValue) {}
    //    virtual string GetTargetName() { return "current target"; }
    //};

    //class TargetCriticalHealthTrigger : public TargetLowHealthTrigger
    //{
    //public:
    //    TargetCriticalHealthTrigger(PlayerbotAI* ai) : TargetLowHealthTrigger(ai, 20) {}
    //};
    class SummonImpTrigger : public BuffTrigger
    {
    public:
        SummonImpTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "summon imp", 30) {}
        virtual bool IsActive() { return !AI_VALUE(Unit*, "pet target") && !AI_VALUE2(bool, "mounted", "self target"); }
    };

    // trigger with soul shards
    class SummonDeamonTrigger : public BuffTrigger
    {
    public:
        SummonDeamonTrigger(PlayerbotAI* ai, string spell) : BuffTrigger(ai, spell, 30) {}
        virtual bool IsActive() { return !AI_VALUE(Unit*, "pet target") && !AI_VALUE2(bool, "mounted", "self target") && BuffTrigger::IsActive() && AI_VALUE2(uint32, "item count", "soul shard") > 0; }
    };

    class WarlockConjuredItemTrigger : public ItemCountTrigger
    {
    public:
        WarlockConjuredItemTrigger(PlayerbotAI* ai, string item) : ItemCountTrigger(ai, item, 1) {}

        virtual bool IsActive() { return ItemCountTrigger::IsActive() && AI_VALUE2(uint32, "item count", "soul shard") > 0; }
    };

    class HasSpellstoneTrigger : public WarlockConjuredItemTrigger
    {
    public:
        HasSpellstoneTrigger(PlayerbotAI* ai) : WarlockConjuredItemTrigger(ai, "spellstone") {}
    };

    class HasFirestoneTrigger : public WarlockConjuredItemTrigger
    {
    public:
        HasFirestoneTrigger(PlayerbotAI* ai) : WarlockConjuredItemTrigger(ai, "firestone") {}
    };

    class HasHealthstoneTrigger : public WarlockConjuredItemTrigger
    {
    public:
        HasHealthstoneTrigger(PlayerbotAI* ai) : WarlockConjuredItemTrigger(ai, "healthstone") {}
    };

    class HasSoulstoneTrigger : public WarlockConjuredItemTrigger
    {
    public:
        HasSoulstoneTrigger(PlayerbotAI* ai) : WarlockConjuredItemTrigger(ai, "demonic soulstone") {}
    };

    //class NoRefreshmentTrigger : public Trigger {
    //public:
    //    NoRefreshmentTrigger(PlayerbotAI* ai) : Trigger(ai, "no refreshment trigger") {}
    //    virtual bool IsActive() { return AI_VALUE2(list<Item*>, "inventory items", "conjured refreshment").empty(); }
    //};
}
