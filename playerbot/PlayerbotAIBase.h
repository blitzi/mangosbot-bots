#pragma once

class Player;
class PlayerbotMgr;
class ChatHandler;

using namespace std;

class PlayerbotAIBase
{
public:
	PlayerbotAIBase();

public:
    virtual void UpdateAI(uint32 elapsed);
    virtual void UpdateAIInternal(uint32 elapsed) = 0;

protected:
};
