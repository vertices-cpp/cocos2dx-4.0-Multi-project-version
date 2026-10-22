#ifndef _GAME_CHARACTER_BASE_H_
#define _GAME_CHARACTER_BASE_H_
#include "cocos2d.h" 
#include "my_resources_manage.h"
//#include "GameStateMachine.h"

enum OP_KEY {
	OP_NON,
	OP_UP,
	OP_DOWN = 1 << 1,
	OP_LEFT = 1 << 2,
	OP_RIGHT = 1 << 3,
	OP_JUMP = 1 << 4,
	OP_ATTACK = 1 << 5,
	OP_THROW = 1 << 6,

	OP_SELECT =1<< 7,
	OP_SPECIAL = 1 << 8,
};

#define  SPECIAL_CONT 30


class GameCharacterBase : public cocos2d::Sprite {
	 
	std::unordered_map<  EventKeyboard::KeyCode, OP_KEY> KeyTable;
public: 
	GameCharacterBase();
	virtual ~GameCharacterBase();
	 
	bool  findKeyTable(EventKeyboard::KeyCode keyVal)
	{
		return  KeyTable.find(keyVal) != KeyTable.end();
	}
	void bindKey() {
	}
	// 2. 递归绑定函数（带模板类型约束）
	template <typename _KEY_TYPE, typename _KEY_CODE, typename... Args>
	typename std::enable_if<
		std::is_same_v<_KEY_CODE, cocos2d::EventKeyboard::KeyCode> &&
		std::is_enum_v<_KEY_TYPE>    // 确保第一个参数是枚举（如Operation）
		 // 确保第二个参数是KeyCode
	>::type
		bindKey( _KEY_CODE code, _KEY_TYPE key, Args... args) {
		KeyTable[code] = key;  // 绑定当前按键
		bindKey(args...);      // 递归处理剩余参数
	}
	void setKeyVal(EventKeyboard::KeyCode keyValue);
	void setUnKeyVal(EventKeyboard::KeyCode keyValue);
	void restoreKeyStates();
	void GetKeyEvent();

	virtual void update(float dt);
	 
	int getKeyPressed() const { return keyPressed; }
	int getPrevKeyPressed() const { return prevKeyPressed; }
	int getSingleKeyPressed() const { return singleKeyPressed; } 

	// 通用的设置和获取方法
 
// 	void setCurStateId(int id);
// 	int getCurStateId() const;

// 	int getNextStateId() const { return _nextStateId; }
// 	void setNextStateId(int id) { _nextStateId = id; }
  
	int getCurFrameId() const;
	void setCurTextureId(const int& curFrameId);
	void setAcceleration(const cocos2d::Vec2& accel);
	cocos2d::Vec2 getAcceleration() const;
	bool getFlippedX();
	void setFlippedX(bool flipped);
	bool isFlippedX() const;
	cocos2d::Vec2 getPosition()  ;

	bool getIsJumping() const {
		return _mIsJumping;
	}

	void setIsJumping(bool isJumping) {
		_mIsJumping = isJumping;
	}

	float getJumpTime() const {
		return _mJumpTime;
	}

	void setJumpTime(float jumpTime) {
		_mJumpTime = jumpTime;
	}
	float getMoveVelX() const {
		return _moveVelX;
	}

	float getMoveVelY() const {
		return _moveVelY;
	}
	
	Node *getParent() {
		return _parent;
	}
	//const cocos2d::Node* getParent() const;
	void addChild(cocos2d::Node* child);
	 
	
	//移动速度
	int setVelX(float velX) { return _moveVelX = velX; }
	int setVelY(float velY) { return _moveVelY = velY; }

	void setMoveVel(int velX, int velY) {
		_moveVelX = velX; _moveVelY = velY;
	}

	int getVelX() { return _moveVelX; }
	int getVelY() { return _moveVelY; }

	void  setCurSize(Size size) {   _curSize = size; }
	Size getCurSize() { 
		return _curSize; 
	}
  
	virtual void setPos(Vec2 & tmxPos);
	void setAtTmxPos(Vec2 &pos) {
		_curAtTmxPos = pos; 
	}//绝对地图屏幕坐标，以左上角为原点
	Vec2 getAtTmxPos() { return _curAtTmxPos; }
	
	Vec2 getOffsetVal() { return _offsetVel; }
	void setOffsetVal(float x, float y);
	void setOffsetVal(const Vec2 &pos);
	void setGravityEnable(bool enable) { _gravityEnable = enable; }
	bool getGravity() { return _gravityEnable; }

	void setKeydelayZero() { pressed_delay = 0; }
	void incKeydelay() { pressed_delay++; }
	int getKeydelay() { return pressed_delay; }
 
	bool isKeyDelayMet() {
		if (pressed_delay > SPECIAL_CONT)
		{ 
			return true;
		}
	 
		return false;
	}

	void  setContentSize(const Size& size)
	{
		if (_renderMode == RenderMode::QUAD_BATCHNODE || _renderMode == RenderMode::POLYGON)
			CCLOGWARN("Sprite::setContentSize() doesn't stretch the sprite when using QUAD_BATCHNODE or POLYGON render modes");

		Node::setContentSize(size);

		updateStretchFactor();
		updatePoly();
		setTextureRect(Rect(Vec2::ZERO,size));//设置纹理矩形
	}
 
protected:
// 	union {
// 		enum CHARACTER_STATE __character_state_1_;
// 		int _nextStateId;
// // 	};
// // 	union {
// // 		enum CHARACTER_STATE __character_state_2_;
// 		int _curStateId;
//	};
	int _curFrameId;
	 
	float _moveVelX = 0, _moveVelY = 0;
	float _m_velocity = 0;

	cocos2d::Vec2 _m_accelaration;

	Vec2 _offsetVel;

	Size _curSize = Size::ZERO;
	
	Vec2 _curAtTmxPos = Vec2::ZERO;//地图位置
	
	

	bool _gravityEnable; 
	bool _mIsJumping;
	float _mJumpTime;

	// 定义布尔变量来记录每个按键的状态
	bool keyUpPressed = false;
	bool keyDownPressed = false;
	bool keyLeftPressed = false;
	bool keyRightPressed = false;
	bool keyJumpPressed = false;
	bool keyAttackPressed = false;
	bool keySelectPressed = false;

	// 定义布尔变量来存档每个按键的当前状态
	bool curKeyUpPressed = false;
	bool curKeyDownPressed = false;
	bool curKeyLeftPressed = false;
	bool curKeyRightPressed = false;
	bool curKeyJumpPressed = false;
	bool curKeyAttackPressed = false;
	bool curKeySelectPressed = false;

	int keyPressed = 0;
	int prevKeyPressed = 0;

	int singleKeyPressed = 0;
	int pressed_delay = 0; 
private:
	 
	void  setVertexCoords(const Rect& rect, V3F_C4B_T2F_Quad* outQuad);
};

#endif // _GAME_CHARACTER_BASE_H_    
