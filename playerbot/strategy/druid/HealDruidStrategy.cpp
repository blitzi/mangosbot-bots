#include "botpch.h"
#include "../../playerbot.h"
#include "DruidMultipliers.h"
#include "HealDruidStrategy.h"

using namespace ai;

class HealDruidStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    HealDruidStrategyActionNodeFactory()
    {
    }
private:
};

HealDruidStrategy::HealDruidStrategy(PlayerbotAI* ai) : GenericDruidStrategy(ai)
{
    actionNodeFactories.Add(new HealDruidStrategyActionNodeFactory());
}

/*
* The healing druid has the following main rotation
*  (DONE) - use 'tree of life' initially on fight
*  (DONE) - use 'rejuvenation' when the anyone including you has lost any hp
*  (DONE) [MAYBE 2 MUCH IN RAID?, TANK IS ENOUGH]- use 'rejuvenation' when the fight has started on as much party players as possible (no need to know their dmg) [no aura]
*  (DONE) - use 'wild growth' whenever any party member has lost any hp (can be started on fight start till end) [almost full]
*  (DONE) - use 'regrowth' when no regrowth buff is applied and you or anyone hast lost any spell
*  (DONE) - use 'regrowth', when nourish is not working and heal + restart hot
*  (DONE) - use 'nourish', when hp has been lost "a bit more than almost full" AND an auro of type lifebloom, rejuvenation, regrowth or wild growth is active
*  (DONE) - use 'barkskin' when on medium hp, or when a huge thread cast like 'tranquily' is about to be used
*  (DONE) - use 'swiftmend' when someon is at low hp
*  (DONE) - use 'healing touch' ONLY, when it can be casted instantly and someone is at least at medium health
*  (DONE) - use 'tranquility' only, when whole party (group or raid) is at low hp
*  (DONE) - use 'nature's swiftness' when someone is at critical health combined with 'healing touch'
*  (DONE) - use 'lifebloom', when 'cleacasting' is available, and noone is lower than medium health (regain mana later)
*  (TODO) - use 'lifebloom' on maintank on big encounters and stack it to 3 (+ holding it when it is nearly about to end) OR get mana back and end
*  (DONE) - use 'innervate' when on low mana
*  (TODO) - use 'innervate' when someone else by importance is at low mana (main healer?)
*  (DONE) - use 'rebirth' when someone died in battle (maybe bad to have this automatically - let human decide this??)
*  (TODO) - use 'rebirth' when someone of importance died in battle (tank, main healer) (maybe human player?)
*/
void HealDruidStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    // we skip the generic druid
    //GenericDruidStrategy::InitTriggers(triggers);
    CombatStrategy::InitTriggers(triggers);

    // non healing related actions
    {
        triggers.push_back(new TriggerNode(
            "enemy out of spell",
            NextAction::array(0, new NextAction("reach spell", ACTION_NORMAL + 9), NULL)));

        triggers.push_back(new TriggerNode(
            "party member dead",
            NextAction::array(0, new NextAction("rebirth", ACTION_HIGH + 1), NULL)));

        triggers.push_back(new TriggerNode(
            "party member to heal out of spell range",
            NextAction::array(0, new NextAction("reach party member to heal", ACTION_CRITICAL_HEAL + 1), NULL)));
    }

    // low section
    {
        // apply rejuv hot on tank
        triggers.push_back(new TriggerNode(
            "rejuvenation hot on tank",
            NextAction::array(0, new NextAction("rejuvenation hot on party", ACTION_LIGHT_HEAL), NULL)));

        // rejuv on any bit of dmg applied to party member
        triggers.push_back(new TriggerNode(
            "party member almost full health",
            NextAction::array(0, new NextAction("rejuvenation on party", ACTION_LIGHT_HEAL + 1), NULL)));

        // rejuv ourself on any dmg
        triggers.push_back(new TriggerNode(
            "almost full health",
            NextAction::array(0, new NextAction("rejuvenation", ACTION_LIGHT_HEAL + 2), NULL)));
    }

    // normal section
    {
        triggers.push_back(new TriggerNode(
            "party member medium health",
            NextAction::array(0, new NextAction("regrowth on party", ACTION_MEDIUM_HEAL + 1), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("regrowth", ACTION_MEDIUM_HEAL + 2), NULL)));

        //  apply nourish, whenever anyone is at medium health and has a druidHoT (regrowth, rejuv, lifebloom, wildgrowth) available
        triggers.push_back(new TriggerNode(
            "party member medium health",
            NextAction::array(0, new NextAction("nourish on party", ACTION_MEDIUM_HEAL + 3), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("nourish", ACTION_MEDIUM_HEAL + 4), NULL)));

        triggers.push_back(new TriggerNode(
            "party member medium health",
            NextAction::array(0, new NextAction("refresh regrowth on party", ACTION_MEDIUM_HEAL + 5), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("refresh regrowth", ACTION_MEDIUM_HEAL + 6), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("instant lifebloom", ACTION_MEDIUM_HEAL + 7), NULL)));
    }

    // critical section
    {
        // cast wildgrowth when: in a party & whenever available & anyone lost any portion of hp
        triggers.push_back(new TriggerNode(
            "almost full aoe heal",
            NextAction::array(0, new NextAction("wild growth", ACTION_CRITICAL_HEAL), NULL)));

        // add swiftmend, whenever anyone is at medium health and has a druid (regrowth, rejuv, wildgrowth)
        triggers.push_back(new TriggerNode(
            "party member low health",
            NextAction::array(0, new NextAction("swiftmend on party", ACTION_CRITICAL_HEAL + 1), NULL)));

        triggers.push_back(new TriggerNode(
            "low health",
            NextAction::array(0, new NextAction("swiftmend", ACTION_CRITICAL_HEAL + 2), NULL)));

        triggers.push_back(new TriggerNode(
            "party member critical health",
            NextAction::array(0, new NextAction("nature's swiftness", ACTION_CRITICAL_HEAL + 4), new NextAction("healing touch on party", ACTION_CRITICAL_HEAL + 3), NULL)));

        triggers.push_back(new TriggerNode(
            "critical health",
            NextAction::array(0, new NextAction("nature's swiftness", ACTION_CRITICAL_HEAL + 6), new NextAction("healing touch", ACTION_CRITICAL_HEAL + 5), NULL)));

        triggers.push_back(new TriggerNode(
            "party member critical health",
            NextAction::array(0, new NextAction("swiftmend on party", ACTION_CRITICAL_HEAL + 7), NULL)));

        triggers.push_back(new TriggerNode(
            "critical health",
            NextAction::array(0, new NextAction("swiftmend", ACTION_CRITICAL_HEAL + 8), NULL)));

        triggers.push_back(new TriggerNode(
            "party member critical health",
            NextAction::array(0, new NextAction("instant healing touch on party", ACTION_CRITICAL_HEAL + 9), NULL)));

        triggers.push_back(new TriggerNode(
            "critical health",
            NextAction::array(0, new NextAction("instant healing touch", ACTION_CRITICAL_HEAL + 10), NULL)));
    }

    // emergencies
    {

        triggers.push_back(new TriggerNode(
            "critical aoe heal",
            NextAction::array(0, new NextAction("tranquility", ACTION_EMERGENCY), NULL)));

        triggers.push_back(new TriggerNode(
            "tree form",
            NextAction::array(0, new NextAction("tree form", ACTION_EMERGENCY + 10), NULL)));

        triggers.push_back(new TriggerNode(
            "medium health",
            NextAction::array(0, new NextAction("barkskin", ACTION_EMERGENCY + 11), NULL)));

        triggers.push_back(new TriggerNode(
            "low mana",
            NextAction::array(0, new NextAction("innervate", ACTION_EMERGENCY + 15), NULL)));
    }
}
