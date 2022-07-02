#pragma once
#include "../triggers/GenericTriggers.h"

namespace ai
{
    DEFLECT_TRIGGER(FireWardTrigger, "fire ward");
    DEFLECT_TRIGGER(FrostWardTrigger, "frost ward");

    class ArcaneIntellectOnPartyTrigger : public BuffOnPartyTrigger {
    public:
        ArcaneIntellectOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "arcane intellect", 2) {}

        virtual bool IsActive() { return BuffOnPartyTrigger::IsActive() && !ai->HasAura("arcane brilliance", GetTarget()); }
    };

    class ArcaneIntellectTrigger : public BuffTrigger {
    public:
        ArcaneIntellectTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "arcane intellect", 2) {}

        virtual bool IsActive() { return BuffTrigger::IsActive() && !ai->HasAura("arcane brilliance", GetTarget()); }
    };

    class MageArmorTrigger : public BuffTrigger {
    public:
        MageArmorTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "mage armor", 5) {}
        virtual bool IsActive();
    };

	MY_DEBUFF_TRIGGER(LivingBombTrigger, "living bomb");

    class FireballTrigger : public DebuffTrigger {
    public:
        FireballTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "fireball") {}
	};

    class PyroblastTrigger : public DebuffTrigger {
    public:
        PyroblastTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "pyroblast") {}
    };

    class HotStreakTrigger : public SpellCanBeCastInstantTrigger {
    public:
        HotStreakTrigger(PlayerbotAI* ai) : SpellCanBeCastInstantTrigger(ai, "pyroblast") {}
    };

    class MissileBarrageTrigger : public HasAuraTrigger {
    public:
        MissileBarrageTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "missile barrage") {}
    };

    class ArcaneBlastTrigger : public BuffTrigger {
    public:
        ArcaneBlastTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "arcane blast") {}
    };

    class CounterspellInterruptSpellTrigger : public InterruptSpellTrigger
    {
    public:
        CounterspellInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "counterspell") {}
    };

    class CombustionTrigger : public BoostBuffTrigger
    {
    public:
        CombustionTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "combustion") {}
    };

    class IcyVeinsTrigger : public BoostBuffTrigger
    {
    public:
        IcyVeinsTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "icy veins") {}
    };

    class PolymorphTrigger : public HasCcTargetTrigger
    {
    public:
        PolymorphTrigger(PlayerbotAI* ai) : HasCcTargetTrigger(ai, "polymorph") {}
    };

    class RemoveCurseTrigger : public NeedCureTrigger
    {
    public:
        RemoveCurseTrigger(PlayerbotAI* ai) : NeedCureTrigger(ai, "remove curse", DISPEL_CURSE) {}
    };

    class PartyMemberRemoveCurseTrigger : public PartyMemberNeedCureTrigger
    {
    public:
        PartyMemberRemoveCurseTrigger(PlayerbotAI* ai) : PartyMemberNeedCureTrigger(ai, "remove curse", DISPEL_CURSE) {}
    };

    class SpellstealTrigger : public TargetAuraDispelTrigger
    {
    public:
        SpellstealTrigger(PlayerbotAI* ai) : TargetAuraDispelTrigger(ai, "spellsteal", DISPEL_MAGIC) {}
    };

    class CounterspellEnemyHealerTrigger : public InterruptEnemyHealerTrigger
    {
    public:
        CounterspellEnemyHealerTrigger(PlayerbotAI* ai) : InterruptEnemyHealerTrigger(ai, "counterspell") {}
    };

    class ArcanePowerTrigger : public BuffTrigger
    {
    public:
        ArcanePowerTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "arcane power") {}
    };

    class PresenceOfMindTrigger : public BuffTrigger
    {
    public:
        PresenceOfMindTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "presence of mind") {}
    };

    class ImprovedScorchTrigger : public DebuffTrigger
    {
    public:
        ImprovedScorchTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "improved scorch") {}
    };
}
