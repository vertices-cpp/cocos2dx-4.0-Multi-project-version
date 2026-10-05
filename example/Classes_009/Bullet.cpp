#include "Bullet.h"


CharacterStateMachine* CharacterStateMachine::create() {
	CharacterStateMachine* stateMachine = new (std::nothrow) CharacterStateMachine();
	if (stateMachine && stateMachine->init()) {
		stateMachine->autorelease();
		return stateMachine;
	}
	CC_SAFE_DELETE(stateMachine);
	return nullptr;
}

bool CharacterStateMachine::init() {
	if (!Node::init()) {
		return false;
	}
	currentState = State::IDLE;
	return true;
}

void CharacterStateMachine::changeState(State newState) {
	currentState = newState;
	if (currentState == State::SHOOTING) {
		shootBullet();
	}
}

void CharacterStateMachine::shootBullet() {
	auto bullet = Bullet::create();
	bullet->setPosition(this->getPosition());
	this->getParent()->addChild(bullet);
	bullet->shoot(cocos2d::Vec2(1, 0));
}


Bullet* Bullet::create() {
	Bullet* bullet = new (std::nothrow) Bullet();
	if (bullet && bullet->init()) {
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}

bool Bullet::init() {
	if (!Sprite::initWithFile("bullet.png")) {
		return false;
	}
	return true;
}

void Bullet::shoot(const cocos2d::Vec2& direction) {
	float speed = 500.0f;
	auto moveAction = cocos2d::MoveBy::create(1.0f, direction * speed);
	this->runAction(moveAction);
}