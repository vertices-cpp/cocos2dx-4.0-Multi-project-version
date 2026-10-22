#ifndef _GAME_PARTICLE_H_
#define  _GAME_PARTICLE_H_



#include "cocos2d.h"
#include <unordered_map>
#include <vector>
#include <functional>

USING_NS_CC;

// 前向声明 GameCharacter 类
class GameCharacter;

class GameParticle;

struct ParticleMethor;
 

struct MyUserData {//需要这个？
	int curlife;
	int life;
	int stop;
};
 


struct ParticleInstance;
// 定义回调函数类型别名
using InitCallback = std::function<void(ParticleInstance&)>;
using StopCallback = std::function<void(ParticleInstance&)>;
using UpdateCallback = std::function<bool(ParticleInstance&)>;


// 粒子模板配置（不包含实例）
struct ParticleMethor {
	InitCallback defaultInit;
	UpdateCallback defaultUpdate;
	StopCallback defaultStop;
};

	// 粒子实例结构（每个角色独立）
	struct ParticleInstance {

		std::vector< Node*> nodes;
		
		GameCharacter* owner = nullptr;
		cocos2d::Node* parentNode = nullptr;
		ParticleMethor methor;

		int intstanceId;
		bool isActive;
		
		

		int number;//总数
		int delay;//间隔创建
		int life;//每个生命值 
		
		int cur_delay;//
		int zOrder;
	};


struct  GameParticle {
	
	

	std::unordered_map<int, ParticleMethor> _callBackMethor;
	std::vector<int> _pendingRemovals;
	// 激活的粒子实例（key: 实例唯一ID）
	std::unordered_map<int, ParticleInstance> _instances;

	int  _nextInstanceId = 1;
public:
	static GameParticle*  getinstance();
	~GameParticle() {
		 
		_callBackMethor.clear();
		_instances.clear();
	}
	// 注册粒子类型配置（不创建实例）
	void registerParticleType(int typeId,
		InitCallback init = nullptr,
		UpdateCallback update = nullptr,
		StopCallback stop = nullptr) {


		assert(_callBackMethor.find(typeId) == _callBackMethor.end());
		_callBackMethor[typeId] = { init, update, stop };
	}

	//  创建粒子实例（返回实例ID）
	int addBind(int typeId, GameCharacter* owner, Node *parent,
		int number, int delay,int life) ;
	void stopInstance(int instanceId)
	{
		auto it = _instances.find(instanceId);
		if (it!=_instances.end())
		{
			it->second.isActive = false;
		}
	}
	void resumeInstance(int instanceId)
	{
		auto it = _instances.find(instanceId);
		if (it != _instances.end())
		{
			it->second.isActive = true;
		}
	}
	void pauseInstance(int instanceId,bool enable)
	{
		auto it = _instances.find(instanceId);
		if (it != _instances.end())
		{
			auto& nodes = it->second.nodes;
			for (auto &node : nodes)
			{
				if (auto particle = dynamic_cast<ParticleSystemQuad*>(node))
				{
					if (enable)
						particle->pauseEmissions();
					else
						particle->resumeEmissions();
				}
				else if (auto sprite = dynamic_cast<Sprite*>(node))
				{
					// 处理普通精灵的暂停逻辑
					sprite->setVisible(!enable);
				}
			}
		//	it->second.isActive = false;
		}
	}
	// 停用粒子直接删
	void removeBind(int instanceId) {
		auto it = _instances.find(instanceId);
		if (it != _instances.end()) {
			auto& inst = it->second;
			if (inst.methor.defaultStop) {
				inst.methor.defaultStop(inst);
			}
			for (auto node : inst.nodes) {
				if (node && inst.parentNode) {
					//node->release();
					inst.parentNode->removeChild(node);
					
				}
			}
			_instances.erase(it);
		}
	} 
	// 更新所有粒子实例
	void update() {

		// 使用 C++11 兼容的遍历方式
		for (auto& entry : _instances) { // 去结构化绑定
			auto& inst = entry.second;      // 值

			//if (!inst.isActive) continue; // 已停用的实例跳过

			if (  inst.methor.defaultUpdate) {
				// 执行自定义更新逻辑
				bool removeEvent = inst.methor.defaultUpdate(inst);//我在里面自已设置呢
				if (removeEvent)
				{
					_pendingRemovals.push_back(inst.intstanceId);
				}
			}
		}
		if (_pendingRemovals.size())
		{
			for (auto &id : _pendingRemovals)
			{
				removeBind(id);
			}
			_pendingRemovals.clear();
		}
	}
	

};
 
class OBjectParticle {//用来作为基类继承的类
	std::vector<int> particle_instance;
	void *custom_data;
	GameCharacter *character;
	int curInstanceId, savInstanceId;
protected:
	OBjectParticle(GameCharacter *character):character(character), curInstanceId(0), savInstanceId(0) {}
	
	~OBjectParticle() {
// 		for (auto &id : particle_id)
// 			GameParticle::getinstance()->removeBind(id);
		particle_instance.clear();
		
	}
public:
	int get_particle_id_size() { return particle_instance.size(); }
	void setCustomData(void *data) { custom_data = data; }
	void *getCustomData() { return custom_data; }
	void addItem(int PType, int number, int delay, int life);
	void addParticle(int PType,int number, int delay,int life );
	void addParticle(int PType);
	void removeParticle();
	void removePreParticle();
	void releaseParticle(int particle_id);
	void releasePreParticle();
	int getSaveId() { return savInstanceId; }
	void toSavId() { savInstanceId = curInstanceId; }
	bool  isPInstance(int InstanceId)
	{
		return  find(particle_instance.begin(), particle_instance.end(), InstanceId) != particle_instance.end();
	}
	void pausePartilce(int particle_id,bool enable)
	{
		auto instance = GameParticle::getinstance();
		instance->pauseInstance(particle_id, enable);
	}
	//仅通知
 
};


#endif // !_GAME_PARTICLE_H_