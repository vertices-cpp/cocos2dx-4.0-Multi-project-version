 
#include "HelloWorldScene.h"
 
#include "renderer/backend/Device.h"
#include "renderer/backend/Program.h"

USING_NS_CC;

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

Vec2 prePos = Vec2::ZERO;
int src = 1;
int dst = 1;
int srcA = 1;
int dstA = 1;
int op1 = 1;
int op2 = 1;
BlendFunc bf;
const int VAL = 5;
#include <iostream>

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

      visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	tmap = TMXTiledMap::create("map1/map1.tmx");
	addChild(tmap);

      sprite = Sprite::create("circle.png");
    if (sprite == nullptr)
    {
        problemLoading("'circle.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.x/2 + origin.x, visibleSize.y/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 3);
		sprite->setScale(0.2);
    }
	// 顶点着色器代码
	std::string vertSource = R"(
            #version 300 es
            layout(location = 0) in vec4 a_position;
            layout(location = 1) in vec2 a_texCoord;
            layout(location = 2) in vec4 a_color;
            
            uniform mat4 u_MVPMatrix;
            
            out vec2 vTexCoord;
            out vec4 vColor;
            
            void main()
            {
                gl_Position = u_MVPMatrix * a_position;
                vTexCoord = a_texCoord;
                vColor = a_color;
            }
        )";

	// 片段着色器代码
	std::string fragSource = R"(
            #version 300 es
            precision highp float;
            
            uniform sampler2D uTex0;
            
            in vec2 vTexCoord;
            in vec4 vColor;
            
            out vec4 oColor;
            
            void main()
            {
                oColor = vColor * texture(uTex0, vTexCoord);
            }
        )";
	auto program =
		backend::Device::getInstance()->newProgram(vertSource.c_str(),
			fragSource.c_str());
	auto programState = new backend::ProgramState(program);
	setProgramState(programState);


	BlendFunc bf = { backend::BlendFactor::SRC_ALPHA,
					backend::BlendFactor::ONE_MINUS_SRC_ALPHA };
	sprite->setBlendFunc(bf);

	sprite->setProgramState(programState);

	// 设置纹理 Uniform
	auto* texture = sprite->getTexture()->getBackendTexture();

	backend::UniformLocation uniformLocation_tex =
		programState->getUniformLocation("uTex0");

	programState->setTexture(
		uniformLocation_tex, // UniformLocation 对象
		0,                   // 纹理单元 slot（对应 GL_TEXTURE0）
		texture              // 纹理对象
	); // uTex0 对应纹理单元0

	// 设置混合函数（可选）
	BlendFunc blendFunc = { backend::BlendFactor::SRC_ALPHA, backend::BlendFactor::ONE_MINUS_SRC_ALPHA };
	sprite->setBlendFunc(blendFunc);

	// 确保 Program 和 ProgramState 不被提前释放
	program->retain();
	programState->retain();

	auto matrixLocation = programState->getUniformLocation("u_MVPMatrix");
	if (matrixLocation.location[0] != -1) {
		programState->setUniform(matrixLocation, sprite->getNodeToWorldTransform().m, sizeof(float) * 16);
	}

	CC_SAFE_RELEASE(programState);
	CC_SAFE_RELEASE(program);
// 	PbatchBode=ParticleBatchNode::create("111.png");
// 	PbatchBode->retain();
// 	TextureAtlas *tex = TextureAtlas::createWithTexture(Director::getInstance()->getTextureCache()->addImage("111.png"),200);
// 	tex->retain();
//	PbatchBode->setTextureAtlas(tex);
	// 创建一个 ParticleSystemQuad 对象
	particle = ParticleSystemQuad::create();
	if (particle) {
		// 设置粒子的纹理
		particle->setTexture(Director::getInstance()->getTextureCache()->addImage("111.png"));
		particle->setTotalParticles(1);
		particle->setSourcePosition(Vec2(10, -30));
		//	particle->setBatchNode(PbatchBode);
		particle->setDuration(-1);//必须设置时长 
		// 设置粒子系统的位置
		particle->setPosition(sprite->getPosition());

		//
		particle->setPosVar(Vec2(0, 20));

		// 设置粒子的发射率
		particle->setEmissionRate(10);

		// 设置粒子的生命周期
		particle->setLife(20);

		// 设置粒子的初始大小
		particle->setStartSize(10.0f);

		// 设置粒子的结束大小
		particle->setEndSize(20.0f);

		// 设置粒子的颜色
		particle->setStartColor(Color4F::RED);
		particle->setEndColor(Color4F::YELLOW);

		// 设置粒子的起始颜色和透明度
		particle->setStartColor(Color4F(1, 0, 0, 1)); // 红色，完全不透明
		// 设置粒子的结束颜色和透明度
		particle->setEndColor(Color4F(1, 1, 0, 0)); // 黄色，完全透明

		// 新增设置

		// 设置粒子的起始旋转角度
		particle->setStartSpin(0.0f);
		// 设置粒子的结束旋转角度
	//	particle->setEndSpin(360.0f);

		// 设置粒子的起始速度
		particle->setSpeed(50);
		// 		// 设置粒子速度的变化范围
		particle->setSpeedVar(50);

		// 设置粒子的重力
		//particle->setGravity(Vec2(0, -100));

		// 设置粒子的径向加速度
// 		particle->setRadialAccel(50.0f);
// 		// 设置粒子径向加速度的变化范围
// 		particle->setRadialAccelVar(10.0f);
// 
// 		// 设置粒子的切向加速度
// 		particle->setTangentialAccel(30.0f);
// 		// 设置粒子切向加速度的变化范围
// 		particle->setTangentialAccelVar(5.0f);

		// 添加粒子系统到场景
		this->addChild(particle,2);
		 
	}



// 	auto listener = EventListenerMouse::create();
// 	listener->onMouseMove = [&](EventMouse* event)
// 	{
// 		auto mouse = event;
// 		auto pos = event->getLocation();
// 		
// 		//sprite向鼠标方向
// 		auto sprite_diff = pos - prePos; //得到差值
// 
// 		prePos = pos;
// 		auto spriteRadians = atan2(sprite_diff.y, sprite_diff.x) + 3.1415926;
// 		spriteRadians *= (180 / 3.1415926);
// 
// 		std::cout << "spriteRadians:" << spriteRadians << std::endl;
// 
// 		auto worldPos = Director::getInstance()->convertToGL(pos);
// 		auto node = (Sprite*)event->getCurrentTarget();
// 		node->setPosition(worldPos); //sprite位置
// 		//_steak->setPosition(worldPos);
// 
// 		node->setRotation(spriteRadians);
// 
// 		const float offsetX = 30;
// 		const float offsetY = 30;
// 		// 计算粒子相对于sprite的偏移位置
// 		auto particleOffsetX = offsetX * cos(spriteRadians * (3.1415926 / 180));
// 		auto particleOffsetY = offsetY * sin(spriteRadians * (3.1415926 / 180));
// 		pos += Vec2(particleOffsetX, particleOffsetY);
// 		auto particleWorld = Director::getInstance()->convertToGL(pos);
// 		
// 		auto particleRadians = atan2(-sprite_diff.y, sprite_diff.x) + 3.1415926;
// 		particleRadians *= (180 / 3.1415926);
// 		particle->setAngle(fmod(particleRadians, 360));
// 		particle->setPosition(particleWorld);
// 		const char *png[] = { "111.png","b2.png" };
// 		cnt ^= 1;
// 
// 		particle->setTexture(Director::getInstance()->getTextureCache()->addImage(png[cnt]));
// 
// 
// 	};
// 
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sprite);

	scheduleUpdate();
	 

	_steak = MotionStreak::create(
		1.5,   // 拖尾渐隐时间（秒）
		0.01f,      // 最小分段长度
		20,     // 拖尾宽度
		Color3B::WHITE, // 拖尾颜色
	//	"Particle3D/textures/pu_fire_01_64x64.png" // 拖尾纹理（需提供）
	 //	"111.png"
		"page9/steak.png"
	//	"b2.png"
		//"hyoga_pikeB/hyoga_pikeB-7.png"
	);
	_steak->setFastMode(false);
	//	_steak->setScale(20);
	addChild(_steak);
	//_steak->setOpacity(0);
	bf.src = backend::BlendFactor::SRC_ALPHA;
	bf.dst = backend::BlendFactor::DST_COLOR;
	bf = BlendFunc::ALPHA_PREMULTIPLIED;
	_steak->setBlendFunc(bf);
	
	// 3. 初始化黄金色参数
	// 修改后（增强亮度与色彩平衡）
	_goldLight = Color3B(105, 255, 241);   
	_goldDark= Color3B(255, 230, 255);
	_colorTime = 0.0f;
	_reversePhase = false;
 
	_steak->schedule([&](float delta) {


		float x = 200, y = 200;
		float radiusX = 150, radiusY = 40; // 平衡椭圆半径
		float cx = radiusX * cos(angle * 3.1415926 / 180) + x;
		float cy = radiusY * sin(angle * 3.1415926 / 180) + y;
		_steak->setPosition3D(Vec3(cx, cy, 0));

		// 平滑角度增量
		angle += (angleOffset + 220) * delta; // 降低速度
		angle = fmod(angle, 360);
// 		_steak->setColor(Color3B::WHITE);
// 	 
// 		return;

		// 修正angleOffset更新逻辑
		angleOffset = (angleOffset + 1) % 16;

		const float CYCLE_DURATION = 2.0f; // 完整周期时间（亮→暗→亮）

	// 更新时间（0~CYCLE_DURATION循环）
		_colorTime += delta;
		if (_colorTime > CYCLE_DURATION) {
			_colorTime -= CYCLE_DURATION;
		}

		// 计算当前渐变进度（0~1）
		float progress = _colorTime / CYCLE_DURATION;

		// 计算插值方向
		if (progress > 0.5f) {
			// 后半周期：暗→亮
			progress = (progress - 0.5f) * 2.0f;
			_reversePhase = true;
		}
		else {
			// 前半周期：亮→暗
			progress *= 2.0f;
			_reversePhase = false;
		}
		//urrentColor = _goldDark + (_goldLight - _goldDark) * progress 来计算当前颜色。
		// 颜色插值计算
		Color3B currentColor; 
		if (_reversePhase) {
			// 使用 A + (B - A) * progress 形式进行暗到亮的插值
			currentColor.r = _goldDark.r +  (_goldLight.r - _goldDark.r) * progress;
			currentColor.g = _goldDark.g +  (_goldLight.g - _goldDark.g) * progress;
			currentColor.b = _goldDark.b +  (_goldLight.b - _goldDark.b) * progress;
		}
		else {
			// 使用 A + (B - A) * progress 形式进行亮到暗的插值
			currentColor.r = _goldLight.r +  (_goldDark.r - _goldLight.r) * progress;
			currentColor.g = _goldLight.g +  (_goldDark.g - _goldLight.g) * progress;
			currentColor.b = _goldLight.b +  (_goldDark.b - _goldLight.b) * progress;
		}

		// 应用颜色到拖尾
		_steak->setColor(currentColor);
// 		Color4B currentColor4B(currentColor.r, currentColor.g, currentColor.b, 255);
// 		_steak->setColor(currentColor4B);

	}, 0.0001f, " ");


	Vec2 center(200, 150);
	float radius = 80.0f; // 运动半径

	// ===== 第一圈：右上椭圆弧 =====
	ccBezierConfig config1;
	config1.controlPoint_1 = Vec2(200, 100);
	config1.controlPoint_2 = Vec2(200 + 100, 200  ); // 调整控制点以形成圆形转弯
	config1.endPosition = Vec2(100, 100);

	// 配置第二条贝塞尔曲线
	ccBezierConfig config2;
	config2.controlPoint_1 = Vec2(420, 150);
	config2.controlPoint_2 = Vec2(80, 150);
	config2.endPosition = Vec2(0, 150);

	// ===== 第三圈：平滑返回起点 =====
	ccBezierConfig config3;
	// 控制点沿终点切线方向延伸（保持运动惯性）
	config3.controlPoint_1 = config2.endPosition; // 左上方惯性
	config3.controlPoint_2 = center + Vec2(radius*0.5f, radius*0.5f);  // 向中心收敛
	config3.endPosition = config1.controlPoint_1;  // 精确回到起点

	// 创建动作序列（每圈2秒）
	auto action1 = BezierBy::create(2.0f, config1);
	auto action2 = BezierTo::create(2.0f, config2);
	auto action3 = BezierTo::create(2.0f, config3);
	 
//	auto action11 = action1->reverse();

	// 组合三次不同路径的动作
	auto sequence = Sequence::create(
		action1,
	//	action2,
	//	action3,
		nullptr
	);

	// 执行动作（节点需提前创建）
	// _steak->runAction(RepeatForever::create(sequence));
//	_steak->runAction(sequence);
	 
	
	auto mo2 = EventListenerKeyboard::create();
	mo2->onKeyPressed = [&](EventKeyboard::KeyCode key, Event *event) {
		 
		switch (key)
		{ 
		case EventKeyboard::KeyCode::KEY_W:
			spriteVel.y += VAL;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			spriteVel.y -= VAL;
			break;
		case EventKeyboard::KeyCode::KEY_A:
			particle->setAngle(0);
			spriteVel.x -= VAL;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			particle->setAngle(180);
			spriteVel.x += VAL;
			break;
		}


	};
	mo2->onKeyReleased = [&](EventKeyboard::KeyCode key, Event *event) {
 
		switch (key)
		{
		case EventKeyboard::KeyCode::KEY_W:
			spriteVel.y -= VAL;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			spriteVel.y += VAL;
			break;
		case EventKeyboard::KeyCode::KEY_A:
			spriteVel.x += VAL;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			spriteVel.x -= VAL;
			break;
		}


	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mo2, tmap);


	//spriteVel.x = 10;
    return true;
}
using namespace std;

void HelloWorld::update(float delta)
{
	if (spriteVel.x!=0 ||spriteVel.y!=0)
	{
		cout << spritePos.x << " " << spritePos.y << endl;
	}
	if (!((spriteVel.x+spritePos.x < 0) || (spriteVel.x + spritePos.x >tmap->getContentSize().width)))
	{
		spritePos.x += spriteVel.x;
	}
	else
	{
		cout << spritePos.x << " " << spritePos.y << endl;
	}
	if (!((spriteVel.y + spritePos.y < 0) ||( spriteVel.y + spritePos.y > tmap->getContentSize().height)))
	{
		spritePos.y += spriteVel.y;
	}
	else
	{
		cout << spritePos.x << " " << spritePos.y << endl;
	}
	sprite->setPosition(spritePos + tmxPos);
	
	setCamera(); 
	tmap->setPosition(tmxPos);
	 
	particle->setPosition(spritePos+Vec2(0,25));
	//particle->setOffsetPosition(-tmxPos);
	 
}

void HelloWorld::setCamera() {
	Vec2 size = sprite->getContentSize();
	auto dotPos = spritePos /*+ size / 2*/;
	tmxPos = dotPos - visibleSize / 2;

	Vec2 tmpXYpos = tmap->getContentSize();
	if (tmxPos.x < 0)
	{
		tmxPos.x = 0;
	}
	if (tmxPos.y < 0)
	{
		tmxPos.y = 0;
	}
	if (tmxPos.x > tmpXYpos.x - visibleSize.x)
	{
		tmxPos.x = tmpXYpos.x - visibleSize.x;
	}
	if (tmxPos.y > tmpXYpos.y - visibleSize.y)
	{
		tmxPos.y = tmpXYpos.y - visibleSize.y;
	}
	
	tmxPos = -tmxPos;
	//spritePos += tmxPos;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
