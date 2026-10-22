// 测试
#ifndef _GAME_CHARACTER_H_
#define _GAME_CHARACTER_H_

#include "cocos2d.h"
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

#include "Collision.h" 
#include "my_resources_manage.h"
#include "GameEnums.h"

#include "GameAnimation.h"
#include "GameCharacterBase.h"
   
#include "GameAnimationType.h"


//// 2. 定义枚举到字符串的映射表（用于将枚举值转为可读字符串）
//extern std::unordered_map<ANIMATION_CHARACTER, std::string> animationEnumToString;
//
//// 3. 定义字符串到枚举的映射表（用于从配置/日志中解析枚举值）
//extern std::unordered_map<std::string, ANIMATION_CHARACTER> animationStringToEnum;
//
//// 4. 辅助函数：将枚举转为字符串（带默认值，避免无效枚举）
//extern std::string animationEnumToStr(ANIMATION_CHARACTER anim);
//
//// 5. 辅助函数：将字符串转为枚举（带默认值）
//extern ANIMATION_CHARACTER animationStrToEnum(const std::string& str);



#define GRAVITY 9.81f   // 重力
#define STATE_JUMP_TIME 0.04f
#define STATE_JUMP_FORCE 15.0f
//碰撞类型
//墙
#define  WALL 1   
//栏杆
#define  RAILING 2
//其它3
#define  OPEN_ENEMY_1 3

const int ITEM = 100000;
 
USING_NS_CC;
 
// 设定垂直速度的最大值和最小值
const float MAX_VERTICAL_VELOCITY = 6.0f; // 下落 可根据需要调整
const float MIN_VERTICAL_VELOCITY = -6.2f; // 起跳 可根据需要调整
const float GRAVITY_MIN_COLLISION_VELOCITY = 1.8f; // 重力碰撞的最小值，可根据需要调整

#include <iostream>
using namespace std;

#include "GameParticle.h"

class GameStateMachine;
 

class GameCharacter : public GameCharacterBase,public OBjectParticle {
protected:
	CharacterResources *c_res;

	std::unordered_map<std::string, std::vector<Rect>>  _tmxBox;

	union {
		CHARACTER_STATE __current_state_;
		int currentState_;
	}; 
	union {
		CHARACTER_STATE __previous_state_;
		int preState_  ;
	};

protected: 
	int character_id;
	std::string character_name;
	//道具部分
	int _item = 0;
	//循环的ID
	//std::vector<int> _loopID;
	union {
		ANIMATION_CHARACTER __animation_character__;
		int _animation_id;
	};
	 GameAnimationPlay _animation_play;
 

	// 状态机相关
	 
	class GameStateMachine *stateMachine;
public:
	GameCharacter();
	
	~GameCharacter();
	
	bool init();

// 	int getStateMachine() const;
// 	int getPreStateMachine()const;
// 	void setStateMachine(int state);

	void setStateMachine(GameStateMachine *sm) {
		assert(sm != NULL);
		stateMachine = sm;
	}
 
	void   setPreState(int state);
	void setCurrentState(int state);
	//void setCurState(int state);
	void setCurrentMachine(int state);
	int getCurrentState() const;
	int getPreState() const;

	GameAnimationPlay& getAnimationPlay() { return _animation_play; }
// 	template <typename...Args>
// 	void setLoopId(Args&&...args)
// 	{
// 		int dummy[] = { 0, (_loopID.push_back( std::forward<Args>(args) ), 0)... };
// 		(void)dummy; // 避免未使用变量警告
// 	}
	std::vector<Rect> getCurCollisionRes() {
		return c_res->get_collision_atlas(_curFrameId);
	}
	
 
	void processEvent(int input);

// 	// 角色类中提供状态机的接口方法
// 	void registerState(int state, std::function<void(GameCharacter*)> onEnter = nullptr,
// 		std::function<void(GameCharacter*)> onExit = nullptr,
// 		std::function<void(GameCharacter*, float)> update = nullptr,
// 		std::function<void(GameCharacter*)> handleEvent = nullptr);
// 
// 	template<typename... FilterArgs>
// 	void addTransition( int input, int from, int to,
// 		std::function<bool(GameCharacter*)> condition,
// 		FilterArgs&&... filterArgs) {
// 		stateMachine->addTransition(input, from, to, condition, filterArgs...);
// 	}

	//获取道具
	int  getItemType() {
		return _item;
	}
	int IsCurAnimationId(int animationId) {
		return _animation_id == animationId; 
	}
	int getCurAnimationId() { return _animation_id; }
	//Rect getRailingRect() { return railingRect; }
	// 读取碰撞盒恒定大小，防止相机在切换帧偏移
#include <mutex>

	std::mutex collisionAtlasMutex;
	Size getCurBoxSize() {  
		std::lock_guard<std::mutex> lock(collisionAtlasMutex);
		auto collision = c_res->get_collision_atlas(_curFrameId);
		if (!collision.empty())
		{

			return collision[0].size;
		}
		return Size::ZERO;
		 
	}
	 
	bool AnimationIsRun() {
		return _animation_play.isRun();
	}
	  
	
	//添加资源引用
	//my_resources_manage *_global_my_res_manage;
// 	void addResClass(my_resources_manage *m) {
// 		assert(m != nullptr);
// 		_global_my_res_manage = m;
// 	}
	 
	

	static GameCharacter* create(CharacterResources *res,const std::string &textureName) {
		GameCharacter *sprite = new (std::nothrow) GameCharacter();
		
		if (sprite&&
			/*sprite->init()&&*/
			sprite->addCharRes(res) && sprite->initWithTexture(textureName)) {

			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
 

	static GameCharacter* create(CharacterResources *res ) {

		GameCharacter *sprite = new (std::nothrow) GameCharacter();

		if (sprite&&
			/*sprite->init() && */
			res != nullptr &&sprite->addCharRes(res)) {

			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	

// 	static GameCharacter* create() {
// 		GameCharacter *sprite = new (std::nothrow) GameCharacter();
// 		if (sprite && sprite->init()) {
// 			sprite->autorelease();
// 			return sprite;
// 		}
// 		CC_SAFE_DELETE(sprite);
// 		return nullptr;
// 	}

	static GameCharacter* create(Texture2D *texture) {
		GameCharacter *sprite = new (std::nothrow) GameCharacter();
		if (sprite&&
			/*sprite->init() && */
			sprite->initWithTexture(texture)) {
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	static GameCharacter*  createWithTexture(Texture2D *texture)
	{
		GameCharacter *sprite = new (std::nothrow) GameCharacter();
		if (sprite&& 
			/*sprite->init() && */
			sprite->initWithTexture(texture))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	bool  initWithTexture(const std::string &textureName)//添加一处由资源获取
	{
		CCASSERT(!textureName.empty(), "Invalid texture for sprite");
		Texture2D * texture  = c_res->readTexture(textureName);

		Rect rect = Rect::ZERO;
		if (texture)
			rect.size = texture->getContentSize();

		return Sprite::initWithTexture(texture, rect, false);
	}
// 
	bool  initWithTexture(Texture2D *texture)
	{
		CCASSERT(texture != nullptr, "Invalid texture for sprite");

		Rect rect = Rect::ZERO;
		if (texture)
			rect.size = texture->getContentSize();

		return Sprite::initWithTexture(texture, rect, false);
	}
	 

	virtual void update(float dt);
	 
	float calculateOffsetY(const int& curFrameId, const int& nextFrameId);
	virtual void moveX(float dt);
	virtual void moveY(float dt);
//	void predictNextFrameYCollision(float dt);

	 
// 	void addcollisionBox(std::unordered_map<int, vector<Rect>> *collisionRes) {
// 		collisiion_atlas = collisionRes;
// 	}
// 	void addAttackBox(std::unordered_map<int, vector<Rect>> *attackRes) {
// 		attack_atlas = attackRes;
// 	}
// 	void addAnchorPoint(std::unordered_map<int, Vec2> *anchorPointRes) {
// 		anchor_point = anchorPointRes;
// 	}

	void addTmxBox(std::unordered_map<std::string, std::vector<Rect>> tmxBox) {
		_tmxBox = tmxBox;
	}

	 
	
	// 新增计算帧步长的方法
	Vec2 calculateFrameStep(const Vec2& start, const Vec2& end) {
 
			float totalFrames = static_cast<float>(_animation_play.getTotalAnimationDelay())   ;
			Vec2 displacement = end - start;
			Vec2 step;
			step.x = displacement.x / totalFrames;
			step.y = displacement.y / totalFrames;
			return step;

	}
	//单步不处理任何重力与碰撞
	void runTmxStep() {
		_curAtTmxPos += _m_accelaration_step;
	}
	void setStep(Vec2 step) {
		_m_accelaration_step = step;
	}
	bool lastCollisionIsSide(CollisionSide side,int boxNumber) {
		return lastCollisionResult.side == side && lastCollisionResult.boxNumber == boxNumber;
	}
	Collision::CollisionResult getLastCollisionBox() {
		return lastCollisionResult;
	}
	Vec2 _m_accelaration_step;
	//添加一个记录碰撞结果
	Collision::CollisionResult lastCollisionResult;

	void setCurAnimationId(int animationId);
	// 更新位置
 
	Texture2D * getAnimationTexture(int ani_id,int frame);
	virtual void promoteFrames();
	void setCurTexture(const int &  curFrameId);

	bool addCharRes(CharacterResources * res);
	int getCharId();

	 
	Vec2 getCharacterAtTmxPos()
	{
 
		auto tmxPos = _curAtTmxPos;
		if (
			IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_1) ||
			IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_2) ||
			IsCurAnimationId(ANIMATION_SQUATTING_THROW_ITEM) ||
			IsCurAnimationId(ANIMATION_SQUAT_1) ||
			IsCurAnimationId(ANIMATION_SQUAT_2))
			tmxPos -= Vec2(0,8);//补全偏移方法,让下蹲时滚动正常
		return tmxPos;
	}
private:
	CC_DISALLOW_COPY_AND_ASSIGN(GameCharacter);
};

#endif
