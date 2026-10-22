
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#pragma once

#include "cocos2d.h"

#include "GameCore.h"
#include "my_resources_manage.h"
#include "GameCharacter.h"
 

#include <unordered_map>

USING_NS_CC;

 
class HelloWorld : public cocos2d::Scene
{
 
	static my_resources_manage *global_resources; //资源总类
	static StateTable *stateMachine;

	GameCore *game_scene;  //场景总类
	 
public:
	HelloWorld();
    static cocos2d::Scene* createScene();
	GameCore*  getGscene() { return game_scene; }
 



	virtual bool init();

	void update(float delta);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
 
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	 
};

#endif // __HELLOWORLD_SCENE_H__
