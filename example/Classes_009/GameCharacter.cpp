#include "GameCharacter.h"
#include "GameCore.h"

GameCharacter::GameCharacter():OBjectParticle(this) {
/*	_curStateId =*/
	init();
}
bool GameCharacter::init() {
	_animation_id = 0;
	_mIsJumping = _mJumpTime = 0;

	stateMachine = NULL;// new class GameStateMachine;

	_gravityEnable = false;
	return true;
}

GameCharacter::~GameCharacter() {

}

// int GameCharacter::getStateMachine() const {
// 	return stateMachine->getCurrentState();
// }
// int GameCharacter::getPreStateMachine()const {
// 	return stateMachine->getPreState();
// }
// void GameCharacter::setStateMachine(int stateId) {
// 	stateMachine->setStateMachine(stateId, this);
// }
void GameCharacter::setCurrentState(int state) {
	currentState_ = state;
}
void GameCharacter::setPreState(int state) {
	preState_ = state;
}

void GameCharacter::setCurrentMachine(int state)   {
	stateMachine->setStateMachine(state,this);
} 


int GameCharacter::getCurrentState() const {
	return currentState_;
}
int GameCharacter::getPreState()const {
	return preState_;
}




void GameCharacter::processEvent(int input) {
	// 假设 state 是当前游戏角色的状态指针
	//std::cout << "当前状态!"<<__current_state_ << std::endl;
	if (keyPressed)
	{
		stateMachine->handleInput(input, this);
		//std::cout << "1检测按键!" <<input<< std::endl;
	}
	stateMachine->handleEvent(this);
	//std::cout << "2检测事件!" << std::endl;

}

// void GameCharacter::registerState(int state, std::function<void(GameCharacter*)> onEnter, 
// 	std::function<void(GameCharacter*)> onExit,  std::function<void(GameCharacter*, float)> update ,
// 	std::function<void(GameCharacter*)> handleEvent )
// {
// 	stateMachine->registerState(state, onEnter, onExit,update, handleEvent);
// }



 

void GameCharacter::update(float dt) {
	GameCharacterBase::update(dt);
    if (singleKeyPressed&OP_SELECT)
    {
		_item++;
		
		if (_item >=3)
		{
			_item = 0;
			
		}
	//	cout << "道具" << _item << endl;
    }
	processEvent(keyPressed);
	moveX(dt);//仅加X盒子碰撞
	moveY(dt);//仅加Y盒子碰撞(如果无重力会清0速度等)
	//predictNextFrameYCollision(dt);//预测Y碰撞

 
	stateMachine->update(this, dt);
 

	promoteFrames();
	
}
 



 
// 实现计算偏移量的函数
float GameCharacter::calculateOffsetY(const int& curFrameId, const int& nextFrameId) {
	  
	auto curBox = c_res->get_collision_atlas(curFrameId);
	  
	auto nextBox = c_res->get_collision_atlas(nextFrameId);

	if (!curBox.empty() && !nextBox.empty()) {
		float nextBoxHeight = nextBox[0].size.height;
		float curBoxHeight = curBox[0].size.height;

		return curBoxHeight - nextBoxHeight;
	}
	return 0.0f;
}

void GameCharacter::moveX(float dt) {
	// tiled对象属性那要加collision
	auto x_vec = c_res->get_collision_atlas(_curFrameId);
	if (!x_vec.empty()) {
		auto curBox = x_vec;

		//----------------- X碰撞------------------------

		float tmpTmxX = _curAtTmxPos.x;
		_curAtTmxPos.x += _m_accelaration.x * dt;

		// 获取绑定在纹理里的碰撞框大小
		std::vector<Rect> tmpVec = curBox;
		for (auto &i : tmpVec) { //tmpVec只记录碰撞盒大小，位置都是0所以得加上当前所在位置才能正确碰撞
			i.origin += _curAtTmxPos;
		}

		auto instance = Collision::getInstance();
		auto ret = instance->checkMapCollision(_tmxBox, tmpVec, (int)CollisionSide::TOP | (int)CollisionSide::BOTTOM);

		bool leftOrRightCollision = (ret.side == CollisionSide::LEFT || ret.side == CollisionSide::RIGHT);

		if (leftOrRightCollision  ) // 表示不是墙是栏杆
		{
			switch (ret.boxNumber)
			{
			case WALL:
				_curAtTmxPos.x = tmpTmxX;
				break;
			case OPEN_ENEMY_1:
				//GameCore::getInstance()->setEnemy();
				break;
			}
		}

	}
}

bool isRailingCollisionValid(const Collision::CollisionResult& ret, float tmpTmxY) {
	auto destMapRect = ret.rect;
	if (ret.boxNumber == RAILING && ret.side == CollisionSide::BOTTOM) {
		return tmpTmxY + ret.characterPosition.size.height <= destMapRect.origin.y;
	}
	else if (ret.boxNumber == RAILING && ret.side == CollisionSide::TOP) {
		return tmpTmxY >= destMapRect.origin.y + destMapRect.size.height;
	}
	return true;
}




void GameCharacter::moveY(float dt) {

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

		_m_velocity += (_m_accelaration.y + GRAVITY) * dt * 3;


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
		std::vector<Rect> tmpVec =  curBox;
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

			setCurrentMachine(STATE_JUMP);
			_mIsJumping = true;
			_mJumpTime = 0;
			_m_velocity = 0; // 开始下落
		}


		if (topOrBottomCollision/* &&ret.boxNumber != RAILING*/
			) {

			lastCollisionResult = ret;//保存这个

			//--------------------如果不是在栏杆矩形边界则不处理碰撞
// 			if (!isRailingCollisionValid(ret, tmpTmxY)) {
// 				return;
// 			}

			if (!isRailingCollisionValid(lastCollisionResult, tmpTmxY) && lastCollisionResult.boxNumber == RAILING) {

				if (lastCollisionResult.side == CollisionSide::BOTTOM)
				{
					return;
				}
				else if (lastCollisionResult.side == CollisionSide::TOP)
				{
					//获取它底部的位置
					auto topCollision = lastCollisionResult.rect.origin.y + lastCollisionResult.rect.size.height;

					if (topCollision - lastCollisionResult.characterPosition.origin.y < MAX_VERTICAL_VELOCITY)
					{
						_curAtTmxPos.y = topCollision;
						setCurrentMachine(STATE_HANGING);
					}

					//	railingRect = predictedRet.rect;
				}
				return;
			}

			_curAtTmxPos.y = tmpTmxY;

			//顶部不能是栏杆
			if (ret.side == CollisionSide::BOTTOM) {



				// 检测小等于设定的重力最小值时并且是跳时才进入
				//if (abs(_m_velocity) < GRAVITY_MIN_COLLISION_VELOCITY)
				{
					if (_mIsJumping
						) {
						if ((keyLeftPressed || keyRightPressed)) {
							setCurrentMachine(STATE_RUN);
							//获取它顶部的位置
							_curAtTmxPos.y = lastCollisionResult.rect.origin.y - _curSize.height;
							if (singleKeyPressed == OP_JUMP)
							{
								std::cout << "已跳" << std::endl;
								setCurrentMachine(STATE_JUMP);
								return;
							}
						}
						else if (keyDownPressed) { // 如果按着下键，进入第一种蹲下

							setCurrentMachine(STATE_SQUAT_1);
						}
						else {
							int curFrameID = _curFrameId;
							// 转换到第二个蹲下状态
							setCurrentMachine(STATE_SQUAT_2);
							int newCurFrameId = getCurFrameId();
							float offset_Y = calculateOffsetY(curFrameID, newCurFrameId);
							_curAtTmxPos.y = tmpTmxY + offset_Y;
						}
						_mIsJumping = false;

						_m_velocity = 0; // 碰撞到底部时，重置垂直速度
						_m_accelaration.y = 0; // 重置垂直加速度
						_mJumpTime = 0;
					}
					
				}
				
			}
			else if (ret.side == CollisionSide::TOP) {

				_curAtTmxPos.y = ret.rect.origin.y + ret.rect.size.height;
				_m_velocity = 0; // 撞到顶部后垂直速度置零
				_m_accelaration.y = 0; // 重置垂直加速度

				if (ret.boxNumber == RAILING)
				{
					//获取它底部的位置

					setCurrentMachine(STATE_HANGING);
				}
			}
		}

	}

}
// void GameCharacter::predictNextFrameYCollision(float dt) {
// 
// 	if (!_gravityEnable)
// 	{
// 		return;
// 	}
// 	auto y_vec = c_res->get_collision_atlas(_curFrameId);
// 	if (!y_vec.empty()) {
// 		auto curBox = y_vec; 
// 		// ---------------------------预测下一帧的垂直速度-------------------------
// 		if (_mIsJumping && _curStateId == STATE_JUMP)
// 		{
// 			float predictedVelocityY = _m_velocity;
// 			if (_gravityEnable)
// 			{
// 				predictedVelocityY += (_m_accelaration.y + GRAVITY) * dt * 3;//下一帧应该是包含了这一帧，
// 			} 			// 边界检查，确保预测速度在最大值和最小值之间
// 			else
// 			{
// 				_m_velocity = _m_accelaration.y = 0;
// 				return;
// 			}
// 			if (predictedVelocityY > MAX_VERTICAL_VELOCITY) {
// 				predictedVelocityY = MAX_VERTICAL_VELOCITY;
// 			}
// 			else if (predictedVelocityY < MIN_VERTICAL_VELOCITY) {
// 				predictedVelocityY = MIN_VERTICAL_VELOCITY;
// 			} 			//预测下一帧的位置
// 			std::vector<Rect> predictedTmpVec =  curBox;
// 
// 			for (auto &i : predictedTmpVec) {
// 				i.origin += _curAtTmxPos; //加上当前帧的位置
// 				i.origin.y += predictedVelocityY;
// 			}
// 
// 			float tmpTmxY = _curAtTmxPos.y;
// 
// 			auto instance = Collision::getInstance();
// 
// 			auto predictedRet = instance->checkMapCollision(_tmxBox, predictedTmpVec, (int)CollisionSide::LEFT | (int)CollisionSide::RIGHT);
// 			bool predictedTopOrBottomCollision = (predictedRet.side == CollisionSide::TOP || predictedRet.side == CollisionSide::BOTTOM); // 
// 			if (predictedTopOrBottomCollision && abs(predictedVelocityY) < GRAVITY_MIN_COLLISION_VELOCITY + 7)
// 			{
// 				lastCollisionResult = predictedRet;
// 
// 				//--------------------如果不是在栏杆矩形边界则不处理碰撞,优先处理状态切换为挂杆
// 				if (!isRailingCollisionValid(predictedRet, tmpTmxY) && predictedRet.boxNumber == RAILING) {
// 
// 					if (predictedRet.side == CollisionSide::BOTTOM)
// 					{
// 						return;
// 					}
// 					else if (predictedRet.side == CollisionSide::TOP)
// 					{
// 						//获取它底部的位置
// 						auto topCollision = predictedRet.rect.origin.y + predictedRet.rect.size.height;
// 
// 						if (topCollision - predictedRet.characterPosition.origin.y < MAX_VERTICAL_VELOCITY)
// 						{
// 							_curAtTmxPos.y = topCollision;
// 							setStateMachine(STATE_HANGING);
// 						}
// 
// 						//	railingRect = predictedRet.rect;
// 					}
// 					return;
// 				}
// 				//顶部不能是栏杆
// 				if (predictedRet.side == CollisionSide::BOTTOM /*&& predictedRet.boxNumber != RAILING*/) {
// 					// 检测小等于设定的重力最小值时并且是跳时才进入
// 
// 					if (_mIsJumping
// 						) {
// 						if ((keyLeftPressed || keyRightPressed)) {
// 							setStateMachine(STATE_RUN);
// 						}
// 						else if (keyDownPressed) { // 如果按着下键，进入第一种蹲下
// 
// 							setStateMachine(STATE_SQUAT_1);
// 						}
// 						else {
// 							int curFrameID = _curFrameId;
// 							// 转换到第二个蹲下状态
// 							setStateMachine(STATE_SQUAT_2);
// 							int newCurFrameId = getCurFrameId();
// 							float offset_Y = calculateOffsetY(curFrameID, newCurFrameId);
// 							_curAtTmxPos.y += +offset_Y;
// 							 
// 						}
// 						_mIsJumping = false;
// 					}
// 					//_m_velocity.x = 0;
// 
// 					_m_velocity = 0; // 碰撞到底部时，重置垂直速度
// 					_m_accelaration.y = 0; // 重置垂直加速度
// 					_mJumpTime = 0;
// 				}
// 				else if (predictedRet.side == CollisionSide::TOP) {
// 					if (predictedRet.boxNumber == RAILING)
// 					{
// 						//获取它底部的位置
// 						_curAtTmxPos.y = predictedRet.rect.origin.y + predictedRet.rect.size.height;
// 						setStateMachine(STATE_HANGING);
// 						//railingRect = predictedRet.rect;
// 						_m_velocity = 0; // 撞到顶部后垂直速度置零
// 						_m_accelaration.y = 0; // 重置垂直加速度
// 						return;
// 					}
// 					
// 					//	_gravityEnable = false;
// 				}
// 			}
// 
// 		}//-------------------------- - 预测结束------------------------ -
// 
// 	}
// 
// 
// 
// } 

Texture2D * GameCharacter::getAnimationTexture(int animation_id,int curFrame)
{ 
	return c_res->readMapperTexture(animation_id,curFrame);
}

void GameCharacter::promoteFrames() {

// 	if (!AnimationIsRun()) { 
// 		auto loopId_Item = find_if(_loopID.begin(), _loopID.end(), [&](int animation_id) {
// 			return _animation_id == animation_id;
// 		});
// 		if (loopId_Item != _loopID.end()) {
// 			_animation_play.initialize(this, c_res->getAnimationPlayCfg(_animation_id));
// 		}
// 	}
	_curFrameId = _animation_play.getcurFrameTextueId();
	setCurTexture(_curFrameId);

	_animation_play.update();
}


void GameCharacter::setCurAnimationId(int animationId)
{	
	auto  curAniPlay = c_res->getAnimationPlayCfg(animationId);
	if (curAniPlay == nullptr)
		return;
	//然后获取正确的动画ID
	_animation_id = animationId;
 
	//自定义部分
	_animation_play.initialize(this, curAniPlay);
	_curFrameId = _animation_play.getcurFrameTextueId();
	// 设置运行状态
}

void GameCharacter::setCurTexture(const int &curFrameId) {
 
	_curFrameId = curFrameId;
	Texture2D * curTexture = c_res->readTexture(curFrameId);
 
	setTexture(curTexture);
	Rect rect; rect.origin = Vec2::ZERO, rect.size = curTexture->getContentSize();
	Sprite::setTextureRect(rect, 0, curTexture->getContentSize());
	Vec2 spfpoint =  c_res->get_anchor_point( _curFrameId);
	//添加锚点
	if (this->isFlippedX())
	{

		Vec2 vec2_one = Vec2::ONE;
		spfpoint.x = vec2_one.x - spfpoint.x;

	}
	setAnchorPoint(spfpoint);
}

bool GameCharacter::addCharRes(CharacterResources * res)
{
	assert(res != nullptr);
	c_res = res;
	character_id = c_res->getCharacterID();
	character_name = c_res->getCharacterName();

	return true;
}
int GameCharacter::getCharId() {
	return character_id;
}
 
