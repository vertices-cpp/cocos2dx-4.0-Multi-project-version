#include "GameParticle.h"
#include "GameCharacter.h"

GameParticle * GameParticle::getinstance()

{
	static GameParticle _instance;
	return &_instance;
}

int GameParticle::addBind(int typeId, GameCharacter * owner, Node * parent,
	int number, int delay,int life)
 {
	assert(_callBackMethor.find(typeId) != _callBackMethor.end() && _callBackMethor.size());
	// 创建粒子实例 
	 
	int instanceId  = ++_nextInstanceId;
	// 存储实例
	_instances[instanceId] =
	{
		{},
		owner,
			parent,
			_callBackMethor[typeId],
			instanceId,
			true, // 默认激活
			
			number,
			delay,
		 life,
		 
		0,
		0
	};

	auto &item = _instances[instanceId];

	auto methor = _callBackMethor[typeId];

	if (methor.defaultInit) {
		methor.defaultInit(item);
		for (auto node : item.nodes) {
			if (node) {
				parent->addChild(node, item.zOrder);
			//	node->retain();
			}
		}
	} 
	return instanceId;
}
void OBjectParticle::addItem(int PType, int number, int delay, int life)
{
	if (character->getParent() == nullptr)
	{
		return;
	}
	curInstanceId = GameParticle::getinstance()->addBind(PType, character, character->getParent(),
		number, delay, life); 

}
void OBjectParticle::addParticle(int PType,int number, int delay,int life)
{
	if (character->getParent() == nullptr)
	{
		return;
	}
	curInstanceId = GameParticle::getinstance()->addBind(PType, character, character->getParent(),
		number, delay,life);
	particle_instance.push_back(curInstanceId);
	 
}
void OBjectParticle::addParticle(int PType)
{
	if (character->getParent() == nullptr)
	{
		return;
	}
	curInstanceId = GameParticle::getinstance()->addBind(PType, character, character->getParent(),
		1, 0, 0);
	particle_instance.push_back(curInstanceId);

}
void OBjectParticle::removeParticle()
{
	for(auto &i: particle_instance)
		GameParticle::getinstance()->removeBind(i);
	particle_instance.clear();
}
void OBjectParticle::removePreParticle()
{ 
	auto item = find(particle_instance.begin(), particle_instance.end(), curInstanceId);
	if (item!=particle_instance.end())
	{
		GameParticle::getinstance()->removeBind(curInstanceId);
		particle_instance.erase(item);
	}
	
}

void OBjectParticle::releaseParticle(int particle_id)
{
	 
	for (auto beg= particle_instance.begin();
		beg!= particle_instance.end();)
	{
		if (*beg == particle_id)
		{
			GameParticle::getinstance()->stopInstance(particle_id);
			beg = particle_instance.erase(beg);
		}
		else
			++beg;
	}
}
 
void OBjectParticle::releasePreParticle()
{

	for (auto beg = particle_instance.begin();
		beg != particle_instance.end();)
	{
		if (*beg == curInstanceId)
		{
			GameParticle::getinstance()->stopInstance(curInstanceId);
			beg = particle_instance.erase(beg);
		}
		else
			++beg;
	}
}