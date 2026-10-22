 

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{ 
	cocos2d::ParticleSystemQuad* particle;
	cocos2d::ParticleBatchNode *PbatchBode;
public:
    static cocos2d::Scene* createScene();
	int cnt = 0;
    virtual bool init();

	void update(float delta);

	void setCamera();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	Vec2   visibleSize;
	cocos2d::MotionStreak* _steak;
	float angle = 0;
	int angleOffset = 0;
	float _colorTime;       // 渐变计时器（0~2循环）
	bool _reversePhase;     // 是否反向渐变
	cocos2d::Color3B _goldLight; // 亮金色（#FFD700）
	cocos2d::Color3B _goldDark;  // 暗金色（#B8860B）
	cocos2d::TMXTiledMap *tmap;
	cocos2d::Vec2 spriteVel = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 tmxPos = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 spritePos = cocos2d::Vec2::ZERO;
	cocos2d::Sprite *sprite = nullptr;
};

#endif // __HELLOWORLD_SCENE_H__
