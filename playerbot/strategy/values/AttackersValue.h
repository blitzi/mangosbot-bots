#pragma once
#include "../Value.h"
#include "TargetValue.h"
#include "NearestUnitsValue.h"

namespace ai
{
    class AttackersValue : public ObjectGuidListCalculatedValue
	{
	public:
        AttackersValue(PlayerbotAI* ai) : ObjectGuidListCalculatedValue(ai, "attackers", 2) {}
        list<ObjectGuid> Calculate();

	private:
        void AddAttackersOf(Group* group, set<Unit*>& targets);
        void AddAttackersOf(Player* player, set<Unit*>& targets);
        void AddAttackersOf(Unit* pet, set<Unit*>& targets);
		void RemoveNonThreating(set<Unit*>& targets);
		void RemoveNonEliteTargets(set<Unit*>& targets);
		void RemoveNonTotemTargets(set<Unit*>& targets);
		void RemoveNonFocusTargets(set<Unit*>& targets);
		void RemoveNonRtiTargets(set<Unit*>& targets);
        void RemoveAlreadyTankedTargets(set<Unit*>& targets, Player* bot);
        bool ListContainsElite(set<Unit*>& targets) const;
        bool ListContainsFocusTarget(set<Unit*>& targets) const;
        bool ListContainsTotem(set<Unit*>& targets) const;
        bool ListContainsRti(set<Unit*>& targets) const;

    public:
        static bool IsPossibleTarget(Unit* attacker, Player *bot);
        static bool IsRti(Unit* enemy, Player* bot);
        static bool IsValidTarget(Unit* attacker, Player *bot);
    };

    class PossibleAddsValue : public BoolCalculatedValue
    {
    public:
        PossibleAddsValue(PlayerbotAI* const ai) : BoolCalculatedValue(ai) {}
        virtual bool Calculate();
    };
}
