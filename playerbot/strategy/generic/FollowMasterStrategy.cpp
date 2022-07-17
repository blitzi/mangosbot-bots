#include "botpch.h"
#include "../../playerbot.h"
#include "FollowMasterStrategy.h"
#include "../../ServerFacade.h"
#include "../actions/GenericSpellActions.h"
#include "../actions/MovementActions.h"
#include "../actions/ChooseTargetActions.h"

using namespace ai;

float FollowMultiplier::GetValue(Action* action)
{
	if (action == NULL) return 1.0f;

	string name = action->GetName();
	
	if (name == "set facing")
		return bot->IsMoving() ? 0 : 1;

	if (dynamic_cast<MovementAction*>(action))
	{
		if (name == "follow")
			return bot->IsMoving() ? 0 : 1;

		return dynamic_cast<AttackAction*>(action) ? 1 : 0;
	}

	if (dynamic_cast<CastRangeSpellAction*>(action))
	{
		uint32 spellId = AI_VALUE2(uint32, "spell id", name);
		const SpellEntry* const pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
		if (!pSpellInfo) return 1.0f;

		uint32 castTime = GetSpellCastTime(pSpellInfo
#ifdef CMANGOS
			, bot
#endif
		);
	
		return castTime == 0 ? 1 : (bot->IsMoving() ? 0 : 1);
	}

	return 1.0f;
}

void FollowMasterStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	triggers.push_back(new TriggerNode(
		"update",
		NextAction::array(0, new NextAction("follow", 1), NULL)));

    triggers.push_back(new TriggerNode(
        "out of react range",
        NextAction::array(0, new NextAction("flee to master", ACTION_HIGH), NULL)));
}

void FollowMasterStrategy::InitMultipliers(std::list<Multiplier*>& multipliers)
{
	multipliers.push_back(new FollowMultiplier(ai));
}