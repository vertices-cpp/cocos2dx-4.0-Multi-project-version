/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

USING_NS_CC;


DrawNode *drawMother;
Action *action;
Sprite *body;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
static int  toStatus = 0;
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	scheduleUpdate();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label


//     else
//     {
//         // position the label on the center of the screen
//         label->setPosition(Vec2(origin.x + visibleSize.width/2,
//                                 origin.y + visibleSize.height - label->getContentSize().height));
// 
//         // add the label as a child to this layer
//         this->addChild(label, 1);
//     }

    // add "HelloWorld" splash screen"
//     auto sprite = Sprite::create("HelloWorld.png");
//     if (sprite == nullptr)
//     {
//         problemLoading("'HelloWorld.png'");
//     }
//     else
//     {
//         // position the sprite on the center of the screen
//         sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
// 
//         // add the sprite as a child to this layer
//         this->addChild(sprite, 0);
//     }
	//setRotation3D(Vec3(0, 315, 0));
 
	auto bg = Sprite::create("flower.jpg");
	addChild(bg);
	bg->setContentSize(visibleSize);
	bg->setPosition(visibleSize / 2);
	bg->setColor(Color3B(100, 100, 100));


	body = Sprite::create();
	body->setPosition(_director->getWinSize() / 2);
	addChild(body);

	body->setRotation3D(Vec3(0, 315, 0));
	 drawMother = cocos2d::DrawNode::create();
	 
	//drawMother->drawCubicBezier(Vec2(600, 0), Vec2(100, 100), Vec2(300, 100), Vec2(300, 400),100,Color4F::WHITE);
//	drawMother->setPosition(visibleSize /2);
	
	drawMother->drawCircle(Vec2(0, 0), 500, 0, 5000, false, Color4F(1,0.97,1,1));
	drawMother->setScale(0.001);
	body->addChild(drawMother);

//	drawMother->setPosition(Vec2(140, 140));
 	auto amplify = cocos2d::ScaleBy::create(1.5, 600);
//  	auto moveTo = cocos2d::MoveTo::create(1.5, Vec2(150 * -9, 150 * -9));
//	auto rotateTo = cocos2d::RotateTo::create(1.5,Vec3(60,60,60));
 	//auto spw = cocos2d::Spawn::create(amplify, moveTo,nullptr);
	auto f = [&]() {
		toStatus = 1;
	};
	auto finishAction = CallFunc::create(f);
	auto seq = cocos2d::Sequence::create(amplify, finishAction, nullptr);
	action =drawMother->runAction(seq);
 
// 	auto center = Vec2::ZERO;
// 
// 	auto a = 0;
// 	auto radius = 490;
// 	auto x = cos(a * (3.1415926 / 180)) * radius;
// 	auto y = sin(a * (3.1415926 / 180)) * radius;
// 	label->setPosition(Vec2(x,y));
// 
//  	float ale = atan2f(y,x);
// 	ale *= (180 / 3.1415926);
 //	label->setRotation(360- a -90);

	
    return true;
}
int  sumAngle=0;
static float angle = 0 ;
static  float cnt = 0;

ParticleSun *sp1, *sp2, *sp3;

void HelloWorld::update(float delta)
{

	

	if (toStatus == 1)
	{
		 
			if (sumAngle < 360)
			{
				auto label = Label::createWithTTF("cool", "fonts/magic dafont.ttf", 44);
				label->setColor(Color3B(255,240,255));

				auto size = label->getContentSize();

				auto center = Vec2::ZERO;

				auto a = 0;
				auto radius = 480;
				auto x = cos(sumAngle * (3.1415926 / 180)) * radius;
				auto y = sin(sumAngle * (3.1415926 / 180)) * radius;
				label->setPosition(Vec2(x, y));
				label->setRotation(360 - sumAngle - 90);

				drawMother->addChild(label);
				sumAngle += size.width / 8;
				cnt = 0;

				auto sp = ParticleSun::create();
				sp->setStartColor(Color4F::MAGENTA);// (38, 38, 38, 255));
				sp->setStartColor(Color4F(1, 0.1, 1,0.8));
				sp->setStartColor(Color4F(1, 0.2, 1, 0.2));
				label->addChild(sp);
			}
			else {
				toStatus = 2;
			}
			return;
	}
	else	 if (toStatus == 2)
   {
		toStatus = 3;

			 auto subLine1 = cocos2d::DrawNode::create();
			 auto subLine2 = cocos2d::DrawNode::create();
			 auto subLine3 = cocos2d::DrawNode::create();

			 auto radius = 350;
			 auto subVec1 = Vec2(cos(120 * (3.1415926 / 180)) * radius, sin(120 * (3.1415926 / 180)) * radius);
			 auto subVec2 = Vec2(cos(240 * (3.1415926 / 180)) * radius, sin(240 * (3.1415926 / 180)) * radius);
			 auto subVec3 = Vec2(cos(360 * (3.1415926 / 180)) * radius, sin(360 * (3.1415926 / 180)) * radius);

			 Color4F Color4F2 = { 1, 0.90, 1, 1 };
			 subLine1->drawLine(subVec1, subVec2, Color4F2);
			 subLine2->drawLine(subVec2, subVec3, Color4F2);
			 subLine3->drawLine(subVec3, subVec1, Color4F2);
			 subLine1->setTag(1);
			 subLine2->setTag(2);
			 subLine3->setTag(3);

			 auto c1 = DrawNode::create();
			 auto c2 = DrawNode::create();
			 auto c3 = DrawNode::create();
/*			 Color4B::MAGENTA(255, 0, 255, 255);*/

			 Color4F Color4F3 = { 1,0.91, 1, 1 };
			 c1->drawCircle(Vec2::ZERO, 60, 0, 500, false, Color4F3);
			 c2->drawCircle(Vec2::ZERO, 60, 0, 500, false, Color4F3);
			 c3->drawCircle(Vec2::ZERO, 60, 0, 500, false, Color4F3);

			 c1->setPosition(subVec1);
			 c2->setPosition(subVec2);
			 c3->setPosition(subVec3);

			

			 subLine1->addChild(c1);
			 subLine2->addChild(c2);
			 subLine3->addChild(c3);

			 /*auto*/ sp1 = ParticleSun::create();
			/* auto*/ sp2 = ParticleSun::create();
			/* auto */sp3 = ParticleSun::create();
			 
			 
			 sp1->setColor(Color3B(38, 38, 38));
			 sp2->setColor(Color3B(38, 38, 38));
			 sp3->setColor(Color3B(38, 38, 38));
			 sp1->setScale(3);
			 sp2->setScale(3);
			 sp3->setScale(3);
			 sp1->setPosition(subVec1);
			 sp2->setPosition(subVec2);
			 sp3->setPosition(subVec3);
			 subLine1->addChild(sp1);
			
			 subLine1->addChild(sp2);
			 
			 subLine1->addChild(sp3);

			 subLine1->setScale(0.01);
			 subLine2->setScale(0.01);
			 subLine3->setScale(0.01);

		     

			 auto amplify1 = cocos2d::ScaleBy::create(1.5, 100);
			 auto amplify2 = cocos2d::ScaleBy::create(1.5, 100);
			 auto amplify3= cocos2d::ScaleBy::create(1.5, 100);
			 auto f = [&]() {
				 toStatus = 4;
			 };
			 auto finishAction = CallFunc::create(f);
			 auto moveTo1 = MoveBy::create(1.5, Vec3(0, 0, 300));
			 auto moveTo2 = MoveBy::create(1.5, Vec3(0, 0, 300));
			 auto moveTo3 = MoveBy::create(1.5, Vec3(0, 0, 300));
			 
			 auto seq1 = cocos2d::Sequence::create(amplify1, finishAction, nullptr);
			 auto seq2 = cocos2d::Sequence::create(amplify2, finishAction, nullptr);
			 auto seq3 = cocos2d::Sequence::create(amplify3, finishAction, nullptr);

			 auto spr1 = Spawn::create(seq1, moveTo1,nullptr);
			 auto spr2 = Spawn::create(seq2, moveTo2, nullptr);
			 auto spr3 = Spawn::create(seq3, moveTo3, nullptr);

			 subLine1->runAction(spr1);
			 subLine2->runAction(spr2);
			 subLine3->runAction(spr3);

			 drawMother->addChild(subLine1);
			 drawMother->addChild(subLine2);
			 drawMother->addChild(subLine3);


	}
 
	
	angle += 1.5;
	drawMother->setRotation(angle); 
	
// 	body->setRotation3D(Vec3(0, 335, angle));
// 	if(sp1!=NULL)
// 	sp1->setRotation3D(Vec3(angle, 335, angle));
// 	if (sp2 != NULL)
// 	sp2->setRotation3D(Vec3(angle, 335, angle));
// 	if (sp3 != NULL)
// 	sp3->setRotation3D(Vec3(angle, 335, angle));
	if (toStatus >= 3)
	{
		auto sub1rotate = drawMother->getChildByTag(1)->getRotation();
		drawMother->getChildByTag(1)->setRotation(sub1rotate - 2.5);
		auto sub2rotate = drawMother->getChildByTag(2)->getRotation();
		drawMother->getChildByTag(2)->setRotation(sub2rotate - 2.5);
		auto sub3rotate = drawMother->getChildByTag(3)->getRotation();
		drawMother->getChildByTag(3)->setRotation(sub3rotate - 2.5);
	}
	
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
