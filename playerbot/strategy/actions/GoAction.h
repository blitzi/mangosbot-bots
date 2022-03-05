#pragma once

#include "../Action.h"
#include "MovementActions.h"

namespace ai
{
	class GoAction : public MovementAction {
	public:
		GoAction(PlayerbotAI* ai) : MovementAction(ai, "Go") {}
		virtual bool Execute(Event event);
		virtual bool IsPossible() { return true; }
		virtual bool IgnoresCasting() { return true; }
	};
}
