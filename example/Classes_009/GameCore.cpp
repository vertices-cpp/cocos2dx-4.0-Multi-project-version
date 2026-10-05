#include "GameCore.h"
#include <iostream>

GameCore* GameCore::ret = nullptr;



GameCore::GameCore() : _tileMap(nullptr),   _mapInfo(nullptr) 
{
	// 初始化操作
}

GameCore::~GameCore()
{
	free();
}

void GameCore::free() {
	_box_text.clear();
	_map_collision_objs.clear();
	_boxr.clear();
	for (auto node : _boxp) {
		if (node) {
			node->removeFromParent();
			node->release();
		}
	}
	_boxp.clear();
	if (_mapInfo) {
		_mapInfo->removeFromParent();
		_mapInfo->release();
	}
}

GameCore* GameCore::getInstance()
{ 
	if (ret == nullptr)
	{
		  ret = new (std::nothrow) GameCore();
		  ret->autorelease();
	}
	
	return ret;
}

bool GameCore::init()
{
	// scheduleUpdate();
	return true;
}

void GameCore::setTmx(const std::string & tmx_file, const int& tag)
{
	if (_tileMap != nullptr)
	{
		setTmxMouseListenerMov(false);
		removeChild(_tileMap);
	}
	_tileMap = TMXTiledMap::create(tmx_file);
	_mapPos = Vec2::ZERO;
	_mapSize = _tileMap->getContentSize();
	_tileMap->setLocalZOrder(tag);
	addChild(_tileMap,1);
	_tileMap->setAnchorPoint(Vec2(0, 1));
	_tileMap->setPosition(_director->convertToGL(Vec2(0, 0)));

	// 添加这个地图的碰撞盒
	TMXObjectGroup *obj = _tileMap->getObjectGroup("collision");
	ValueVector& objects = obj->getObjects();

	for (auto &objectValue : objects)
	{
		const ValueMap &object = objectValue.asValueMap();
		std::string objName;
		Rect rect;

		if (object.find("name") != object.end())
		{
			objName = object.at("name").asString();

			if (!objName.size())
				objName = "1";

			if (object.find("x") != object.end() &&
				object.find("y") != object.end() &&
				object.find("width") != object.end() &&
				object.find("height") != object.end())
			{
				rect.origin.x = object.at("x").asFloat();
				rect.origin.y = object.at("y").asFloat();
				rect.size.width = object.at("width").asFloat();
				rect.size.height = object.at("height").asFloat();
			}

			auto it = _map_collision_objs.find(objName);
			if (it != _map_collision_objs.end())
			{
				it->second.push_back(rect);
			}
			else
				_map_collision_objs[objName].push_back(rect);
		}
	}

	for (auto &i : _map_collision_objs)
	{
		for (Rect &j : i.second)
		{
			Vec2 v = j.origin;
			v.y = (_mapSize.height - (v.y + j.size.height));

			auto d = DrawNode::create();
			d->drawRect(Vec2::ZERO, j.size, Color4F::WHITE);
			addChild(d, 200);
			_boxp.push_back(d);
			_boxr.push_back(v);
			d->setContentSize(j.size);
			d->setAnchorPoint(Vec2(0, 1));
		}
	}

	Collision::getInstance()->resizeGindicate(_boxr.size());//碰撞颜色

	for (size_t i = 0; i < _boxp.size(); ++i)
	{
		auto label = Label::createWithTTF(TOSTRING(_boxp[i]->getPosition()), "fonts/Marker Felt.ttf", 12);

		if (label == nullptr)
		{
			// problemLoading("'fonts/Marker Felt.ttf'");
		}
		else
		{
			// position the label on the center of the screen
			label->setPosition(Vec2(0, _director->getWinSize().height - 60));
			label->setAnchorPoint(Vec2(0, 1));

			this->addChild(label, 1);
			_box_text.push_back(label);

			auto textlistener = [&](EventCustom *event)
			{
				size_t cur = atoi(event->getEventName().c_str());
				Vec2 pos1 = _boxp[cur]->getPosition();
				Vec2 pos2 = _boxr[cur];

				//转成UI坐标
				auto tmp_pos1 = _director->convertToUI(pos1);
				auto tmp_pos2 = pos2;
				_box_text[cur]->setString(TOSTRING(tmp_pos2) + "#" + TOSTRING(tmp_pos1));
				_box_text[cur]->setPosition(pos1);

				// 通过 Collision 类的公共方法访问 Gindicate
				const auto& gindicate = Collision::getInstance()->getGindicate();
				if (cur < gindicate.size() && gindicate[cur])
				{
					_boxp[cur]->clear();
					_boxp[cur]->setLineWidth(3);
					_box_text[cur]->setTextColor(Color4B::RED);
					_boxp[cur]->drawRect(Vec2::ZERO, _boxp[cur]->getContentSize(), Color4F::RED);
				}
				else {
					_boxp[cur]->clear();
					_boxp[cur]->setLineWidth(2);
					_box_text[cur]->setTextColor(Color4B::WHITE);
					_boxp[cur]->drawRect(Vec2::ZERO, _boxp[cur]->getContentSize(), Color4F::WHITE);
				}
			};
			_eventDispatcher->addCustomEventListener(to_string(i), textlistener);
		}
	}
}

void GameCore::setTmxMouseListenerMov(bool enable)
{
	if (enable != mouseEventstate && enable)
	{
		mouseEventstate = true;

		std::string vec2str = TOSTRING(_tileMap->getPosition());

		_mapInfo = Label::createWithTTF(vec2str, "fonts/Marker Felt.ttf", 24);

		if (_mapInfo == nullptr)
		{
			// problemLoading("'fonts/Marker Felt.ttf'");
		}
		else
		{
			// position the label on the center of the screen
			_mapInfo->setPosition(Vec2(0, _director->getWinSize().height - 160));
			_mapInfo->setAnchorPoint(Vec2(0, 1));
			// add the label as a child to this layer
			this->addChild(_mapInfo, 1);
			auto textlistener = [&](EventCustom *event)
			{
				_mapInfo->setString(TOSTRING(_mapPos));
			};
			_eventDispatcher->addCustomEventListener("curPos", textlistener);
		}
	}
	else if (enable != mouseEventstate && mouseEventstate)
	{
		_eventDispatcher->removeEventListenersForTarget(this);
		if (_mapInfo != nullptr)
		{
			removeChild(_mapInfo);
			_mapInfo = nullptr;
		}
		mouseEventstate = enable;
	}
}

TMXLayer* GameCore::getTmxLayer(const std::string &layerName) {
	return _tileMap->getLayer(layerName);
}

TMXObjectGroup *  GameCore::getObjectName(const std::string &objName)
{
	return  _tileMap->getObjectGroup(objName);
}

bool GameCore::setLayerVisable(const std::string layer_name, bool visible)
{
	auto layer = _tileMap->getLayer(layer_name);
	if (layer == nullptr)
	{
		return false;
	}
	layer->setLocalZOrder(10);
	layer->setVisible(visible);

	return true;
}

void GameCore::initMyMap(const std::string &tmp) {

	setTmx(tmp);
	getTmxLayer("Layer1")->setLocalZOrder(100);
	//设置地图偏移
//	 game_scene->setTmxPos(Vec2(0, -190));
	setTmxMouseListenerMov(true);
	//game_scene->setShake(1.5, Vec2(0, 20));
   //  game_scene->setTmxTintToColor(1.5, Color3B(100, 0, 100), Color3B(255, 255, 255));
	setScroll(1, 1);//只设置水平滚动
  // this->setPosition(Vec2(0, 0x65));
// 	 auto shaky3D = cocos2d::Shaky3D::create(5, Size(10, 10), 15, false);
// 	 game_scene->runAction(shaky3D);
}

 

void GameCore::initMyChar(const std::string &charPath){

	// init_res();
	my_char1 = GameCharacter::create(my_resources_manage::getInstance()->getCharacterResources(charPath));//将自动加载资源

	addChild(my_char1, 4);

	my_char1->setCurSize(Size(16, 32));
	my_char1->setMoveVel(100, 50);
	my_char1->setGravityEnable(true);

	my_char1->addTmxBox(getMapTotalBox());

	// my_char->setCurStateId(1);

	auto sTable = StateTable::getInstance();
	my_char1->setStateMachine(sTable->get_character_state(1));
	
	// 添加状态

	my_char1->setCurrentMachine(STATE_IDLE);
	//设置支持循环的ID
	//my_char1->setLoopId(ANIMATION_IDLE, ANIMATION_RUN, ANIMATION_SQUAT_1, ANIMATION_JUMP, ANIMATION_HANGING);

	my_char1->setFlippedX(true);
	my_char1->setOffsetVal(Vec2(8, -16));

	my_char1->setAtTmxPos(Vec2(572, 234));// my_char->getContentSize().height * 2 + 16));
   
  
	my_char1->bindKey(EventKeyboard::KeyCode::KEY_W, OP_UP,
		EventKeyboard::KeyCode::KEY_S, OP_DOWN,
		EventKeyboard::KeyCode::KEY_A, OP_LEFT,
		EventKeyboard::KeyCode::KEY_D, OP_RIGHT,
		EventKeyboard::KeyCode::KEY_K, OP_ATTACK,
		EventKeyboard::KeyCode::KEY_L, OP_JUMP,
		EventKeyboard::KeyCode::KEY_J, OP_SELECT
	);

	auto my_char1_keyboard = cocos2d::EventListenerKeyboard::create();

	my_char1_keyboard->onKeyPressed = [&](EventKeyboard::KeyCode key, Event *e)
	{
		if (my_char1->findKeyTable(key))
		{

			my_char1->setKeyVal(key);
		 
		} 
	};
	my_char1_keyboard->onKeyReleased = [&](EventKeyboard::KeyCode key, Event *e)
	{
		if (my_char1->findKeyTable(key))
		{

			my_char1->setUnKeyVal(key);
	 
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(my_char1_keyboard, my_char1);


}

#include "BehaviorTreeManager.h"
//#include "init_enemy_machine.h"

void GameCore::initEnemy(const std::string &EnemyPath) {

	auto betree = BehaviorTreeManager::getInstance();
	betree->createTree();

// 
// 	for (auto &i : enemy_container)
// 	{
		char enemy_name[1024];
		memset(enemy_name, 0, 1024);
		int No = 1;

		sprintf(enemy_name, "character/enemy-%.3d.tmj", No);
		Enemy* enemy_ = Enemy ::create(
			my_resources_manage::getInstance()->getCharacterResources(enemy_name));//将自动加载资源
		enemy_container.push_back(enemy_);

		addChild(enemy_, 4);

		enemy_->setBtree(betree); 
		enemy_->setID(ENEMY_001);
	//	enemy1_init(enemy_);


		auto sTable = StateTable::getInstance(); 
		enemy_->setStateMachine(sTable->get_character_state(ENEMY_001));
		enemy_->setCurrentMachine(STATE_IDLE);


		if (my_char1)
			enemy_->set_play(my_char1);
		if (my_char2)
			enemy_->set_play(my_char2);

		enemy_->setCurSize(Size(16, 32));
		enemy_->setMoveVel(90, 30);
		enemy_->setGravityEnable(true);

		enemy_->addTmxBox(getMapTotalBox());
		 

		enemy_->setFlippedX(true);
		enemy_->setOffsetVal(Vec2(8, -16));

		enemy_->setAtTmxPos(Vec2(572, 234));
		//enemy_->setLoopId(ANIMATION_IDLE, ANIMATION_RUN, ANIMATION_JUMP );
		enemy_->setCurAnimationId(ANIMATION_IDLE);
	//}
}

void GameCore::update(float dt)
{
	TmxAnimationPlay(dt);

	setTmxPos(_mapPos);
	_eventDispatcher->dispatchCustomEvent("curPos", nullptr);

	my_char1->update(dt);//更新角色操作，纹理等
	my_char1->setPos(getTmxPos());//设置角色所在地图（及屏幕）的位置
// 	my_char2->update(dt);//更新角色操作，纹理等
// 	my_char2->setPos(game_scene->getTmxPos());//设置角色所在地图（及屏幕）的位置

	for (auto &i : enemy_container) {
		i->update(dt);
		i->setPos(getTmxPos());

	}

	setCamera(my_char1, my_char2);//设置地图的滚动部分  
	 
	GameParticle::getinstance()->update();
}

static int curLayer = 0;
const char *layerArray[] = { "ani1","ani2","ani3" }; int cnt = 0;
void GameCore::TmxAnimationPlay(float delta)
{
	cnt++;
	if (cnt > 3)
	{
		cnt = 0;


		//	my_char->setNextName("jmp-attack");
// 
		setLayerVisable(layerArray[curLayer], false);
		curLayer++;
		if (curLayer >= 3)
			curLayer = 0;
		setLayerVisable(layerArray[curLayer], true);


		//	container->setOpacity(b+=10);
	}
}

void GameCore::setTmxPos(const Vec2 & pos)
{
	_mapPos = pos;
	auto nodePos = _director->convertToGL(pos);
	_tileMap->setPosition(nodePos);
}