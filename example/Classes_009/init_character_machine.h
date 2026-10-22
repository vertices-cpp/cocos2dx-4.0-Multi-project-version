#ifndef _INIT_CHARACTER_MACHINE_H_
#define _INIT_CHARACTER_MACHINE_H_

#include "GameCharacter.h"

static void init_character_machine() {
	//文件名character_init.inc 

	
	auto sTable = StateTable::getInstance();

	auto character_machine = new GameStateMachine;
	sTable->add_state(1,character_machine);
 

		//--------------------------站立-----------------------------------------

	auto StepAttackKey = [](GameCharacter* character) {

		return (character->getSingleKeyPressed() & OP_ATTACK);
			 
	}; 

	auto leftAndRight = [](GameCharacter* character)
	{
		return ((character->getKeyPressed() ^ (OP_LEFT | OP_RIGHT))&(OP_LEFT | OP_RIGHT));

	};
	auto jumpKey = [](GameCharacter* character) {
		std::cout << "跳---" <<character->getCurrentState()<<" "<< (bool)(character->getSingleKeyPressed() & OP_JUMP)<< endl;
		return character->getSingleKeyPressed() & OP_JUMP;

	};

	character_machine->addTransition(OP_LEFT | OP_RIGHT, STATE_IDLE, STATE_RUN, leftAndRight);

	//特殊技

//	character_machine->addTransition(OP_UP, STATE_IDLE, STATE_SPECIAL_SKILL, leftAndRight);


	character_machine->addTransition(OP_DOWN, STATE_IDLE, STATE_SQUAT_1, [](GameCharacter* c) {
		return true; // 这里可以添加更复杂的条件判断
	});

	character_machine->addTransition(OP_JUMP, STATE_IDLE, STATE_JUMP, jumpKey);

	character_machine->addTransition(OP_ATTACK, STATE_IDLE, STATE_ATTACK_1, [](GameCharacter* c) {
		if (c->getSingleKeyPressed()&OP_ATTACK)
		{
			return true;
		}
		return false; // 这里可以添加更复杂的条件判断
	});

	auto  StandingState_onEnter = [](GameCharacter* character) {
//		auto paricle_id = character->getSaveId();
//		if (!paricle_id)
//		{
//		 	character->addParticle(2);
//		    character->toSavId();//保存ID
//// 			character->addParticle(1, 100, 10, 100);
//// 			character->toSavId();
//		}
//		else {
//			//character->pausePartilce(character->getSaveId(), true);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              			character->pausePartilce(character->getSaveId(), true);
//		}


		//character->setCurStateId(STATE_IDLE);
		character->setCurAnimationId(ANIMATION_IDLE);
		character->setAcceleration(Vec2::ZERO);
	};
	auto  StandingState_onExit = [](GameCharacter* character) {
		//character->removePreParticle();
		//character->pausePartilce(character->getSaveId(),true);
	};
	auto  StandingState_onHandle = [](GameCharacter* character) {
		auto curKey = character->getKeyPressed();
		bool delayKey = character->isKeyDelayMet();
	//	std::cout << character->getKeydelay() << std::endl;
		//如果触发技能
		if (delayKey && !(curKey & OP_ATTACK) )
		{
			//character->setCurrentMachine(STATE_SPECIAL_SKILL);
			character->setCurrentMachine(STATE_SPECIAL_SKILL);
			character->setKeydelayZero();
			
// 
// 
// 			auto sprite = GameCharacter::createWithTexture(character->getAnimationTexture(character->getCurAnimationId(), 4));
// 			character->getParent()->addChild(sprite);
// 			sprite->setOffsetVal(Vec2(character->getOffsetVal().x, character->getOffsetVal().y));
// 			auto pos = character->getPosition();
// 			pos.y = -300;
// 			pos.x = -300;
// 			sprite->setPosition(pos);
// 			sprite->setScale(60);
// 
// 			auto runA = ScaleTo::create(1.2, 1);
// 			auto moveto = MoveTo::create(1.2, /*sprite->getPosition(),*/character->getPosition());
// 			// 定义一个删除节点的函数
// 			auto removeSprite = [sprite]() {
// 				if (sprite && sprite->getParent()) {
// 					sprite->removeFromParent();
// 				}
// 			};
// 
// 			// 创建 CallFunc 动作
// 			auto callFunc = CallFunc::create(removeSprite);
// 
// 			// 创建动作序列，先执行缩放动作，再执行删除节点的动作
// 			auto sequence = Sequence::create(Spawn::create(runA, moveto, nullptr), callFunc, nullptr);
// 
// 			// 运行动作序列
// 			sprite->runAction(sequence);

			return;
		}


	};

	character_machine->registerState(STATE_IDLE, StandingState_onEnter, StandingState_onExit, nullptr, StandingState_onHandle);


	//--------------------------跑步-----------------------------------------

	// 添加状态转换规则 

	character_machine->addTransition(OP_JUMP, STATE_RUN, STATE_JUMP, jumpKey);

	character_machine->addTransition(OP_ATTACK, STATE_RUN, STATE_ATTACK_1, [](GameCharacter* c) {
		if (c->getSingleKeyPressed() & OP_ATTACK)
		{
			return true;
		}
		return false;
	});



	auto  Running_onEnter = [](GameCharacter* character) {
		//character->addParticle(1,200,10, 80);

		if (character->getKeyPressed() & OP_LEFT) {
			character->setFlippedX(true);
			character->setAcceleration(Vec2(-character->getMoveVelX(), 0));
		}
		else if (character->getKeyPressed() &OP_RIGHT) {
			character->setFlippedX(false);
			character->setAcceleration(Vec2(character->getMoveVelX(), 0));
		}
		//character->setCurStateId(STATE_RUN);
		character->setCurAnimationId(ANIMATION_RUN);

	};

	auto   Running_onExit = [](GameCharacter* character) {
	//	character->releasePreParticle();
		// 重置水平速度和加速度，使角色停止移动
		character->setAcceleration(Vec2::ZERO);

	};
	auto  Running_handleEvent = [](GameCharacter * character)
	{
		int prevKey = character->getPrevKeyPressed();
		int curKey = character->getKeyPressed();


		bool delayKey = character->isKeyDelayMet();
		// 优先处理跳跃
		if ((curKey & OP_JUMP) && !(prevKey & OP_JUMP)) {
			character->setCurrentMachine(STATE_JUMP);
			return; // 直接跳出，不处理其他逻辑
		}
		if (delayKey && !(curKey & OP_ATTACK) )
		{
			//character->setCurrentMachine(STATE_SPECIAL_SKILL);

			character->setCurrentMachine(STATE_SPECIAL_SKILL);
			character->setKeydelayZero();
			return;
		}

		// 奔跑状态的更新逻辑
		if ((prevKey & OP_LEFT) && (!(curKey & OP_LEFT) || (curKey & OP_RIGHT))) {
			// 左方向键释放且右方向键未按下，转换到站立状态
			//character->setCurrentMachine(STATE_IDLE);
			std::cout << "站立调用1" << std::endl;
			character->setCurrentMachine(STATE_IDLE);
			return;
		}

		// 检查右方向键是否释放
		if ((prevKey & OP_RIGHT) && (!(curKey & OP_RIGHT) || (curKey & OP_LEFT))) {
			// 右方向键释放且左方向键未按下，转换到站立状态
		//	character->setCurrentMachine(STATE_IDLE);
			std::cout << "站立调用2" << std::endl;
			character->setCurrentMachine(STATE_IDLE);
			//	character->setCurAnimationId(ANIMATION_IDLE);
		}
	};
	character_machine->registerState(STATE_RUN, Running_onEnter, Running_onExit,nullptr, Running_handleEvent);

	//--------------------------蹲下状态1-----------------------------------------


	auto Localfilter = [](GameCharacter* c) {
		if (
			(
			c->AnimationIsRun() && (c->IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_1) ||
			c->IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_2) ||
			c->IsCurAnimationId(ANIMATION_SQUATTING_THROW_ITEM)))
			||
			c->IsCurAnimationId(ANIMATION_HANGING_ROLL_DOWN)) {
			return false;
		}
		return true;
	};


	// 添加状态转换规则
	character_machine->addTransition(OP_JUMP, STATE_SQUAT_1, STATE_HANGING, [](GameCharacter* c) {
		//下翻的条件
		if (c->lastCollisionIsSide(CollisionSide::BOTTOM, RAILING)) {
			auto curW = c->getCurBoxSize().width;
			auto curTmxPosX = c->getAtTmxPos().x;
			auto railingRect = c->getLastCollisionBox().rect;
			//必须在栏杆里 范围+-3
			if (curTmxPosX < railingRect.origin.x - 3 || curTmxPosX + curW > railingRect.origin.x + railingRect.size.width + 3) {
				return false;
			}
			//速度清0
			c->setAcceleration(Vec2::ZERO);
			//设置下翻动画
			c->setCurAnimationId(ANIMATION_HANGING_ROLL_DOWN);
			c->setGravityEnable(false);
			auto tmxPos = railingRect.origin;
			auto characterSizeDiffY = c->getCurSize().height - c->getCurBoxSize().height;
			auto characterTmxPos = c->getAtTmxPos();
			//翻滚时不改变X
			tmxPos.x = characterTmxPos.x;
			tmxPos.y += railingRect.size.height + characterSizeDiffY;
			//并且翻转下去需要一个栏杆的底部位置
			Vec2 stepAcceleration = c->calculateFrameStep(characterTmxPos, tmxPos);
			c->setStep(stepAcceleration);
			//return ;
		}
		return false; //只是为了调用。强制退出false
	},
		Localfilter);//添加过滤器


	character_machine->addTransition(OP_LEFT, STATE_SQUAT_1, STATE_SQUAT_1, [](GameCharacter* character) {
		character->setFlippedX(true);
		return false;
	},
		Localfilter);//添加过滤器
	character_machine->addTransition(OP_RIGHT, STATE_SQUAT_1, STATE_SQUAT_1, [](GameCharacter* character) {
		character->setFlippedX(false);
		return false;
	},
		Localfilter);//添加过滤器

	character_machine->addTransition(OP_ATTACK, STATE_SQUAT_1, STATE_SQUAT_1, [](GameCharacter* character) {
		//如果不是这个道具动画则进入处理
		if (!character->IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_1 + character->getItemType()))
		{
			switch (character->getItemType())
			{
			case 0:
				character->setCurAnimationId(ANIMATION_SQUATTING_ATTACK_1);
				break;
			case 1:
				character->setCurAnimationId(ANIMATION_SQUATTING_ATTACK_2);
				break;
			case 2:
				character->setCurAnimationId(ANIMATION_SQUATTING_THROW_ITEM);
				break;
			}

		}
		return false;
	},
		Localfilter,StepAttackKey);//添加过滤器,单次攻击


	auto Squatting_onEnter = [](GameCharacter* character) {

		character->setAcceleration(Vec2::ZERO);

		//character->setCurStateId(STATE_SQUAT_1);
		character->setCurAnimationId(ANIMATION_SQUAT_1);

		auto atTmxPos = character->getAtTmxPos();
		atTmxPos.y += 8;
		character->setAtTmxPos(atTmxPos);

	};

	auto Squatting_onExit = [](GameCharacter* character) {
		// 退出站立状态时的逻辑

		auto  atTmxPos = character->getAtTmxPos();
		atTmxPos.y -= 8;
		character->setAtTmxPos(atTmxPos);

	};
	auto Squatting_handleEvent = [](GameCharacter * character)
	{
		int curKey = character->getKeyPressed();
		//如果当前动画没完成，且是攻击动画时让它放完
		if (character->AnimationIsRun() && (
			character->IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_1) ||
			character->IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_2) ||
			character->IsCurAnimationId(ANIMATION_SQUATTING_THROW_ITEM)

			|| character->IsCurAnimationId(ANIMATION_HANGING_ROLL_DOWN))) {
			//如果是下翻动画
			if (character->IsCurAnimationId(ANIMATION_HANGING_ROLL_DOWN))
			{

				cout << " " << character->getAnimationPlay() << endl;
				auto tmxPos = character->getAtTmxPos();
				character->runTmxStep();
			}
			return;
		}
		if (character->IsCurAnimationId(ANIMATION_HANGING_ROLL_DOWN))
		{

			character->setCurrentMachine(STATE_HANGING);
			return;
		}

		//如果攻击结束则设置当前下蹲动画
		if (
			character->IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_1) ||
			character->IsCurAnimationId(ANIMATION_SQUATTING_ATTACK_2) ||
			character->IsCurAnimationId(ANIMATION_SQUATTING_THROW_ITEM))

			character->setCurAnimationId(ANIMATION_SQUAT_1);

		if (!(curKey & OP_DOWN))
		{
			character->setCurrentMachine(STATE_IDLE);
			character->setCurAnimationId(ANIMATION_IDLE);
		}

	};

	auto Squatting_Update = [](GameCharacter *character,float)
	{
		int curKey = character->getKeyPressed();
		bool delayKey = character->isKeyDelayMet();

		if (delayKey && !(curKey & OP_ATTACK))
		{
			character->setCurrentMachine(STATE_SPECIAL_SKILL);
			character->setKeydelayZero();
			return;
		}
	};
	character_machine->registerState(STATE_SQUAT_1, Squatting_onEnter, Squatting_onExit, Squatting_Update, Squatting_handleEvent);

	//--------------------------SquattingState2-----------------------------------------

	// 新增状态类实现
	auto  SquattingState2_onEnter = [](GameCharacter* character) {
		//character->setCurStateId(STATE_SQUAT_2); // 假设 STATE_SQUAT_2 是第二个蹲下状态的 ID
		character->setCurAnimationId(ANIMATION_SQUAT_2);
		character->setAcceleration(Vec2::ZERO);
	};

	auto SquattingState2_update = [](GameCharacter* character, float dt) {

		if (character->AnimationIsRun()) {
			return;
		}
		auto  curBox = character->getCurCollisionRes();
		// 转换到站立状态
		character->setCurrentMachine(STATE_IDLE);
		character->setCurAnimationId(ANIMATION_IDLE);
		// 得到当前的碰撞盒
		auto NextItem = character->getCurCollisionRes();
		if (!curBox.empty() && !NextItem.empty()) {
			float nextBoxHeight = NextItem[0].size.height;
			float offset_Y = curBox[0].size.height - nextBoxHeight;
			auto atTmxPos = character->getAtTmxPos();
			atTmxPos.y += round(offset_Y);
			character->setAtTmxPos(atTmxPos);
		}
	};
	character_machine->registerState(STATE_SQUAT_2, SquattingState2_onEnter, nullptr, SquattingState2_update);


	//------------------跳跃----------------

	 // 检查方向键
	auto leftAndRightfilter = [](GameCharacter *character) {
		auto curKey = character->getKeyPressed();
		if ((curKey&OP_LEFT) && (curKey & OP_RIGHT))
		{
			character->setAcceleration(Vec2(0, character->getAcceleration().y));
			return false;
		}
		return true;
	};

	character_machine->addTransition(OP_LEFT, STATE_JUMP, STATE_JUMP, [](GameCharacter *character) {
		character->setFlippedX(true);
		// 改变水平加速度或速度以改变转向速度
		character->setAcceleration(Vec2(-character->getMoveVelX(), character->getAcceleration().y));
		return false;
	}, leftAndRightfilter);

	character_machine->addTransition(OP_RIGHT, STATE_JUMP, STATE_JUMP, [](GameCharacter *character) {
		character->setFlippedX(false);
		// 改变水平加速度或速度以改变转向速度
		character->setAcceleration(Vec2(character->getMoveVelX(), character->getAcceleration().y));
		return false;
	}, leftAndRightfilter);

	auto attackFilter = [](GameCharacter *character) {
		if ((character->IsCurAnimationId(ANIMATION_JUMP_ATTACK_1)) ||
			(character->IsCurAnimationId(ANIMATION_JUMP_ATTACK_2)) ||
			(character->IsCurAnimationId(ANIMATION_JUMP_THROW_ITEM)
				) && character->AnimationIsRun())
			return false;
		else
			return true; 
	};
	character_machine->addTransition(OP_ATTACK, STATE_JUMP, STATE_JUMP, [](GameCharacter *character) {
		switch (character->getItemType())
		{
		case 0:
			character->setCurAnimationId(ANIMATION_JUMP_ATTACK_1);
			break;
		case 1:
			character->setCurAnimationId(ANIMATION_JUMP_ATTACK_2);
			break;
		case 2:
			character->setCurAnimationId(ANIMATION_JUMP_THROW_ITEM);
			break;
		}
		return false;
	}, attackFilter,StepAttackKey);


	auto JumpingState_onEnte = [](GameCharacter* character) {

// 		auto curState = character->getCurStateId();
// 		character->setCurStateId(STATE_JUMP);//不包含动画初始画了
		character->setCurAnimationId(ANIMATION_JUMP);
		character->setIsJumping(true);



		if (character->getPreState() == STATE_HANGING) //如果让跳跃的攻击继承这个，则不应该去处理时间，速度等
			return;
		character->setJumpTime(STATE_JUMP_TIME);
		character->setAcceleration(Vec2(character->getAcceleration().x, -character->getMoveVelY() * STATE_JUMP_FORCE));


	};

	auto JumpingState_onExit = [](GameCharacter* character) {
		character->setIsJumping(false);

		character->setJumpTime(0);
		character->setAcceleration(Vec2(character->getAcceleration().x, 0));


	};
	auto Jumping_State = [](GameCharacter * character)
	{
		int curKey = character->getKeyPressed();

		if (!(curKey & OP_LEFT) && !(curKey & OP_RIGHT)) {
			// 如果松开了键
			character->setAcceleration(Vec2(0, character->getAcceleration().y));
		}

		if ((character->IsCurAnimationId(ANIMATION_JUMP_ATTACK_1) ||
			character->IsCurAnimationId(ANIMATION_JUMP_ATTACK_2) ||
			character->IsCurAnimationId(ANIMATION_JUMP_THROW_ITEM)

			) && character->AnimationIsRun())
		{
			return;
		}
		if (!character->IsCurAnimationId(ANIMATION_JUMP))
			character->setCurAnimationId(ANIMATION_JUMP);



	};
	auto JumpingState_update = [](GameCharacter* character, float dt) {

		int curKey = character->getKeyPressed();
		bool delayKey = character->isKeyDelayMet();

		if (delayKey && !(curKey & OP_ATTACK))
		{
			character->setCurrentMachine(STATE_SPECIAL_SKILL);
			character->setKeydelayZero();
			return;
		}

		if (character->getJumpTime() > 0) {
			character->setJumpTime(character->getJumpTime() - dt);
		}
		else {
			character->setAcceleration(cocos2d::Vec2(character->getAcceleration().x, 0));
		}
	};

	character_machine->registerState(STATE_JUMP, JumpingState_onEnte, JumpingState_onExit, JumpingState_update, Jumping_State);

	//--------------------------站立攻击1-----------------------------------------

	// AttackingState1
	auto AttackingState1_onEnter = [](GameCharacter* character) {
		//character->setCurStateId(STATE_ATTACK_1);

		switch (character->getItemType())
		{
		case 0:
			character->setCurAnimationId(ANIMATION_ATTACK_1);
			break;
		case 1:
			character->setCurAnimationId(ANIMATION_ATTACK_2);
			break;
		case 2:
			character->setCurAnimationId(ANIMATION_THROW_ITEM);
			break;
		}
		
		// 可以在这里添加攻击动画初始化等逻辑
	};

	auto  AttackingState1_update = [](GameCharacter* character, float dt) {
		// 攻击状态的更新逻辑，比如判断攻击动画是否结束等
		int curKey = character->getKeyPressed();

		if (character->AnimationIsRun()) {
			return;
		}
		// 攻击动画结束，转换到站立状态
		character->setCurrentMachine(STATE_IDLE);
		//	character->setCurAnimationId(ANIMATION_IDLE);

	};
	character_machine->registerState(STATE_ATTACK_1, AttackingState1_onEnter, nullptr,  AttackingState1_update);


	//-------------------------- 扔道具状态类实现-----------------------------------------

	auto ThrowingItemState_onEnter = [](GameCharacter* character) {
		//character->setCurStateId(STATE_THROW_ITEM); // 假设 STATE_THROW_ITEM 是扔道具状态的 ID
		character->setCurAnimationId(ANIMATION_THROW_ITEM);
		character->setAcceleration(Vec2::ZERO);


		// 可以在这里添加扔道具动画初始化等逻辑
	// 	auto itemSprite = cocos2d::Sprite::create("item.png"); // 假设道具图片名为 item.png
	// 	cocos2d::Vec2 pos = character->getPosition();
	// 	if (character->isFlippedX()) {
	// 		pos.x -= 50;
	// 	}
	// 	else {
	// 		pos.x += 50;
	// 	}
	// 	itemSprite->setPosition(pos);
	// 	character->getParent()->addChild(itemSprite);
	// 
	// 	auto moveAction = cocos2d::MoveBy::create(1.0f, cocos2d::Vec2(100, 0));
	// 	itemSprite->runAction(moveAction);
		//AudioEngine::play2d("sound1.mp3");
		//auto sprite3 = Sprite::create("hyoga_pike/hyoga_pike-0.png");
		//character->getParent()->addChild(sprite3);


		//Vec2  pos = character->getPosition();
		//pos.y += 60;
		////	pos.y += 30;
		//if (character->isFlippedX())
		//{
		//	pos.x -= 80;
		//	sprite3->setFlippedX(true);
		//}
		//else
		//{
		//	pos.x += 80;
		//	sprite3->setFlippedX(false);
		//}
		//sprite3->setPosition(pos);
		//// 	sprite3->schedule([&](float dt) {
		//// 		 
		//// 	}, 0.1f, "MyScheduleKey");
		//sprite3->setTag(1);
		//sprite3->setScale(0.5);
		////sprite3->setColor(Color3B::MAGENTA);
		//const std::string fileName = "hyoga_pike/hyoga_pike-";
		////"ikki_shooter/ikki_shooter-";
		//my_game_animation* ani = my_game_animation::create();

		//for (int i = 0; i <= 36; ++i)
		////for (int i = 22; i >= 1; --i)
		//{
		//	ani->addFrame(fileName + to_string(i + 1) + ".png", 0.030);
		//}
		//Animate *a = Animate::create(ani);

		//Color3B begColor = Color3B::GREEN;
		//Color3B reverseTintTo = Color3B::YELLOW;
		//Color3B endColor = Color3B(100, 200, 255);


		//auto Tint1 = TintTo::create(0.1, begColor);
		//auto Tint2 = TintTo::create(0.1, reverseTintTo);
		//auto Tint3 = TintTo::create(0.1, endColor);
		//// 	auto isEnd = [&]() {
		//// 		auto node = character->getParent()->getChildByTag(1);
		//// 		if (node!=nullptr)
		//// 		{
		//// 			node->stopAllActions();
		//// 			character->removeChild(node,true);
		//// 		}
		//// 	};
		//sprite3->runAction(
		//	//RepeatForever::create(
		//		Spawn::create(
		//			a,

		//			Sequence::create(
		//				Tint1, Tint2, Tint3, nullptr
		//			)
		//			, nullptr)

		////	)
		//);

	};


	auto  ThrowingItemState_update = [](GameCharacter* character, float dt) {
		// 扔道具状态的更新逻辑，比如判断道具是否扔出等
		// 这里简单示例，当道具动画结束后转换到站立状态
		// 假设这里有一个判断动画是否结束的方法 isThrowAnimationEnded
	// // 攻击状态的更新逻辑，比如判断攻击动画是否结束等
		if (character->AnimationIsRun()) {
			return;
		}

		character->setCurrentMachine(STATE_IDLE);
		//	character->setCurAnimationId(ANIMATION_IDLE);

	};

	character_machine->registerState(STATE_THROW_ITEM, ThrowingItemState_onEnter, nullptr,   ThrowingItemState_update);

	//------------------------挂杠-------------------

	// 过滤左右键同时按下的情况
	auto leftRightFilter = [](GameCharacter* c) {
		int curKey = c->getKeyPressed();
		if ((curKey & OP_LEFT) && (curKey & OP_RIGHT))
		{
			c->setAcceleration(Vec2(0, 0));
			c->setCurAnimationId(ANIMATION_HANGING);
			return false;
		}
		return true;

	};


	// 添加挂杆状态的转换规则

	auto up_roll_filter = [](GameCharacter* c) {//如果已经在上翻中
		return !c->IsCurAnimationId(ANIMATION_HANGING_ROLL_UP);
	};
	// 过滤动画正在运行的情况（攻击或上翻动画）
	auto animationRunningFilter = [](GameCharacter* c) {
		return !(c->AnimationIsRun() && (c->IsCurAnimationId(ANIMATION_HANGING_ATTACK_1) ||
			c->IsCurAnimationId(ANIMATION_HANGING_ATTACK_2) ||
			c->IsCurAnimationId(ANIMATION_HANGING_THROW_ITEM) ||
			c->IsCurAnimationId(ANIMATION_HANGING_ROLL_UP)));
	};


	// 上翻操作的条件
	auto upRollCondition = [](GameCharacter* c) {
		auto curW = c->getCurBoxSize().width;
		auto curTmxPosX = c->getAtTmxPos().x;
		Rect railingRect = c->getLastCollisionBox().rect;
		return !(curTmxPosX < railingRect.origin.x - 3 || curTmxPosX + curW > railingRect.origin.x + railingRect.size.width + 3);
	};

	// 攻击操作规则
	character_machine->addTransition(OP_ATTACK, STATE_HANGING, STATE_HANGING, [](GameCharacter* character) {
		
		switch (character->getItemType())
		{
		case 0:
			character->setCurAnimationId(ANIMATION_HANGING_ATTACK_1);
			break;
		case 1:
			character->setCurAnimationId(ANIMATION_HANGING_ATTACK_2);
			break;
		case 2:
			character->setCurAnimationId(ANIMATION_HANGING_THROW_ITEM);
			break;
		}
		character->setAcceleration(Vec2(0, 0));
		return false;//仅执行函数,不切换
	},
		animationRunningFilter, up_roll_filter, StepAttackKey);




	// 上翻操作规则
	character_machine->addTransition(OP_UP, STATE_HANGING, STATE_HANGING, [](GameCharacter* c) {
		c->setAcceleration(Vec2::ZERO);
		c->setCurAnimationId(ANIMATION_HANGING_ROLL_UP);
		Vec2 tmxPos = c->getLastCollisionBox().rect.origin;
		auto characterTmxPos = c->getAtTmxPos();
		tmxPos.x = characterTmxPos.x;
		tmxPos.y -= c->getCurSize().height * 1.3;
		Vec2 stepAcceleration = c->calculateFrameStep(characterTmxPos, tmxPos);
		c->setStep(stepAcceleration);
		return false;//仅执行函数
	},
		upRollCondition, animationRunningFilter, up_roll_filter);

	// 向左移动规则
	character_machine->addTransition(OP_LEFT, STATE_HANGING, STATE_HANGING, [](GameCharacter* c) {
		c->setFlippedX(true);
		c->setAcceleration(Vec2(-c->getMoveVelX(), 0));
		if (!c->AnimationIsRun() || !c->IsCurAnimationId(ANIMATION_HANGING_MOVE))
			c->setCurAnimationId(ANIMATION_HANGING_MOVE);
		return false;//仅移动无需转换
	},
		leftRightFilter, animationRunningFilter, up_roll_filter);

	// 向右移动规则
	character_machine->addTransition(OP_RIGHT, STATE_HANGING, STATE_HANGING, [](GameCharacter* c) {
		c->setFlippedX(false);
		c->setAcceleration(Vec2(c->getMoveVelX(), 0));
		if (!c->AnimationIsRun() || !c->IsCurAnimationId(ANIMATION_HANGING_MOVE))
			c->setCurAnimationId(ANIMATION_HANGING_MOVE);
		return false;//仅移动
	},
		leftRightFilter, animationRunningFilter, up_roll_filter);

	// 跳跃操作规则
	character_machine->addTransition(OP_JUMP, STATE_HANGING, STATE_JUMP, [](GameCharacter* c) {
		if (!(c->getSingleKeyPressed() & OP_JUMP))
		{
			return false;
		}
		c->setIsJumping(true);
		c->setJumpTime(0);
		c->setAcceleration(Vec2(0, 0));
		return true;
	},
		animationRunningFilter, up_roll_filter);

	auto HangingState_onEnter = [](GameCharacter* character) {
		//character->setCurStateId(STATE_HANGING);
		character->setCurAnimationId(ANIMATION_HANGING);
		character->setGravityEnable(false); // 禁用重力
		character->setAcceleration(Vec2::ZERO);

	};

	auto HangingState_onExit = [](GameCharacter* character) {
		character->setGravityEnable(true); // 启用重力
	};
	auto HangingState_update = [](GameCharacter* character, float dt) {
		//是否超出杆范围
		auto curW = character->getCurBoxSize().width;
		auto curTmxPosX = character->getAtTmxPos().x;
		Rect railingRect = character->getLastCollisionBox().rect;
		auto range_x = curW;
		if (curTmxPosX  < railingRect.origin.x - range_x ||
			curTmxPosX + curW  >railingRect.origin.x + railingRect.size.width + range_x)
		{
			character->setGravityEnable(true);

			character->setCurrentMachine(STATE_JUMP);

		}
		int curKey = character->getKeyPressed();
		bool delayKey = character->isKeyDelayMet();

		if (delayKey && !(curKey & OP_ATTACK))
		{
			character->setCurrentMachine(STATE_SPECIAL_SKILL);
			character->setKeydelayZero();
			return;
		}


		// 挂杆状态的更新逻辑
	};



	//当挂杆时，按键则移动，屏蔽同时左右方向键
	auto HangingState_handleEvent = [](GameCharacter* character) {
		int curKey = character->getKeyPressed();

		//翻滚的优先级与攻击一样，
		if (character->AnimationIsRun() &&
			(character->IsCurAnimationId(ANIMATION_HANGING_ATTACK_1) ||
				character->IsCurAnimationId(ANIMATION_HANGING_ATTACK_2) ||
				character->IsCurAnimationId(ANIMATION_HANGING_THROW_ITEM) ||
				character->IsCurAnimationId(ANIMATION_HANGING_ROLL_UP)))
		{
			if (character->IsCurAnimationId(ANIMATION_HANGING_ROLL_UP))
			{
				character->runTmxStep();
			}
			return;
		}
		//如果执行到这里表示结束
		if (character->IsCurAnimationId(ANIMATION_HANGING_ROLL_UP))
		{
			character->setGravityEnable(true);
			character->setCurrentMachine(ANIMATION_IDLE);
			return;
		}
		if (!(curKey& OP_LEFT) && !(curKey&OP_RIGHT))
		{// 没有按下左右键，角色静止
			character->setAcceleration(Vec2(0, 0));
			character->setCurAnimationId(ANIMATION_HANGING);
		}



	};
	character_machine->registerState(STATE_HANGING, HangingState_onEnter, HangingState_onExit
		, HangingState_update, HangingState_handleEvent);


	//添加特殊技
	auto SpecialSkill_onEnter = [](GameCharacter* character) {
		//character->setCurStateId(STATE_SPECIAL_SKILL);
		character->setCurAnimationId(ANIMATION_SPECIAL_ATTACK);
		character->setGravityEnable(false); // 禁用重力
		character->setAcceleration(Vec2::ZERO);

	};
	auto cur_specialskll_is_done = [](GameCharacter* c) {//如果已经在上翻中
		return !c->IsCurAnimationId(ANIMATION_SPECIAL_ATTACK);
	};
	auto SpecialSkill_Handle = [](GameCharacter* character) {
		if (character->AnimationIsRun())
		{
			return;
		}
		character->setGravityEnable(true); // 启用重力
		character->setKeydelayZero();
		character->setCurrentMachine(STATE_IDLE);
		return;
	};

	character_machine->registerState(STATE_SPECIAL_SKILL, SpecialSkill_onEnter, nullptr, nullptr,SpecialSkill_Handle);
	 

}

#endif