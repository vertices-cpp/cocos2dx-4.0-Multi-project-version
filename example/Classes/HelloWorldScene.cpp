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

#include<iostream>
#include<string>
using namespace std;

#include "HelloWorldScene.h"
#include "AudioEngine.h"
USING_NS_CC;


MotionStreak* _streak = nullptr;


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
const char * layer_river[] = { "river01","river02","river03","river04",
"river05","river05","river06","river07",
"river08","river09","river10","river11","river12",
"river13","river14","river15","river16" };

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	;

    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	scheduleUpdate();
	AudioEngine::play2d("0013.mp3");
     auto visibleSize = Director::getInstance()->getVisibleSize();
     Vec2 origin = Director::getInstance()->getVisibleOrigin();

	 auto draw1 = DrawNode::create();
	 draw1->drawRect(Vec2(101, 101), Vec2(190, 190) ,Color4F(1, 0, 1, 1));
	 addChild(draw1);
	 auto dn = DrawNode::create();
	 addChild(dn);

	 std::string plistFile = "particle.plist";
	 auto emitter = ParticleSystemQuad::create(plistFile);
	 if (!emitter)
	 {
		 CCLOG("ParticleSystemQuad::create 失败: %s", plistFile);
		 return false;
	 }


// 	 auto mouseListener22 = EventListenerMouse::create();
// 	 mouseListener22->onMouseScroll = [this](EventMouse* event) {
// 		 //获取滚轮滚动值（ > 0 向上滚，放大）
// 
// 		 auto scene = (Scene*)event->getCurrentTarget();
// 
// 		 float scroll = event->getScrollY() ;
// 		 float scaleFactor = 0.02f;
// 		// float newScale = scene->_defaultCamera->getZoom() + (scroll > 0 ? scaleFactor : -scaleFactor);
// 		 scroll += scene->getScale();
// 		 //限制缩放范围（例如 0.5 ~2.0）
// 		 scroll =   scroll < 0.5f ?0.5f :  scroll >2.0f?  2.0f : scroll;
// 
// 		 //可选：以鼠标位置为中心缩放（需要坐标转换）
// 		 //	如果不做坐标转换，地图会以锚点（默认中心）缩放
// 		 scene->setScale( scroll);
// 
// 
// 	 };
// 	 _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener22, this);

	 // 放到屏幕中心
	 emitter->setPosition(Vec2(
		 origin.x + visibleSize.width  * 0.5f,
		 origin.y + visibleSize.height * 0.5f));

	 this->addChild(emitter);
	 // 空心圆：center, radius, angle, segments, drawLineToCenter=false, color
	 dn->drawCircle(Vec2(300, 300), 50, 0, 64, false, Color4F::RED);


	
	 _streak = MotionStreak::create(5.5f, 5.0f, 32.0f, Color3B(255, 255, 255), "circle_texture.png");

	 if (_streak) {
		 this->addChild(_streak);

		 // 启用 Fast Mode (预设为 true)
		 // Fast Mode 在采样点较多时采用单侧计算，大角度转角容易产生断层/交叉扭曲
		 _streak->setFastMode(true);
	 }

	 // 2. 注册鼠标移动监听
	 auto mouseListener_streak = EventListenerMouse::create();
	 mouseListener_streak->onMouseMove = [this](EventMouse* event) {
		 // 获取鼠标当前屏幕坐标并转换为 Cocos 坐标系 (左下角为原点)
		 Vec2 mousePos = event->getLocationInView();
		 //mousePos.y = Director::getInstance()->getWinSize().height - mousePos.y;

		 if (_streak) {
			 // 直接更新 Cocos MotionStreak 的位置
			 _streak->setPosition(Vec2(mousePos.x,mousePos.y));
		 }
	 };

	 _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener_streak, this);
// 
//     /////////////////////////////
//     // 2. add a menu item with "X" image, which is clicked to quit the program
//     //    you may modify it.
// 
//     // add a "close" icon to exit the progress. it's an autorelease object
//     auto closeItem = MenuItemImage::create(
//                                            "CloseNormal.png",
//                                            "CloseSelected.png",
//                                            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
// 
//     if (closeItem == nullptr ||
//         closeItem->getContentSize().width <= 0 ||
//         closeItem->getContentSize().height <= 0)
//     {
//         problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
//     }
//     else
//     {
//         float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
//         float y = origin.y + closeItem->getContentSize().height/2;
//         closeItem->setPosition(Vec2(x,y));
//     }
// 
//     // create menu, it's an autorelease object
//     auto menu = Menu::create(closeItem, NULL);
//     menu->setPosition(Vec2::ZERO);
//     this->addChild(menu, 1);
// 
//     /////////////////////////////
//     // 3. add your codes below...
// 
//     // add a label shows "Hello World"
//     // create and initialize a label
// 
//       auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
//       if (label == nullptr)
//       {
//           problemLoading("'fonts/Marker Felt.ttf'");
//       }
//       else
//       {
//   		auto x = origin.x + visibleSize.width / 2, y = origin.y + visibleSize.height - label->getContentSize().height;
//           // position the label on the center of the screen
//           label->setPosition(Vec2(x,y));
//   		label->setString(to_string(x) + " " + to_string(y));
//           // add the label as a child to this layer
//           this->addChild(label, 1);
//       }
// // // 	
// // // 
// // //     // add "HelloWorld" splash screen"
#include "base/ZipUtils.h" // 需要引入头文件

// 1. 获取 ZIP 文件的真实绝对路径
// 	 std::string zipPath = FileUtils::getInstance()->fullPathForFilename("HelloWorld.zip");
// 
// 	 if (!zipPath.empty()) {
// 		 // 2. 创建 ZipFile 对象解析压缩包
// 		 cocos2d::ZipFile zipFile(zipPath);
// 
// 		 // 3. 从 ZIP 包内读取根目录下的 HelloWorld.png
// 		 ssize_t size = 0;
// 		 unsigned char* buffer = zipFile.getFileData("HelloWorld.png", &size);
// 
// 		 if (buffer && size > 0) {
// 			 // 4. 用内存数据创建 Image 和 Texture
// 			 auto image = new cocos2d::Image();
// 			 if (image->initWithImageData(buffer, size)) {
// 				 auto texture = new cocos2d::Texture2D();
// 				 texture->initWithImage(image);
// 
// 				 // 5. 成功创建 Sprite！
// 				 auto sprite = Sprite::createWithTexture(texture);
// 				 this->addChild(sprite);
// 
// 				 texture->release();
// 			 }
// 			 image->release();
// 			 free(buffer); // 注意释放 ZipFile 分配的内存
// 		 }
// 
// 		 // 6. 如果要读子目录里的 "gl/HelloWorld.png"
// 		 unsigned char* buffer1 = zipFile.getFileData("gl/HelloWorld.png", &size);
// 		 if (buffer1 && size > 0) {
// 			 // 同上逻辑创建 sprite1 ...
// 			 free(buffer1);
// 		 }
// 	 }
       auto sprite = Sprite::create("HelloWorld.png");
	   auto sprite1 = Sprite::create("HelloWorld.png");
       if (sprite == nullptr)
       {
           problemLoading("'HelloWorld.png'");
       }
       else
       {
           // position the sprite on the center of the screen
           sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
   
           // add the sprite as a child to this layer
           this->addChild(sprite, 0);
       }
// 
	//  jtm = TMXTiledMap::create("map2.tmx");
	  jtm = TMXTiledMap::create("map3.tmx");
	  jtm->setScale(0.5);
	addChild(jtm);
	auto fin =  FadeOut::create(0.2);
	sprite->runAction(fin);
	this->setCascadeColorEnabled(true);
// 	auto jtm_pos = jtm->getPosition();
// 	auto jtm_point = jtm->getAnchorPoint();
// 	//auto  zz = jtm->getLayer("Collision")->getTileAt(Vec2(0, 0));
// // 	jtm->getLayer("Collision")->removeChild(zz,1);
// 	//jtm->getLayer("Collision")->removeTileAt(Vec2(0,0));
// // 	auto jtm_num = jtm->getMapSize();
// // 	auto layer = jtm->getLayer("brand_attr");
// // 	auto v2 = layer->getPosition();
// // 	layer->setPosition(Vec2(v2.x, v2.y + 5));
//  
// 
//  	/*auto rotateZ = RotateBy::create(3.1f, Vec3(-70.0f, 0.0f, 0.0f));
//  	auto moveTo = MoveTo::create(3.1f, Vec3(20, 20, 0));
//  	auto ScaleT = ScaleTo::create(3.1f, 1.5);
//  
//  	jtm->runAction(Spawn::create(rotateZ,moveTo, ScaleT,NULL));
//  
//  	for (int map_x = 0; map_x < jtm_num.width; map_x++)
//  	{
//  		for (int map_y = 0; map_y < jtm_num.height; map_y++)
//  		{
//  			auto lay = jtm->getLayer("Layer1");
//  			if (lay!=NULL && lay->getTileAt(Vec2(map_x, map_y)) != NULL)
//  			{
//  				auto tile_sprite = lay->getTileAt(Vec2(map_x, map_y));
//  				auto tP = tile_sprite->getPosition();
//  				int x = tP.x, y = tP.y;
//  				string map_text = to_string(x) + "-" + to_string(y);
//  					auto map_info = Label::createWithTTF(map_text, "fonts/Marker Felt.ttf", 8);
//  					map_info->setPosition(tP);
//  					jtm->addChild(map_info);
//  					cout << "map_x:"<<map_x << " map_y:" << map_y << " position= " << map_text << endl;
//  					auto rotateZ = RotateBy::create(3.1f, Vec3(80.0f, 0.0f, 0.0f));
//  					map_info->runAction(rotateZ);
//  			}
//  
//  		}
//  	}*/
// 	
// 
// 	//jtm->setRotation3D(Vec3(90, 0, 0));
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = [](const std::vector<Touch*>& touches, Event  *event)
	{
		auto touch = touches[0];

		auto diff = touch->getDelta();
		diff.x = (float)diff.x, diff.y = (float)diff.y;
		//auto node = getChildByTag(1);
		auto node = event->getCurrentTarget();
		auto currentPos = node->getPosition();
		node->setPosition(currentPos + diff);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, jtm);
	
	// 鼠标滚轮缩放
	auto tmx_mouseListener = EventListenerMouse::create();
	tmx_mouseListener->onMouseScroll = [this](EventMouse* event) {
		// 获取滚轮滚动值（>0 向上滚，放大）

		auto tmx_map =(Scene*) event->getCurrentTarget();
		float scroll = event->getScrollY();
		float scaleFactor = 0.1f;
		float newScale = tmx_map->getScale() + (scroll > 0 ? scaleFactor : -scaleFactor);

		// 限制缩放范围（例如 0.5 ~ 2.0）
		newScale = std::max(0.5f, std::min(newScale, 2.0f));

		// 可选：以鼠标位置为中心缩放（需要坐标转换）
		// 如果不做坐标转换，地图会以锚点（默认中心）缩放
		tmx_map->setScale(newScale);
		//Director::getInstance()->setContentScaleFactor(newScale);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(tmx_mouseListener, jtm);

	

// 	particle = cocos2d::ParticleMeteor::create();
// 	particle->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
// 	//particle->setStyle(ParticleExample::METEOR);
// 	jtm->addChild(particle,0,0);
// 	//particle->setRotation(50);
// 	auto particle_mouseListener = EventListenerMouse::create();
// 	particle_mouseListener->onMouseScroll = [this](EventMouse* event) {
// 	 
// 		auto particle = (ParticleMeteor*)event->getCurrentTarget();
// 		float scroll = event->getScrollY();
// 		//float value = particle->getRotation();
// 		float value = particle->getSkewX();
// 		float newRotate =  (scroll > 0 ?  value +1 : value-1);
// 		particle->setTotalParticles(particle->getTotalParticles() + (int)newRotate);
// 		//particle->setSkewX(newRotate);
// 		//particle->setRotation3D(Vec3(0,0,newRotate )); 
// 	};
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(particle_mouseListener, particle);
// 	 
// 	particle->setVisible(true);
// 
// 	particle->setLocalZOrder(3);
// 	auto mouseEvent = EventListenerMouse::create();
// 
// 	particle->setPosition(Vec2(200, 200));
// 	auto keyEvent = EventListenerKeyboard::create();
// 	keyEvent->onKeyPressed = [](EventKeyboard::KeyCode key, Event* event) {
// 		auto ps = (ParticleMeteor*)event->getCurrentTarget();
// 
// 		auto newScale = ps->getScale();
// 		auto newRotate = ps->getRotation();
// 		//ps->setStyle(ParticleExample::FIRE);
// 		switch (key)
// 		{
// 		case EventKeyboard::KeyCode::KEY_1:
// 
// 			ps->setScale(newScale + 1.0f);
// 			break;
// 		case EventKeyboard::KeyCode::KEY_2:
// 
// 			ps->setScale(newScale - 1.0f);
// 			break;
// 		case EventKeyboard::KeyCode::KEY_3:
// 
// 			ps->setRotation(newRotate + 10.0f);
// 			break;
// 		case EventKeyboard::KeyCode::KEY_4:
// 
// 			ps->setRotation(newRotate - 10.0f);
// 			break;
// 		}
// 
// 	};
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyEvent, particle);
// 	mouseEvent->onMouseMove = [&](EventMouse *event)
// 	{
// 		auto particle = ((ParticleFire*)event->getCurrentTarget());// ->getLayer("Layer1");
// 		Vec2 mousePos = event->getLocation();
// 		  mousePos = event->getLocationInView();
// 		particle->setPosition(mousePos);
// 	};
// 	// 
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent, this);
// 	particle->setAnchorPoint(Vec2(0,0));
// //	sprite->setAnchorPoint(Vec2(0, 0));
// 	// 鼠标滚轮缩放
// 	auto mouseListener = EventListenerMouse::create();
// 	mouseListener->onMouseScroll = [this](EventMouse* event) {
// 		// 获取滚轮滚动值（>0 向上滚，放大）
// 
// 		auto ps = (ParticleSystem*)event->getCurrentTarget();
// 		float scroll = event->getScrollY();
// 		float scaleFactor = 1.0f;
// 		auto newScale = ps->getScale();
// 		newScale += (scroll > 0 ? scaleFactor : -scaleFactor); 
// 		 
//  
// 		// 可选：以鼠标位置为中心缩放（需要坐标转换）
// 		// 如果不做坐标转换，地图会以锚点（默认中心）缩放
// 		ps->setScale(newScale);
// 	};
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, particle);

	/*auto keyEvent = EventListenerKeyboard::create();
	keyEvent->onKeyPressed = [](EventKeyboard::KeyCode key, Event* event) {
		static int  pos = 0;
		std::vector<Vec2> v = { Vec2::ZERO,
 Vec2::ONE ,
  Vec2::UNIT_X ,
  Vec2::UNIT_Y ,
 Vec2::ANCHOR_MIDDLE,
 Vec2::ANCHOR_BOTTOM_LEFT,
 Vec2::ANCHOR_TOP_LEFT,
  Vec2::ANCHOR_BOTTOM_RIGHT,
 Vec2::ANCHOR_TOP_RIGHT,
  Vec2::ANCHOR_MIDDLE_RIGHT,
 Vec2::ANCHOR_MIDDLE_LEFT,
  Vec2::ANCHOR_MIDDLE_TOP,
 Vec2::ANCHOR_MIDDLE_BOTTOM };
		switch (key)
		{
		case EventKeyboard::KeyCode::KEY_1:
			auto ps = (ParticleSystem*)event->getCurrentTarget();
			ps->setAnchorPoint(v[pos]);
			pos++;
			if (pos>=v.size())
			{
				pos = 0;
			}
			break;
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyEvent , particle);*/

    return true;
}
int  i = 0;
float j = 0;
void HelloWorld::update(float delta)
{
// 	auto p = particle->getPosition();
// 	p = CC_POINT_POINTS_TO_PIXELS(p);
// 	
// 
// 	// there are only 4 layers. (grass and 3 trees layers)
// 	// if tamara < 30, z=4
// 	// if tamara < 60, z=3
// 	// if tamara < 90,z=2
// 
// 	int newZ = 4 - (static_cast<int>(p.y) / 30);
// 	newZ = std::max(newZ, 0);
// 
// 	jtm->reorderChild(particle, newZ);

// 	j += delta;
// 	auto layer = jtm->getLayer(layer_river[i]);
// 	layer->setVisible(false);
// // 	 if (j >= 0.25f)
// // 	{
//   		i++; j = 0.0f;
// // 	} 
// 	int size_arr = sizeof(layer_river) / sizeof(layer_river[0]);
// 	if (i>= 2)
// 	{
// 		i = 0;
// 	}
// 	jtm->getLayer(layer_river[i])->setVisible(true);
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
	
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
	;
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
