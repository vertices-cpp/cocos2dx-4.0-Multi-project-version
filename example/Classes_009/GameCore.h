#ifndef _GAME_CORE_H_
#define _GAME_CORE_H_

#include "cocos2d.h"

#include <string>
#include <vector>
#include <unordered_map>


#include "GameCharacter.h"
#include "Enemy.h"

USING_NS_CC;

 

class GameCharacter;


class GameCore :public NodeGrid {
	bool mouseEventstate = false;
	Label* _mapInfo;
	TMXTiledMap *_tileMap;
	Vec2 _mapPos = Vec2::ZERO;
	Size _mapSize = Size::ZERO;

	std::unordered_map<std::string, std::vector<Rect>> _map_collision_objs;
	std::vector<Label*> _box_text; 
	
	std::vector<DrawNode*> _boxp;
	std::vector<Vec2> _boxr;
	

	Vec2 _scroll = Vec2::ZERO;

public:
	GameCore();
	~GameCore();
	void free();
	static GameCore *ret ;
	static GameCore* getInstance();
	bool init();

	Vec2 getTmxPos(){ return _mapPos; }
	Size getMapSize() { return _mapSize; }
	void setScroll(float h, float v){
		_scroll = { h,v };
	}
 
	
	void sTopAction() { stopAllActions(); }
	void setShake(float duration, Vec2 &deltaPosition) {

		runAction(RepeatForever::create(
			cocos2d::Sequence::create(
				MoveBy::create(3.8f, deltaPosition),
				MoveBy::create(3.8f, -deltaPosition),
				nullptr
			)
			));
	}
	void setTmxTintToColor(float duration, Color3B src,Color3B dst) {

		if (_tileMap)
			_tileMap->runAction(
				RepeatForever::create(
					cocos2d::Sequence::create(
						TintTo::create(duration, src.r, src.g, src.b),
						TintTo::create(duration, dst.r, dst.g, dst.b),
						nullptr
					)
				)
			);
	}
	void TmxAnimationPlay(float delta);
	void setTmxPos(const Vec2 & pos);
	void setTmx(const std::string & tmx_file,const int& tag = 0);
	void setTmxMouseListenerMov(bool enable);
	TMXLayer * getTmxLayer(const std::string &layerName );
	TMXObjectGroup * getObjectName(const std::string & objName);
	bool setLayerVisable(const std::string layer_name, bool visible);
	void initMyMap(const std::string & tmp);
	void initMyChar(const std::string &charPath);
	void initEnemy(const std::string &EnemyPath);
	void update(float delta);

	
	void setCamera(GameCharacter* obj1, GameCharacter *obj2)
	{
		Vec2 screen = _director->getVisibleSize();
		//这里改成仅使用碰撞框大小
		Vec2 curDotPos = Vec2::ZERO; 
		if (obj1 != nullptr &&obj2 != nullptr)
			curDotPos = ((obj1->getCharacterAtTmxPos() + obj1->getCurSize() / 2) +
			(obj2->getCharacterAtTmxPos() + obj2->getCurSize() / 2))/2;
		else if (obj1 != nullptr)
			curDotPos = (obj1->getCharacterAtTmxPos() + obj1->getCurSize() / 2);
		else if(obj2 != nullptr)
			curDotPos = (obj2->getCharacterAtTmxPos() + obj2->getCurSize() / 2);

		if (_scroll.x)
			_mapPos.x = curDotPos.x - screen.x / 2;
		if (_scroll.y)
			_mapPos.y = curDotPos.y - screen.y / 2;

		if (_mapPos.x < 0)
		{
			_mapPos.x = 0;
		}
		if (_mapPos.x > _mapSize.width - screen.x)
		{
			_mapPos.x = _mapSize.width - screen.x;
		}
		if (_mapPos.y < 0)
		{
			_mapPos.y = 0;
		}

		if (_mapPos.y > _mapSize.height - screen.y)
		{
			_mapPos.y = _mapSize.height - screen.y;
		}
		_mapPos = -_mapPos;

		for (size_t i = 0; i < _boxr.size(); ++i)
		{
			auto r = _boxr[i];
			r.y = _director->getVisibleSize().height - r.y;
			auto newBoxPos = _mapPos;
			auto newPos = _mapPos + r;
			newBoxPos.y = -newBoxPos.y;
			newBoxPos += r;

			_boxp[i]->setPosition(newBoxPos);
			_eventDispatcher->dispatchCustomEvent(to_string(i), nullptr);
			Collision::getInstance()->setGindicate(i, 0);
		}
	}

	std::unordered_map<std::string, std::vector<Rect>> getMapTotalBox() {

		auto filpYBox = _map_collision_objs;
		for (auto &i: filpYBox)
		{
			for (auto &j:i.second)
			{//转换Y轴
				j.origin.y = (_mapSize.height - (j.origin.y + j.size.height));
			}
		}

		return filpYBox;
	}

	GameCharacter* my_char1 = nullptr;//对象类
	GameCharacter* my_char2 = nullptr;//对象类
	std::vector< Enemy*> enemy_container;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(GameCore);
};


//extern GameCore *gs;

#endif
