//
//  Behaviour.h
//  EngineTesting
//
//  Created by Hakob on 11/13/14.
//  Copyright (c) 2014 Haksist. All rights reserved.
//

#ifndef EngineTesting_GameLogic_h
#define EngineTesting_GameLogic_h

#include <string>

#include "Component.h"
#include "GameObject.h"
#include "ActiveComponent.h"


class GameLogic :public ActiveComponent
{
	friend class GameObject;
protected:
	~GameLogic(){};
public:
	virtual void OnAction(std::string action, void*const data){};
	void sendAction(std::string action, void*const data);
};

inline void GameLogic::sendAction(std::string action, void*const data)
{
	gameObject->sendAction(action, data);
}


#endif