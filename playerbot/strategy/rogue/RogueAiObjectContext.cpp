#include "botpch.h"
#include "../../playerbot.h"
#include "RogueActions.h"
#include "RogueTriggers.h"
#include "RogueAiObjectContext.h"
#include "DpsRogueStrategy.h"
#include "GenericRogueNonCombatStrategy.h"
#include "../generic/PullStrategy.h"
#include "../NamedObjectContext.h"

using namespace ai;


namespace ai
{
    namespace rogue
    {
        using namespace ai;

        class StrategyFactoryInternal : public NamedObjectContext<Strategy>
        {
        public:
            StrategyFactoryInternal()
            {
                creators["dps"] = &rogue::StrategyFactoryInternal::dps;
                creators["nc"] = &rogue::StrategyFactoryInternal::nc;
                creators["pull"] = &rogue::StrategyFactoryInternal::pull;
                creators["aoe"] = &rogue::StrategyFactoryInternal::aoe;
                creators["boost"] = &rogue::StrategyFactoryInternal::boost;
                creators["stealthed"] = &rogue::StrategyFactoryInternal::stealthed;
                creators["stealth"] = &rogue::StrategyFactoryInternal::stealth;
                creators["cc"] = &rogue::StrategyFactoryInternal::cc;
                creators["combat"] = &rogue::StrategyFactoryInternal::combat;
                creators["assassin"] = &rogue::StrategyFactoryInternal::assassination;
                creators["subtlety"] = &rogue::StrategyFactoryInternal::subtlety;
            }

        private:
            static Strategy* boost(PlayerbotAI* ai) { return new RogueBoostStrategy(ai); }
            static Strategy* aoe(PlayerbotAI* ai) { return new RogueAoeStrategy(ai); }
            static Strategy* dps(PlayerbotAI* ai) { return new DpsRogueStrategy(ai); }
            static Strategy* nc(PlayerbotAI* ai) { return new GenericRogueNonCombatStrategy(ai); }
            static Strategy* pull(PlayerbotAI* ai) { return new PullStrategy(ai, "shoot"); }
            static Strategy* stealthed(PlayerbotAI* ai) { return new StealthedRogueStrategy(ai); }
            static Strategy* stealth(PlayerbotAI* ai) { return new StealthStrategy(ai); }
            static Strategy* cc(PlayerbotAI* ai) { return new RogueCcStrategy(ai); }
            static Strategy* combat(PlayerbotAI* ai) { return new CombatRogueStrategy(ai); }

            static Strategy* assassination(PlayerbotAI* ai) { return new AssassinationRogueStrategy(ai); }
            static Strategy* subtlety(PlayerbotAI* ai) { return new CombatRogueStrategy(ai); }
        };
    };
};

namespace ai
{
    namespace rogue
    {
        using namespace ai;

        class TriggerFactoryInternal : public NamedObjectContext<Trigger>
        {
        public:
            TriggerFactoryInternal()
            {
                creators["kick"] = &TriggerFactoryInternal::kick;
                creators["rupture"] = &TriggerFactoryInternal::rupture;
                creators["slice and dice"] = &TriggerFactoryInternal::slice_and_dice;
                creators["expose armor"] = &TriggerFactoryInternal::expose_armor;
                creators["kick on enemy healer"] = &TriggerFactoryInternal::kick_on_enemy_healer;
                creators["adrenaline rush"] = &TriggerFactoryInternal::adrenaline_rush;
                creators["unstealth"] = &TriggerFactoryInternal::unstealth;
                creators["sap"] = &TriggerFactoryInternal::sap;
                creators["in stealth"] = &TriggerFactoryInternal::in_stealth;
                creators["no stealth"] = &TriggerFactoryInternal::no_stealth;
                creators["stealth"] = &TriggerFactoryInternal::stealth;
                creators["sprint"] = &TriggerFactoryInternal::sprint;
                creators["sinister strike"] = &TriggerFactoryInternal::sinister_strike;
                creators["eviscerate"] = &TriggerFactoryInternal::eviscerate;
                creators["killing spree"] = &TriggerFactoryInternal::killing_spree;
                creators["blade flurry"] = &TriggerFactoryInternal::blade_flurry;
                creators["cloak of shadows"] = &TriggerFactoryInternal::cloak_of_shadows;
                
                //creators["tricks of the trade on tank"] = &TriggerFactoryInternal::tricks_of_the_trade_on_tank;
                creators["fan of knives"] = &TriggerFactoryInternal::fan_of_knives;
            }

        private:
            static Trigger* adrenaline_rush(PlayerbotAI* ai) { return new RogueBoostBuffTrigger(ai, "adrenaline rush"); }
            static Trigger* kick(PlayerbotAI* ai) { return new KickInterruptSpellTrigger(ai); }
            static Trigger* rupture(PlayerbotAI* ai) { return new RuptureTrigger(ai); }
            static Trigger* slice_and_dice(PlayerbotAI* ai) { return new SliceAndDiceTrigger(ai); }
            static Trigger* expose_armor(PlayerbotAI* ai) { return new ExposeArmorTrigger(ai); }
            static Trigger* kick_on_enemy_healer(PlayerbotAI* ai) { return new KickInterruptEnemyHealerSpellTrigger(ai); }
            static Trigger* unstealth(PlayerbotAI* ai) { return new UnstealthTrigger(ai); }
            static Trigger* sap(PlayerbotAI* ai) { return new SapTrigger(ai); }
            static Trigger* in_stealth(PlayerbotAI* ai) { return new InStealthTrigger(ai); }
            static Trigger* no_stealth(PlayerbotAI* ai) { return new NoStealthTrigger(ai); }
            static Trigger* stealth(PlayerbotAI* ai) { return new StealthTrigger(ai); }
            static Trigger* sprint(PlayerbotAI* ai) { return new SprintTrigger(ai); }
            static Trigger* sinister_strike(PlayerbotAI* ai) { return new SpellCanBeCastTrigger(ai, "sinister strike"); }
            static Trigger* eviscerate(PlayerbotAI* ai) { return new EviscerateTrigger(ai); }
            static Trigger* killing_spree(PlayerbotAI* ai) { return new SpellCanBeCastTrigger(ai, "killing spree"); }
            static Trigger* blade_flurry(PlayerbotAI* ai) { return new RogueBoostBuffTrigger(ai, "blade flurry"); }
            static Trigger* cloak_of_shadows(PlayerbotAI* ai) { return new CloakOfShadowsTrigger(ai); }

            //static Trigger* tricks_of_the_trade_on_tank(PlayerbotAI* ai) { return new TricksOfTheTradeOnTankTrigger(ai); }
            static Trigger* fan_of_knives(PlayerbotAI* ai) { return new SpellCanBeCastTrigger(ai, "fan of knives"); }
        };
    };
};


namespace ai
{
    namespace rogue
    {
        using namespace ai;

        class AiObjectContextInternal : public NamedObjectContext<Action>
        {
        public:
            AiObjectContextInternal()
            {
                creators["riposte"] = &AiObjectContextInternal::riposte;
                creators["mutilate"] = &AiObjectContextInternal::mutilate;
                creators["sinister strike"] = &AiObjectContextInternal::sinister_strike;
                creators["kidney shot"] = &AiObjectContextInternal::kidney_shot;
                creators["rupture"] = &AiObjectContextInternal::rupture;
                creators["slice and dice"] = &AiObjectContextInternal::slice_and_dice;
                creators["eviscerate"] = &AiObjectContextInternal::eviscerate;
                creators["vanish"] = &AiObjectContextInternal::vanish;
                creators["evasion"] = &AiObjectContextInternal::evasion;
                creators["kick"] = &AiObjectContextInternal::kick;
                creators["feint"] = &AiObjectContextInternal::feint;
                creators["backstab"] = &AiObjectContextInternal::backstab;
                creators["expose armor"] = &AiObjectContextInternal::expose_armor;
                creators["kick on enemy healer"] = &AiObjectContextInternal::kick_on_enemy_healer;
                creators["blade flurry"] = &AiObjectContextInternal::blade_flurry;
                creators["adrenaline rush"] = &AiObjectContextInternal::adrenaline_rush;
                creators["ambush"] = &AiObjectContextInternal::ambush;
                creators["stealth"] = &AiObjectContextInternal::stealth;
                creators["sprint"] = &AiObjectContextInternal::sprint;
                creators["garrote"] = &AiObjectContextInternal::garrote;
                creators["cheap shot"] = &AiObjectContextInternal::cheap_shot;
                creators["blind"] = &AiObjectContextInternal::blind;
                creators["unstealth"] = &AiObjectContextInternal::unstealth;
                creators["sap"] = &AiObjectContextInternal::sap;
                creators["check stealth"] = &AiObjectContextInternal::check_stealth;
                creators["killing spree"] = &AiObjectContextInternal::killing_spree;
                creators["cloak of shadows"] = &AiObjectContextInternal::cloak_of_shadows;

                //creators["tricks of the trade on tank"] = &AiObjectContextInternal::tricks_of_the_trade_on_tank;
                creators["fan of knives"] = &AiObjectContextInternal::fan_of_knives;
            }

        private:
            static Action* check_stealth(PlayerbotAI* ai) { return new CheckStealthAction(ai); }
            static Action* sap(PlayerbotAI* ai) { return new CastSapAction(ai); }
            static Action* unstealth(PlayerbotAI* ai) { return new UnstealthAction(ai); }
            static Action* blind(PlayerbotAI* ai) { return new CastBlindAction(ai); }
            static Action* ambush(PlayerbotAI* ai) { return new CastAmbushAction(ai); }
            static Action* stealth(PlayerbotAI* ai) { return new CastStealthAction(ai); }
            static Action* sprint(PlayerbotAI* ai) { return new CastSprintAction(ai); }
            static Action* garrote(PlayerbotAI* ai) { return new CastGarroteAction(ai); }
            static Action* cheap_shot(PlayerbotAI* ai) { return new CastCheapShotAction(ai); }
            static Action* adrenaline_rush(PlayerbotAI* ai) { return new CastAdrenalineRushAction(ai); }
            static Action* blade_flurry(PlayerbotAI* ai) { return new CastBladeFlurryAction(ai); }
            static Action* riposte(PlayerbotAI* ai) { return new CastRiposteAction(ai); }
            static Action* mutilate(PlayerbotAI* ai) { return new CastMutilateAction(ai); }
            static Action* sinister_strike(PlayerbotAI* ai) { return new CastSinisterStrikeAction(ai); }
            static Action* kidney_shot(PlayerbotAI* ai) { return new CastKidneyShotAction(ai); }
            static Action* rupture(PlayerbotAI* ai) { return new CastRuptureAction(ai); }
            static Action* slice_and_dice(PlayerbotAI* ai) { return new CastSliceAndDiceAction(ai); }
            static Action* eviscerate(PlayerbotAI* ai) { return new CastEviscerateAction(ai); }
            static Action* vanish(PlayerbotAI* ai) { return new CastVanishAction(ai); }
            static Action* evasion(PlayerbotAI* ai) { return new CastEvasionAction(ai); }
            static Action* kick(PlayerbotAI* ai) { return new CastKickAction(ai); }
            static Action* feint(PlayerbotAI* ai) { return new CastFeintAction(ai); }
            static Action* backstab(PlayerbotAI* ai) { return new CastBackstabAction(ai); }
            static Action* expose_armor(PlayerbotAI* ai) { return new CastExposeArmorAction(ai); }
            static Action* kick_on_enemy_healer(PlayerbotAI* ai) { return new CastKickOnEnemyHealerAction(ai); }
            static Action* killing_spree(PlayerbotAI* ai) { return new CastKillingSpreeAction(ai); }
            static Action* cloak_of_shadows(PlayerbotAI* ai) { return new CastCloakOfShadowsAction(ai); }

            //static Action* tricks_of_the_trade_on_tank(PlayerbotAI* ai) { return new CastTricksOfTheTradeOnPartyAction(ai); }
            static Action* fan_of_knives(PlayerbotAI* ai) { return new CastFanOfKnivesAction(ai); }
        };
    };
};

RogueAiObjectContext::RogueAiObjectContext(PlayerbotAI* ai) : AiObjectContext(ai)
{
    strategyContexts.Add(new ai::rogue::StrategyFactoryInternal());
    actionContexts.Add(new ai::rogue::AiObjectContextInternal());
    triggerContexts.Add(new ai::rogue::TriggerFactoryInternal());
}
