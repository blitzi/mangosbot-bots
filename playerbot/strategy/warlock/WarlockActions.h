#pragma once

#include "../actions/GenericActions.h"

namespace ai
{
	class CastDemonSkinAction : public CastBuffSpellAction {
	public:
		CastDemonSkinAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "demon skin") {}
	};

	class CastDemonArmorAction : public CastBuffSpellAction
	{
	public:
		CastDemonArmorAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "demon armor") {}
	};

	class CastFelArmorAction : public CastBuffSpellAction
	{
	public:
		CastFelArmorAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "fel armor") {}
	};

    BEGIN_RANGED_SPELL_ACTION(CastShadowBoltAction, "shadow bolt")
    END_SPELL_ACTION()

	class CastDrainSoulAction : public CastRangeSpellAction
	{
	public:
		CastDrainSoulAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "drain soul") {}
		virtual bool isUseful()
		{
			return AI_VALUE2(uint32, "item count", "soul shard") < int(AI_VALUE(uint8, "bag space") * 0.2);
		}
	};

	class CastDrainManaAction : public CastRangeSpellAction
	{
	public:
		CastDrainManaAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "drain mana") {}
	};

	class CastDrainLifeAction : public CastRangeSpellAction
	{
	public:
		CastDrainLifeAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "drain life") {}
	};

	class CastCurseOfAgonyAction : public CastDebuffSpellAction
	{
	public:
		CastCurseOfAgonyAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "curse of agony") {}
	};

	class CastCurseOfWeaknessAction : public CastDebuffSpellAction
	{
	public:
		CastCurseOfWeaknessAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "curse of weakness") {}
	};

	class CastCorruptionAction : public CastDebuffSpellAction
	{
	public:
		CastCorruptionAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "corruption") {}
	};

	class CastCorruptionOnAttackerAction : public CastDebuffSpellOnAttackerAction
	{
	public:
	    CastCorruptionOnAttackerAction(PlayerbotAI* ai) : CastDebuffSpellOnAttackerAction(ai, "corruption") {}
	};

    class CastCurseOfAgonyOnAttackerAction : public CastDebuffSpellOnAttackerAction
    {
    public:
        CastCurseOfAgonyOnAttackerAction(PlayerbotAI* ai) : CastDebuffSpellOnAttackerAction(ai, "curse of agony") {}
    };

	class CastSummonVoidwalkerAction : public CastBuffSpellAction
	{
	public:
		CastSummonVoidwalkerAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "summon voidwalker") {}
	};

	class CastSummonFelguardAction : public CastBuffSpellAction
	{
	public:
		CastSummonFelguardAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "summon felguard") {}
	};

	class CastSummonImpAction : public CastBuffSpellAction
	{
	public:
		CastSummonImpAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "summon imp") {}
	};

    class CastSummonSuccubusAction : public CastBuffSpellAction
    {
    public:
        CastSummonSuccubusAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "summon succubus") {}
    };

	class CastCreateHealthstoneAction : public CastBuffSpellAction
	{
	public:
		CastCreateHealthstoneAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "create healthstone") {}
	};

	class CastCreateFirestoneAction : public CastBuffSpellAction
	{
	public:
		CastCreateFirestoneAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "create firestone") {}
	};

	class CastCreateSpellstoneAction : public CastBuffSpellAction
	{
	public:
		CastCreateSpellstoneAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "create spellstone") {}
	};

    class CastBanishAction : public CastBuffSpellAction
    {
    public:
        CastBanishAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "banish on cc") {}
        virtual Value<Unit*>* GetTargetValue() { return context->GetValue<Unit*>("cc target", "banish"); }
        virtual bool Execute(Event event) { return ai->CastSpell("banish", GetTarget()); }
    };

    class CastSeedOfCorruptionAction : public CastDebuffSpellAction
    {
    public:
        CastSeedOfCorruptionAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "seed of corruption") {}
    };

    class CastRainOfFireAction : public CastRangeSpellAction
    {
    public:
        CastRainOfFireAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "rain of fire") {}
    };

    class CastShadowfuryAction : public CastRangeSpellAction
    {
    public:
        CastShadowfuryAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "shadowfury") {}
    };

    class CastImmolateAction : public CastDebuffSpellAction
    {
    public:
        CastImmolateAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "immolate") {}
    };

    class CastConflagrateAction : public CastRangeSpellAction
    {
    public:
        CastConflagrateAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "conflagrate") {}
    };

    class CastIncinirateAction : public CastRangeSpellAction
    {
    public:
        CastIncinirateAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "incinirate") {}
    };

    class CastFearAction : public CastDebuffSpellAction
    {
    public:
        CastFearAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "fear") {}
    };

    class CastFearOnCcAction : public CastBuffSpellAction
    {
    public:
        CastFearOnCcAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "fear on cc") {}
        virtual Value<Unit*>* GetTargetValue() { return context->GetValue<Unit*>("cc target", "fear"); }
        virtual bool Execute(Event event) { return ai->CastSpell("fear", GetTarget()); }
        virtual bool isPossible() { return ai->CanCastSpell("fear", GetTarget()); }
        virtual bool isUseful() { return true; }
    };

    class CastLifeTapAction: public CastRangeSpellAction
    {
    public:
        CastLifeTapAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "life tap") {}
        virtual string GetTargetName() { return "self target"; }
        virtual bool isUseful() { return AI_VALUE2(float, "health", "self target") > sPlayerbotAIConfig.lowHealth; }
    };

    class CastAmplifyCurseAction : public CastBuffSpellAction
    {
    public:
        CastAmplifyCurseAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "amplify curse") {}
    };

    class CastSiphonLifeAction : public CastDebuffSpellAction
    {
    public:
        CastSiphonLifeAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "siphon life") {}
    };

    class CastSiphonLifeOnAttackerAction : public CastDebuffSpellOnAttackerAction
    {
    public:
        CastSiphonLifeOnAttackerAction(PlayerbotAI* ai) : CastDebuffSpellOnAttackerAction(ai, "siphon life") {}
    };

}
