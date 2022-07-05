#pragma once

#include "../actions/GenericActions.h"
#include "DruidShapeshiftActions.h"
#include "DruidBearActions.h"
#include "DruidCatActions.h"

namespace ai
{
	class CastFaerieFireAction : public CastDebuffSpellAction
	{
	public:
		CastFaerieFireAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "faerie fire") {}
	};

    class CastFaerieFireFeralAction : public CastDebuffSpellAction
    {
    public:
        CastFaerieFireFeralAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "faerie fire (feral)") {}
    };

	class CastRejuvenationAction : public CastHealingSpellAction {
	public:
		CastRejuvenationAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "rejuvenation") {}
	};

	class CastRegrowthAction : public CastHealingSpellAction {
	public:
		CastRegrowthAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "regrowth") {}

	};

    class CastHealingTouchAction : public CastHealingSpellAction {
    public:
        CastHealingTouchAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "healing touch") {}

    };

    class CastRejuvenationOnPartyAction : public HealPartyMemberAction
    {
    public:
        CastRejuvenationOnPartyAction(PlayerbotAI* ai) : HealPartyMemberAction(ai, "rejuvenation") {}
    };

    class CastRegrowthOnPartyAction : public HealPartyMemberAction
    {
    public:
        CastRegrowthOnPartyAction(PlayerbotAI* ai) : HealPartyMemberAction(ai, "regrowth") {}
    };

    class CastHealingTouchOnPartyAction : public HealPartyMemberAction
    {
    public:
        CastHealingTouchOnPartyAction(PlayerbotAI* ai) : HealPartyMemberAction(ai, "healing touch") {}
    };

	class CastReviveAction : public ResurrectPartyMemberAction
	{
	public:
		CastReviveAction(PlayerbotAI* ai) : ResurrectPartyMemberAction(ai, "revive") {}

		virtual NextAction** getPrerequisites() {
			return NextAction::merge( NextAction::array(0, new NextAction("caster form"), NULL), ResurrectPartyMemberAction::getPrerequisites());
		}
	};

	class CastRebirthAction : public ResurrectPartyMemberAction
	{
	public:
		CastRebirthAction(PlayerbotAI* ai) : ResurrectPartyMemberAction(ai, "rebirth") {}

		virtual NextAction** getPrerequisites() {
			return NextAction::merge( NextAction::array(0, new NextAction("caster form"), NULL), ResurrectPartyMemberAction::getPrerequisites());
		}
	};

	class CastMarkOfTheWildAction : public CastBuffSpellAction {
	public:
		CastMarkOfTheWildAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "mark of the wild") {}
	};

	class CastMarkOfTheWildOnPartyAction : public BuffOnPartyAction {
	public:
		CastMarkOfTheWildOnPartyAction(PlayerbotAI* ai) : BuffOnPartyAction(ai, "mark of the wild") {}
	};

	class CastSurvivalInstinctsAction : public CastBuffSpellAction {
	public:
		CastSurvivalInstinctsAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "survival instincts") {}
	};

	class CastThornsAction : public CastBuffSpellAction {
	public:
		CastThornsAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "thorns") {}
	};

    class CastThornsOnPartyAction : public BuffOnPartyAction {
    public:
        CastThornsOnPartyAction(PlayerbotAI* ai) : BuffOnPartyAction(ai, "thorns") {}
    };

	class CastOmenOfClarityAction : public CastBuffSpellAction {
	public:
	    CastOmenOfClarityAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "omen of clarity") {}
	};

	class CastWrathAction : public CastRangeSpellAction
	{
	public:
		CastWrathAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "wrath") {}
	};

	class CastStarfallAction : public CastBuffSpellAction
	{
	public:
		CastStarfallAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "starfall") {}
		virtual bool isUseful()
		{
			return sServerFacade.IsDistanceLessOrEqualThan(AI_VALUE2(float, "distance", GetTargetName()), 25.0f);
		}
	};

	class CastHurricaneAction : public CastRangeSpellAction
	{
	public:
	    CastHurricaneAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "hurricane") {}
	};

	class CastMoonfireAction : public CastDebuffSpellAction
	{
	public:
		CastMoonfireAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "moonfire") {}
	};

	class CastInsectSwarmAction : public CastDebuffSpellAction
	{
	public:
		CastInsectSwarmAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "insect swarm") {}
	};

	class CastStarfireAction : public CastRangeSpellAction
	{
	public:
		CastStarfireAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "starfire") {}
	};

	class CastEntanglingRootsAction : public CastRangeSpellAction
	{
	public:
		CastEntanglingRootsAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "entangling roots") {}
	};

    class CastEntanglingRootsCcAction : public CastRangeSpellAction
    {
    public:
        CastEntanglingRootsCcAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "entangling roots on cc") {}
        virtual Value<Unit*>* GetTargetValue();
        virtual bool Execute(Event event);
    };

    class CastHibernateAction : public CastRangeSpellAction
	{
	public:
        CastHibernateAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "hibernate") {}
	};

    class CastHibernateCcAction : public CastRangeSpellAction
    {
    public:
        CastHibernateCcAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "hibernate on cc") {}
        virtual Value<Unit*>* GetTargetValue();
        virtual bool Execute(Event event);
    };

    class CastNaturesGraspAction : public CastBuffSpellAction
	{
	public:
        CastNaturesGraspAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "nature's grasp") {}
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

	class CastAbolishPoisonAction : public CastCureSpellAction
	{
	public:
		CastAbolishPoisonAction(PlayerbotAI* ai) : CastCureSpellAction(ai, "abolish poison") {}
		virtual NextAction** getAlternatives();
	};

    class CastAbolishPoisonOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastAbolishPoisonOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "abolish poison", DISPEL_POISON) {}

        virtual NextAction** getAlternatives();
    };

    class CastBarkskinAction : public CastBuffSpellAction
    {
    public:
        CastBarkskinAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "barkskin") {}
    };

    class CastInnervateAction : public CastRangeSpellAction
    {
    public:
        CastInnervateAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "innervate") {}

        virtual string GetTargetName() { return "self target"; }
    };

    class CastTranquilityAction : public CastAoeHealSpellAction
    {
    public:
        CastTranquilityAction(PlayerbotAI* ai) : CastAoeHealSpellAction(ai, "tranquility") {}
    };

    class CastNaturesSwiftnessAction : public CastBuffSpellAction
    {
    public:
        CastNaturesSwiftnessAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "nature's swiftness") {}
    };
}
