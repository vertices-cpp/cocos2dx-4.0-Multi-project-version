#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"
USING_NS_CC;

#include "my_resources_manage.h"

#include "GameAnimationType.h"

#include "BehaviorTreeManager.h"
#include "GameCharacter.h"

#include "GameStateMachine.h"

#include <memory>

enum ENEMY_ID {
	ENEMY_001 = 2 ,
	ENEMY_002
};


class character_attribute {
	int hp;
public:
	int GetHp() { return hp; }
};
 
enum class ENEMY_DIR
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Enemy :public GameCharacter{
	 
	int _enemy_id;
	BehaviorTreeManager *btM;
	bool _behavior_tree_state = false;
	std::shared_ptr<BNode> currentBehavior;
	 
	character_attribute attr;

	GameCharacter *_play[2];

	int _play_number;
	int _select_play;

	int _timer = 0;
 

	ENEMY_DIR _dir;

public:
	void setDir(ENEMY_DIR dir)
	{
		_dir = dir;
	}

	ENEMY_DIR getDir() { return _dir; }

	static Enemy* create(CharacterResources *res) {

		Enemy *sprite = new (std::nothrow) Enemy();

		if (sprite&& res != nullptr &&sprite->addCharRes(res)) {

			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	static Enemy* create(CharacterResources *res, const std::string &textureName) {
		Enemy *sprite = new (std::nothrow) Enemy();

		if (sprite&&sprite->addCharRes(res) && sprite->initWithTexture(textureName)) {

			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	void setBtree(BehaviorTreeManager *btm)
	{
		btM = btm;
	}

	void setID(int enemy_id)
	{
		_enemy_id = enemy_id;
	}
	int getID()
	{
		return _enemy_id;
	}
	void set_play(GameCharacter *play) {
		_play[_play_number++] = play;
	}
	void set_cur_play(int select_play) {
		_select_play = select_play;
	}

	Enemy_State getState() {
		return currentBehavior ? currentBehavior->state : Enemy_None;
	}
	std::shared_ptr<BNode> getBehavior() {
		return currentBehavior;
	}


	int getHp() { return attr.GetHp(); }
	

	int get_play()const  {
		return _play_number ;
	}
	
	GameCharacter* getPlayPtr(int sel) { 
		assert(_play[sel] != nullptr);
		return _play[sel];
	}
	GameCharacter* getPlayPtr() {
		assert(_play[_select_play] != nullptr);
		return _play[_select_play];
	}

	void setWaitTimer(int timer) {
		_timer = timer;
	}
	int getWaitTimer() {
		return _timer;
	}
	void updateWaitTimer() {
		if (_timer > 0)
		{ 
			_timer--;
		}
	}
 

	void setBehaviorEnable() {
		 _behavior_tree_state = true;
	}
	void setBehaviorDisable() {
		_behavior_tree_state = false;
	}
	void fetchEnemyBehaviorIfActive() {//自动检测状态是否符合
		if (_behavior_tree_state)
		{
			btM->getBehavior(_enemy_id, this);
		}
	}
	
	void update(float dt)
	{
		fetchEnemyBehaviorIfActive();
		stateMachine->update(this, dt);
	
		moveX(dt);
		moveY(dt);
		promoteFrames();
	}
	

	void  moveY(float dt) {

		if (!_gravityEnable)
		{
			_m_velocity = _m_accelaration.y = 0;
			return;
		}
		//----------------------- Y碰撞-------------------------
		auto y_vec = c_res->get_collision_atlas(_curFrameId);
		if (!y_vec.empty()) {
			auto curBox = y_vec;

			float tmpTmxY = _curAtTmxPos.y; // 保存Y

			_m_velocity += (_m_accelaration.y + GRAVITY) * dt ;


			// 边界检查，确保速度在最大值和最小值之间
			if (_m_velocity > MAX_VERTICAL_VELOCITY) {
				_m_velocity = MAX_VERTICAL_VELOCITY;
			}
			else if (_m_velocity < MIN_VERTICAL_VELOCITY) {
				_m_velocity = MIN_VERTICAL_VELOCITY;
			}

			// 垂直位移更新，乘以系数 2 加快速度
			_curAtTmxPos.y += _m_velocity;
			// 更新临时对象
			std::vector<Rect> tmpVec = curBox;
			for (auto &i : tmpVec) {
				i.origin += _curAtTmxPos;
			}

			auto instance = Collision::getInstance();
			auto ret = instance->checkMapCollision(_tmxBox, tmpVec, (int)CollisionSide::LEFT | (int)CollisionSide::RIGHT);
			bool topOrBottomCollision = (ret.side == CollisionSide::TOP || ret.side == CollisionSide::BOTTOM);


			// 底部突然为空的检查逻辑
			if (/*(_curStateId == ANIMATION_IDLE || _curStateId == STATE_RUN)*/

				(getCurrentState() == STATE_IDLE || getCurrentState() == STATE_RUN)
				&& !topOrBottomCollision) {

				_mJumpTime = 0;
				_m_velocity = 0; // 开始下落
				_mIsJumping = true;

				setCurrentMachine(STATE_JUMP); 
			}


			if (topOrBottomCollision/* &&ret.boxNumber != RAILING*/
				) {

				lastCollisionResult = ret;//保存这个
				 

				_curAtTmxPos.y = tmpTmxY;

				if (ret.side == CollisionSide::BOTTOM)
				{
					//if (abs(_m_velocity) < GRAVITY_MIN_COLLISION_VELOCITY)
					{
						if (_mIsJumping
							)
						{ 
							setCurrentMachine(STATE_IDLE);

							_curAtTmxPos.y = lastCollisionResult.rect.origin.y - _curSize.height;

							_mIsJumping = false;

							_m_velocity = 0; // 碰撞到底部时，重置垂直速度
							_m_accelaration.y = 0; // 重置垂直加速度
							_mJumpTime = 0;
						}
		 
						//_m_velocity.x = 0;
					}
					
					//setCurrentMachine(STATE_IDLE);
				}
				//顶部不能是栏杆
				else if (ret.side == CollisionSide::TOP) {

					_curAtTmxPos.y = ret.rect.origin.y + ret.rect.size.height;
					_m_velocity = 0; // 撞到顶部后垂直速度置零
					_m_accelaration.y = 0; // 重置垂直加速度

				}
			}

		}

	}
};

 
#endif
