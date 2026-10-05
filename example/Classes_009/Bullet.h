#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
#include "cocos2d.h"
USING_NS_CC;


class CharacterStateMachine : public cocos2d::Node {
public:
	enum class State {
		IDLE,
		SHOOTING
	};

	static CharacterStateMachine* create();
	virtual bool init();
	void changeState(State newState);
	void shootBullet();

private:
	State currentState;

};
class Bullet : public cocos2d::Sprite {
	// 待转换的左上角的位置
	Vec2 _curPos = Vec2::ZERO;
	// 大小
	Size _curSize = Size::ZERO;
	// 速度和加速度
	Vec2 _m_velocity = Vec2::ZERO, _m_accelaration = Vec2::ZERO;
	// 地图位置
	Vec2 _curAtTmxPos = Vec2::ZERO;
public:
	static Bullet* create();
	virtual bool init();
	void shoot(const cocos2d::Vec2& direction);
	

	

	// 设置方法
	void setCurPos(const Vec2& pos) { _curPos = pos; }
	void setCurSize(const Size& size) { _curSize = size; }
	void setVelocity(const Vec2& velocity) { _m_velocity = velocity; }
	void setAcceleration(const Vec2& acceleration) { _m_accelaration = acceleration; }
	void setCurAtTmxPos(const Vec2& pos) {
		_curAtTmxPos = pos; 
	}
	// 获取方法
	const Vec2& getCurPos() const { return _curPos; }
	const Size& getCurSize() const { return _curSize; }
	const Vec2& getVelocity() const { return _m_velocity; }
	const Vec2& getAcceleration() const { return _m_accelaration; }
	const Vec2& getCurAtTmxPos() const { return _curAtTmxPos; }

};

#endif // _BULLET_H_    