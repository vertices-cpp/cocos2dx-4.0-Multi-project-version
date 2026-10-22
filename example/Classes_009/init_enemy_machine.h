#ifndef _INIT_ENEMY_H_
#define _INIT_ENEMY_H_

#include "Enemy.h"

void init_enemy_machine() {

	auto sTable = StateTable::getInstance();

	auto enemy001_machine = new GameStateMachine;
	sTable->add_state(ENEMY_001, enemy001_machine);


	//获取方向
	auto setDIr = [](Enemy *enemy)
	{
		auto play = enemy->getPlayPtr();
		Vec2 playTmxPos = play->getAtTmxPos();
		Vec2 enemy_pos = enemy->getAtTmxPos();


		float diffPosX = (enemy_pos.x + enemy->getCurSize().width / 2) - (playTmxPos.x + play->getCurSize().width / 2);

		ENEMY_DIR dir = ENEMY_DIR::NONE;
		if (diffPosX > 0)
		{
			
			enemy->setFlippedX(false);
			dir = ENEMY_DIR::LEFT;

		}
		else
		{
		
			enemy->setFlippedX(true);
			dir = ENEMY_DIR::RIGHT;
		}

		enemy->setDir(dir);
	};
	//设置速度
	auto setAcceleration = [](Enemy *enemy)
	{
		if (enemy->getDir()== ENEMY_DIR::LEFT)
		{
			enemy->setAcceleration(Vec2(-enemy->getMoveVelX(), 0));
		}
		else
			enemy->setAcceleration(Vec2(enemy->getMoveVelX(), 0));
	};

	 
	auto  enemy_StandingState_onEnter = [](GameCharacter* character) {
	 
		character->setCurAnimationId(ANIMATION_IDLE);
		character->setAcceleration(Vec2::ZERO);
		auto enemy = dynamic_cast<Enemy*>(character);
		if (enemy)
		{
			enemy->setWaitTimer(5); //则等待20 
			enemy->setBehaviorDisable();
		}
	};
	auto  enemy_StandingState_onExit = [](GameCharacter* character) {

		auto enemy = dynamic_cast<Enemy*>(character);
		if (enemy)
			enemy->setWaitTimer(0); //则等待20
	};
	auto  enemy_StandingUpdate = [setDIr](GameCharacter* character,float) {

		auto enemy = dynamic_cast<Enemy*>(character);

		setDIr(enemy);

		if (enemy)
			enemy->updateWaitTimer(); //则等待20


		if (!enemy->getWaitTimer())
		{
			enemy->setBehaviorEnable();
		}
	};

	enemy001_machine->registerState(STATE_IDLE, enemy_StandingState_onEnter, enemy_StandingState_onExit, enemy_StandingUpdate);


		//--------------------------跑步-----------------------------------------

	// 添加状态转换规则 
 
	auto  enemy_Running_onEnter = [setDIr, setAcceleration](GameCharacter* character) {
		 
		auto enemy = dynamic_cast<Enemy*>(character);

		enemy->setBehaviorDisable();

		setDIr(enemy);
		setAcceleration(enemy);

  		character->setCurAnimationId(ANIMATION_RUN);

		enemy->setWaitTimer(80); //则等待10 

	};

	auto   enemy_Running_onExit = [](GameCharacter* character) {
		//	character->releasePreParticle();
			// 重置水平速度和加速度，使角色停止移动
		character->setAcceleration(Vec2::ZERO);
		auto enemy = dynamic_cast<Enemy*>(character);
		if (enemy->getCurrentState() != STATE_JUMP)
		enemy->setBehaviorEnable();
	};
	auto  enemy_Running_Update= [](GameCharacter * character,float dt)
	{

// 		character->moveX(dt);
// 		character->moveY(dt);
		auto enemy = dynamic_cast<Enemy*>(character);

		auto dir = enemy->getDir();
		 

		if (enemy)
			enemy->updateWaitTimer(); //则等待20
		 
		auto play = enemy->getPlayPtr();

		if ((dir == ENEMY_DIR::LEFT &&enemy->getAtTmxPos().x <= play->getAtTmxPos().x+ play->getCurSize().width/2)||
			(dir == ENEMY_DIR::RIGHT &&enemy->getAtTmxPos().x >= play->getAtTmxPos().x- play->getCurSize().width / 2) || !enemy->getWaitTimer())
		{
			enemy->setCurrentMachine(STATE_IDLE);
		}
	};
	enemy001_machine->registerState(STATE_RUN, enemy_Running_onEnter, enemy_Running_onExit, enemy_Running_Update);

	//--------------------------跳跃-----------------------------------------
	


	auto enemy_JumpingState_onEnte = [setDIr, setAcceleration](GameCharacter* character) {
		 
		character->setCurAnimationId(ANIMATION_JUMP);
		character->setIsJumping(true);
		 
		character->setJumpTime(STATE_JUMP_TIME);
		//获取方向
		auto enemy = dynamic_cast<Enemy*>(character);
		setDIr(enemy);
		setAcceleration(enemy);
		int moveX = 0;

		auto dir = enemy->getDir();
		if (dir == ENEMY_DIR::LEFT)
			moveX = -80;
		else
			moveX = 80;

		character->setAcceleration(Vec2(moveX, -character->getMoveVelY() * 2.3f));


	};

	auto enemy001_JumpingState_onExit = [](GameCharacter* character) {
		character->setIsJumping(false);

		character->setJumpTime(0);
		character->setAcceleration(Vec2::ZERO);


	};
	auto enemy001_JumpingState_update = [](GameCharacter* character, float dt) {
		  
		if (character->getJumpTime() > 0) {
			character->setJumpTime(character->getJumpTime() - dt);
		}
		else {
			character->setAcceleration(cocos2d::Vec2(character->getAcceleration().x, 0));
		}
	};
	

	enemy001_machine->registerState(STATE_JUMP,
		enemy_JumpingState_onEnte, enemy001_JumpingState_onExit, enemy001_JumpingState_update);
}
void enemy1_init(Enemy *enemy) {

}
void enemy2_init(Enemy *enemy) {

}

void enemy3_init(Enemy *enemy) {

}

void enemy4_init(Enemy *enemy) {

}

void enemy5_init(Enemy *enemy) {

}

void enemy6_init(Enemy *enemy) {

}

#endif
