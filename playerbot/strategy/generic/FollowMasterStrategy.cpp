#include "botpch.h"
#include "../../playerbot.h"
#include "FollowMasterStrategy.h"
#include "../../ServerFacade.h"
#include "../actions/GenericSpellActions.h"
#include "../actions/MovementActions.h"
#include "../actions/ChooseTargetActions.h"
#include <playerbot/strategy/actions/ReachTargetActions.h>
#include <playerbot/strategy/actions/UseItemAction.h>

using namespace ai;

float FollowMultiplier::GetValue(Action* action)
{
	if (action == NULL) return 1.0f;

	string name = action->GetName();
	Unit* master = ai->GetMaster();

	if (!master)
		return 1.0f;

	float distanceToMaster = sServerFacade.GetDistance2d(bot, master);
	bool teamIsMoving = bot->IsMoving() || (master->IsMoving() && distanceToMaster > sPlayerbotAIConfig.followDistance);
	
	if (name == "set facing")
		return teamIsMoving ? 0 : 1;

	if (dynamic_cast<ReachTargetAction*>(action))
		return 0;
		//return teamIsMoving ? 0 : 1;

	if (name == "mount")
		return 1.0f;

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
	
		return castTime == 0 ? 1 : (teamIsMoving ? 0 : 1);
	}

	return 1.0f;
}

void FollowMasterStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	triggers.push_back(new TriggerNode(
		"update",
		NextAction::array(0, new NextAction("follow", 1), NULL)));
}

void FollowMasterStrategy::InitMultipliers(std::list<Multiplier*>& multipliers)
{
	multipliers.push_back(new FollowMultiplier(ai));
}