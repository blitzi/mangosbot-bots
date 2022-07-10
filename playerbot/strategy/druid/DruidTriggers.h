#pragma once
#include "../triggers/GenericTriggers.h"

namespace ai {
    class MarkOfTheWildOnPartyTrigger : public BuffOnPartyTrigger
    {
    public:
        MarkOfTheWildOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "mark of the wild", 2) {}

        virtual bool IsActive() { return BuffOnPartyTrigger::IsActive() && !ai->HasAura("gift of the wild", GetTarget()); }
    };

    class MarkOfTheWildTrigger : public BuffTrigger
    {
    public:
        MarkOfTheWildTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "mark of the wild", 2) {}

        virtual bool IsActive() { return BuffTrigger::IsActive() && !ai->HasAura("gift of the wild", GetTarget()); }
    };

    class ThornsOnPartyTrigger : public BuffOnPartyTrigger
    {
    public:
        ThornsOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "thorns", 2) {}

        virtual bool IsActive() { return BuffOnPartyTrigger::IsActive() && !ai->HasAura("thorns", GetTarget()); }
    };

    class ThornsTrigger : public BuffTrigger
    {
    public:
        ThornsTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "thorns", 2) {}

        virtual bool IsActive() { return BuffTrigger::IsActive() && !ai->HasAura("thorns", GetTarget()); }
    };

    class OmenOfClarityTrigger : public BuffTrigger
    {
    public:
        OmenOfClarityTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "omen of clarity") {}
    };

    class RakeTrigger : public DebuffTrigger
    {
    public:
        RakeTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "rake") {}
    };

    class InsectSwarmTrigger : public DebuffTrigger
    {
    public:
        InsectSwarmTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "insect swarm") {}
    };

    class MoonfireTrigger : public DebuffTrigger
    {
    public:
        MoonfireTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "moonfire") {}

        virtual bool IsActive() { return DebuffTrigger::IsActive() && !GetTarget()->IsRooted(); }
    };

    class FaerieFireTrigger : public DebuffTrigger
    {
    public:
        FaerieFireTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "faerie fire") {}
    };

    class FaerieFireFeralTrigger : public DebuffTrigger
    {
    public:
        FaerieFireFeralTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "faerie fire (feral)") {}
    };

    class BashInterruptSpellTrigger : public InterruptSpellTrigger
    {
    public:
        BashInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "bash") {}
    };

    class TigersFuryTrigger : public BoostBuffTrigger
    {
    public:
        TigersFuryTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "tiger's fury") {}
    };

    class NaturesGraspTrigger : public BoostBuffTrigger
    {
    public:
        NaturesGraspTrigger(PlayerbotAI* ai) : BoostBuffTrigger(ai, "nature's grasp") {}
    };

    class EntanglingRootsTrigger : public HasCcTargetTrigger
    {
    public:
        EntanglingRootsTrigger(PlayerbotAI* ai) : HasCcTargetTrigger(ai, "entangling roots") {}
    };

    class EntanglingRootsKiteTrigger : public DebuffTrigger
    {
    public:
        EntanglingRootsKiteTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "entangling roots") {}

        virtual bool IsActive() { return DebuffTrigger::IsActive() && AI_VALUE(uint8, "attacker count") < 3 && !GetTarget()->HasMana(); }
    };

    class HibernateTrigger : public HasCcTargetTrigger
    {
    public:
        HibernateTrigger(PlayerbotAI* ai) : HasCcTargetTrigger(ai, "hibernate") {}
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

    class BearFormTrigger : public BuffTrigger
    {
    public:
        BearFormTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "bear form") {}
        virtual bool IsActive() { return !ai->HasAnyAuraOf(bot, "bear form", "dire bear form", NULL); }
    };

    class TreeFormTrigger : public BuffTrigger
    {
    public:
        TreeFormTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "tree of life") {}
        virtual bool IsActive() { return !ai->HasAura("tree of life", bot); }
    };

    class CatFormTrigger : public BuffTrigger
    {
    public:
        CatFormTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "cat form") {}
        virtual bool IsActive() { return !ai->HasAura("cat form", bot); }
    };

    class EclipseSolarTrigger : public HasAuraTrigger
    {
    public:
        EclipseSolarTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "eclipse (solar)") {}
    };

    class EclipseLunarTrigger : public HasAuraTrigger
    {
    public:
        EclipseLunarTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "eclipse (lunar)") {}
    };

    class BashInterruptEnemyHealerSpellTrigger : public InterruptEnemyHealerTrigger
    {
    public:
        BashInterruptEnemyHealerSpellTrigger(PlayerbotAI* ai) : InterruptEnemyHealerTrigger(ai, "bash") {}
    };

    class NaturesSwiftnessTrigger : public BuffTrigger
    {
    public:
        NaturesSwiftnessTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "nature's swiftness") {}
    };

    class ForceOfNatureTrigger : public BuffTrigger
    {
    public:
        ForceOfNatureTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "force of nature") {}
    };

    class NourishTrigger : public BuffTrigger {
    public:
        NourishTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "nourish") {}

        virtual bool IsActive() {  return BuffTrigger::IsActive() && ai->HasAnyAuraOf(bot, "rejuvenation", "regrowth", "lifebloom", "wild growth", NULL);  }
    };

    class NourishOnPartyTrigger : public BuffOnPartyTrigger {
    public:
        NourishOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "nourish") {}

        virtual bool IsActive() { return BuffOnPartyTrigger::IsActive() && ai->HasAnyAuraOf(GetTarget(), "rejuvenation", "regrowth", "lifebloom", "wild growth", NULL); }
    };

    class NourishAndMediumHealthTrigger : public AndTrigger {
    public:
        NourishAndMediumHealthTrigger(PlayerbotAI* ai) : AndTrigger(ai, new NourishTrigger(ai), new MediumHealthTrigger(ai)) {}
    };

    class NourishOnPartyAndMediumHealthTrigger : public AndTrigger {
    public:
        NourishOnPartyAndMediumHealthTrigger(PlayerbotAI* ai) : AndTrigger(ai, new NourishOnPartyTrigger(ai), new PartyMemberMediumHealthTrigger(ai)) {}
    };

    class SwiftmendTrigger : public BuffTrigger {
    public:
        SwiftmendTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "swiftmend") {}

        virtual bool IsActive() { return ai->HasAnyAuraOf(bot, "rejuvenation", "regrowth", NULL); }
    };

    class SwiftmendOnPartyTrigger : public BuffOnPartyTrigger {
    public:
        SwiftmendOnPartyTrigger(PlayerbotAI* ai) : BuffOnPartyTrigger(ai, "switfmend") {}

        virtual bool IsActive() { return ai->HasAnyAuraOf(GetTarget(), "rejuvenation", "regrowth", NULL); }
    };

    class SwiftmendAndLowHealthTrigger : public AndTrigger {
    public:
        SwiftmendAndLowHealthTrigger(PlayerbotAI* ai) : AndTrigger(ai, new SwiftmendTrigger(ai), new LowHealthTrigger(ai)) {}
    };

    class SwiftmendOnPartyAndLowHealthTrigger : public AndTrigger {
    public:
        SwiftmendOnPartyAndLowHealthTrigger(PlayerbotAI* ai) : AndTrigger(ai, new SwiftmendOnPartyTrigger(ai), new PartyMemberLowHealthTrigger(ai)) {}
    };

    BUFF_PARTY_TRIGGER(RejuvenationHotOnPartyTrigger, "rejuvenation");

    class RejuvenationHotOnTankTrigger : public BuffOnTankTrigger {
    public:
        RejuvenationHotOnTankTrigger(PlayerbotAI* ai) : BuffOnTankTrigger(ai, "rejuvenation") {}
    };
}
