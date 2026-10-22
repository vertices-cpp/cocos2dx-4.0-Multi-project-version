#include "GameCharacterBase.h" 

USING_NS_CC;

#include <iostream>

GameCharacterBase::GameCharacterBase()
	/*: _curStateId(0) */ {
	//currentState = nullptr;
}

GameCharacterBase::~GameCharacterBase() {
 
}



void GameCharacterBase::setKeyVal(EventKeyboard::KeyCode keyValue)
{
	OP_KEY keyCode = KeyTable[keyValue];
	switch (keyCode) {
	case OP_UP:
		keyUpPressed = true;
		curKeyUpPressed = true;
		break;
	case OP_DOWN:
		keyDownPressed = true;
		curKeyDownPressed = true;
		break;
	case OP_LEFT:
		keyLeftPressed = true;
		curKeyLeftPressed = true;
		break;
	case OP_RIGHT:
		keyRightPressed = true;
		curKeyRightPressed = true;
		break;
	case OP_JUMP:
		keyJumpPressed = true;
		curKeyJumpPressed = true;
		break;
	case OP_ATTACK:
		keyAttackPressed = true;
		curKeyAttackPressed = true;
		break;
	case OP_SELECT:
		keySelectPressed = true;
		curKeySelectPressed = true;
		break;
	default:
		break;
	}
}

void GameCharacterBase::setUnKeyVal(EventKeyboard::KeyCode keyValue)
{
	OP_KEY keyCode = KeyTable[keyValue];
	switch (keyCode) {
	case OP_UP:
		keyUpPressed = false;

		break;
	case OP_DOWN:
		keyDownPressed = false;
		break;
	case OP_LEFT:
		keyLeftPressed = false;
		break;
	case OP_RIGHT:
		keyRightPressed = false;
		break;
	case OP_JUMP:
		keyJumpPressed = false;
		break;
	case OP_ATTACK:
		keyAttackPressed = false;
		break;
	case OP_SELECT:
		keySelectPressed = false; 
		break;
	default:
		break;
	}
}

// 新增函数：将 key 系列变量的值赋给 curKey 系列变量
void GameCharacterBase::restoreKeyStates() {
	curKeyUpPressed = keyUpPressed;
	curKeyDownPressed = keyDownPressed;
	curKeyLeftPressed = keyLeftPressed;
	curKeyRightPressed = keyRightPressed;
	curKeyJumpPressed = keyJumpPressed;
	curKeyAttackPressed = keyAttackPressed;
	curKeySelectPressed = keySelectPressed;
}
 
void GameCharacterBase::GetKeyEvent()
{ 
	//std::cout << "开始:";
	//仅使用带cur的KEY来检测当前，防止cocos2dx本身轮询完松开键。而丢失键值导致的没处理
	prevKeyPressed = keyPressed;
	keyPressed = 0;

	// 根据当前按键状态更新 keyPressed
	keyPressed |= curKeyUpPressed ? OP_UP : 0;
	keyPressed |= curKeyDownPressed ? OP_DOWN : 0;
	keyPressed |= curKeyLeftPressed ? OP_LEFT : 0;
	keyPressed |= curKeyRightPressed ? OP_RIGHT : 0;
	keyPressed |= curKeyJumpPressed ? OP_JUMP : 0;
	keyPressed |= curKeyAttackPressed ? OP_ATTACK : 0;
	keyPressed |= curKeySelectPressed ? OP_SELECT : 0;

	singleKeyPressed = keyPressed & ~prevKeyPressed;

	if (singleKeyPressed & OP_UP)
	{
		std::cout << "单次 上键按下" << std::endl;
	}
	if (singleKeyPressed & OP_DOWN) {
		std::cout << "单次 下键按下" << std::endl;
	}
	if (singleKeyPressed & OP_LEFT) {
		std::cout << "单次 左键按下" << std::endl;
	}
	if (singleKeyPressed & OP_RIGHT) {
		std::cout << "单次 右键按下" << std::endl;
	}
	if (singleKeyPressed & OP_ATTACK) {
		std::cout << "单次 攻击按下" << std::endl;
	}
	if (singleKeyPressed & OP_JUMP) {
		std::cout << "单次 跳键按下" << std::endl;
	}
	if (singleKeyPressed & OP_SELECT) {
		std::cout << "单次 选择按下" << std::endl;
	} 
//	KeyReleaseLastFrame = ~keyPressed & prevKeyPressed;
//std::cout << std::hex<< singleKeyPressed <<" "<< keyPressed<< std::dec<< std :: endl;
}
void GameCharacterBase::update(float dt)
{
	GetKeyEvent();
	restoreKeyStates();
	if (keyPressed & OP_ATTACK)
	{
		std::cout << "已按下：" << pressed_delay << std::endl;
		incKeydelay();
	}
// 	else
// 	{
// 		 
// 		setKeydelayZero();
// 	}
}
 

// void GameCharacterBase::setCurStateId(int id) {
// 	_curStateId = id;
// }
// 
// int GameCharacterBase::getCurStateId() const {
// 	return _curStateId;
// }
 

int GameCharacterBase::getCurFrameId() const {
	return _curFrameId;
}

void GameCharacterBase::setCurTextureId(const int& curFrameId) {
	_curFrameId = curFrameId;
}

void GameCharacterBase::setAcceleration(const cocos2d::Vec2& accel) {
	_m_accelaration = accel;
}

cocos2d::Vec2 GameCharacterBase::getAcceleration() const {
	return _m_accelaration;
}

bool GameCharacterBase::getFlippedX()
{
	return _flippedX;
}
void GameCharacterBase::setFlippedX(bool flippedX)
{
	if (_flippedX != flippedX)
	{
		// 获取当前锚点
		Vec2 spfpoint = getAnchorPoint();

		// 计算翻转后的锚点 x 坐标
		// 当水平翻转时，锚点的 x 坐标需要相应调整
		// 例如，若当前锚点 x 为 0.33333，翻转后应为 0.66666
		Vec2 vec2_one = Vec2::ONE;
		spfpoint.x = vec2_one.x - spfpoint.x;

		// 设置新的锚点
		setAnchorPoint(spfpoint);

		// 更新翻转状态
		_flippedX = flippedX;

		// 执行水平翻转操作
		flipX();
	}
}

bool GameCharacterBase::isFlippedX() const {
	return _flippedX;
}

cocos2d::Vec2 GameCharacterBase::getPosition()   {
	return cocos2d::Sprite::getPosition();
}
 

void GameCharacterBase::addChild(cocos2d::Node* child) {
	cocos2d::Sprite::addChild(child);
}
 

void GameCharacterBase::setPos(Vec2 & tmxPos)
{
	auto _curPos = _curAtTmxPos + tmxPos;
	auto cur_pos = _director->convertToGL(_curPos); 
	Node::setPosition(cur_pos);
}

void GameCharacterBase::setOffsetVal(float x, float y)
{
	_offsetVel = { x,y };
	updatePoly();
}

void GameCharacterBase::setOffsetVal(const Vec2 & pos)
{
	setOffsetVal(pos.x, pos.y);
}




void  GameCharacterBase::setVertexCoords(const Rect& rect, V3F_C4B_T2F_Quad* outQuad)
{
	float relativeOffsetX = _unflippedOffsetPositionFromCenter.x;
	float relativeOffsetY = _unflippedOffsetPositionFromCenter.y;

	// issue #732
	if (_flippedX)
		relativeOffsetX = -relativeOffsetX;
	if (_flippedY)
		relativeOffsetY = -relativeOffsetY;

	_offsetPosition.x = relativeOffsetX + (_originalContentSize.width - _rect.size.width) / 2;
	_offsetPosition.y = relativeOffsetY + (_originalContentSize.height - _rect.size.height) / 2;


	_offsetPosition += _offsetVel;
	// FIXME: Stretching should be applied to the "offset" as well
	// but probably it should be calculated in the caller function. It will be tidier
	if (_renderMode == RenderMode::QUAD)
	{
		_offsetPosition.x *= _stretchFactor.x;
		_offsetPosition.y *= _stretchFactor.y;
	}

	// rendering using batch node
	if (_renderMode == RenderMode::QUAD_BATCHNODE)
	{
		// update dirty_, don't update recursiveDirty_
		setDirty(true);
	}
	else
	{
		// self rendering

		// Atlas: Vertex
		const float x1 = 0.0f + _offsetPosition.x + rect.origin.x;
		const float y1 = 0.0f + _offsetPosition.y + rect.origin.y;
		const float x2 = x1 + rect.size.width;
		const float y2 = y1 + rect.size.height;

		// Don't update Z.
		outQuad->bl.vertices.set(x1, y1, 0.0f);
		outQuad->br.vertices.set(x2, y1, 0.0f);
		outQuad->tl.vertices.set(x1, y2, 0.0f);
		outQuad->tr.vertices.set(x2, y2, 0.0f);
	}
}
