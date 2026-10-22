
#include "HelloWorldScene.h"

#include "GameStateMachine.h"
  
#include "GameCharacter.h"

//#include "init_character_state.h"

 
#include <fstream>
#include <string>
#include <iostream>   
#include <unordered_map>

Size visibleSize;


float _angle = 0;
// typedef struct  BindKey {
// 	EventKeyboard::KeyCode key;
// 	int virtualKey;
// }BindKey;

my_resources_manage * HelloWorld::global_resources = nullptr;
StateTable *HelloWorld::stateMachine = nullptr;

// static unordered_map< EventKeyboard::KeyCode, int> bindKey = {
// 	{EventKeyboard::KeyCode::KEY_W,OP_UP},
// 	{EventKeyboard::KeyCode::KEY_S,OP_DOWN},
// 	{EventKeyboard::KeyCode::KEY_A,OP_LEFT},
// 	{EventKeyboard::KeyCode::KEY_D,OP_RIGHT},
// 	{EventKeyboard::KeyCode::KEY_K,OP_ATTACK},
// 	{EventKeyboard::KeyCode::KEY_L,OP_JUMP},
// 	{EventKeyboard::KeyCode::KEY_J,OP_SELECT}
// };
static unordered_map< EventKeyboard::KeyCode, string> keyString = {
	{EventKeyboard::KeyCode::KEY_W,u8"上"},
	{EventKeyboard::KeyCode::KEY_S,u8"下"},
	{EventKeyboard::KeyCode::KEY_A,u8"左"},
	{EventKeyboard::KeyCode::KEY_D,u8"右"},
	
	{EventKeyboard::KeyCode::KEY_K,u8"攻击"},
	{EventKeyboard::KeyCode::KEY_L,u8"跳"},
	{EventKeyboard::KeyCode::KEY_J,u8"选择键"} 
};

static unordered_map< EventKeyboard::KeyCode, int> keytoNum = {
	{EventKeyboard::KeyCode::KEY_W,0},
	{EventKeyboard::KeyCode::KEY_S,1},
	{EventKeyboard::KeyCode::KEY_A,2},
	{EventKeyboard::KeyCode::KEY_D,3},
	{EventKeyboard::KeyCode::KEY_K,4},
	{EventKeyboard::KeyCode::KEY_L,5},
	{EventKeyboard::KeyCode::KEY_J,6}
};
static unordered_map< int ,EventKeyboard::KeyCode> NumToKey = {
	{0,EventKeyboard::KeyCode::KEY_W},
	{1,EventKeyboard::KeyCode::KEY_S},
	{2,EventKeyboard::KeyCode::KEY_A},
	{3,EventKeyboard::KeyCode::KEY_D},
	{4,EventKeyboard::KeyCode::KEY_K},
	{5,EventKeyboard::KeyCode::KEY_L},
	{6,EventKeyboard::KeyCode::KEY_J}
};

Label *keyInfo[7];

#define PLAY1 1

std::vector< GameCharacter*> enemy;


HelloWorld::HelloWorld()
{
// 	 
	CREATE_DEBUG_CONSOLE;

}

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
   
#include "init_particle.h"
#include "init_character_machine.h"
#include "init_enemy_machine.h"

bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	} 
 

 	 
 	auto winsize = Director::getInstance()->getWinSize();

	 visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	global_resources = my_resources_manage::getInstance();

	game_scene = GameCore::getInstance();
	addChild(game_scene, 0);


	global_resources->addJsonTexture("character/character-001.tmj");
	global_resources->addJsonAnimationJson("character/character-001.json");

	global_resources->addJsonTexture("character/character-002.tmj");
	global_resources->addJsonAnimationJson("character/character-002.json");

	global_resources->addJsonTexture("character/enemy-001.tmj");
	global_resources->addJsonAnimationJson("character/enemy-001.json");


	init_particle();

	stateMachine = StateTable::getInstance();

	init_character_machine();
	init_enemy_machine();

	//创建状态机
	//state_container = StateContainer::getInstance();
	//init_character_state(1);
	//创建行为树

	BehaviorTreeManager::getInstance()->createTree();

	auto label = Label::createWithTTF("Hello World", "e:/sarasa-fixed-sc-regular.ttf", 15);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height * 6.0f));

		// add the label as a child to this layer
		addChild(label, 1);
		auto textlistener = EventListenerMouse::create();
		textlistener->onMouseMove = [&](EventMouse *event)
		{
			auto pos = event->getLocation();
			auto label = (Label*)event->getCurrentTarget();
			auto scene_pos = game_scene->getPosition();

			Vec2 curTmx = game_scene->getTmxPos();

			Vec2 curTmxPos = Vec2(
				(-scene_pos.x) + (-curTmx.x),
				scene_pos.y + (-curTmx.y)  // 显式翻转Y轴
			) + pos;
			label->setString(u8"鼠标坐标：地图" + TOSTRING(curTmxPos) + u8"/屏幕" + TOSTRING(pos) /*+ u8"/ 屏幕" + TOSTRING(pos)*/);

		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(textlistener, label);
	}
	 
	
	game_scene->initMyMap("game_map/level01_01_tmj.tmx");
	game_scene->initMyChar("character/character-001.tmj");
	game_scene->initEnemy("character/enemy-001.tmj"); 
  	
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = [&](const std::vector<Touch*>& touches, Event  *event)
	{
		auto touch = touches[0];

		auto diff = touch->getDelta();
		diff.x = (float)diff.x, diff.y = (float)diff.y;
		auto node = /*(GameCore*)*/event->getCurrentTarget();
		auto currentPos = node->getPosition();
		node->setPosition(currentPos + diff);
		// node->setTmxPos(currentPos + diff);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, game_scene);

	scheduleUpdate();
	
	return true;
}


 
void HelloWorld::update(float dt)
{   
	game_scene->update(dt);

	
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

