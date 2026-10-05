#include "HelloWorldScene.h"
#include "renderer/backend/Device.h"
#include "renderer/backend/Program.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
	
	return HelloWorld::create();
}

TMXTiledMap* tmap;
Sprite* sprite;

// 顶点着色器修改（简化坐标系处理）
std::string vertSource = R"(
#version 330 core
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec4 a_color;

uniform mat4 CC_MVPMatrix;
uniform vec2 u_LightPos;

out vec2 vTexCoord;
out vec4 vColor;
out vec2 vFragPos;

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    vec4 worldPos = CC_MVPMatrix * a_position;
    vFragPos = worldPos.xy / worldPos.w * vec2(960, 640);
    vTexCoord = a_texCoord;
    vColor = a_color;
}

)";

// 片段着色器修改（Basic-2D-lighting核心逻辑）
std::string fragSource = R"(
#version 330 core
precision highp float;

uniform vec2 u_LightPos;
uniform sampler2D uTex0;
uniform vec3 u_LightColor;
uniform float u_LightRadius;
uniform float u_LightIntensity;

in vec2 vTexCoord;
in vec4 vColor;
in vec2 vFragPos;

out vec4 oColor;

void main() {
    vec4 texColor = texture(uTex0, vTexCoord);
    
    vec2 lightDir = vFragPos - u_LightPos;
    float distance = length(lightDir);
    float normalizedDist = clamp(distance / u_LightRadius, 0.0, 1.0);
    
    float attenuation = 1.0 - smoothstep(0.2, 0.9, normalizedDist);
    attenuation *= pow(1.0 - normalizedDist, 2.0);
    
    float halo = pow(1.0 - normalizedDist, 4.0) * 0.2;
    
    vec3 lightEffect = u_LightColor * (attenuation + halo) * u_LightIntensity;
    
    float alpha = 1.0 - smoothstep(0.0, 1.0, normalizedDist);
    alpha = pow(alpha, 1.2) * 0.8;

    vec3 finalColor = texColor.rgb * (1.0 + lightEffect * 0.6);
    finalColor = clamp(finalColor, 0.0, 1.2);
    
    oColor = vec4(
        finalColor,
        texColor.a * alpha * 0.7
    );
}
)";


backend::ProgramState* _programState;
Vec2 _lightPosition;

bool HelloWorld::init() {
	if (!Scene::init()) return false;

	// 在AppDelegate.cpp的applicationDidFinishLaunching中添加
	auto glView = _director->getOpenGLView();
	if (!glView) {
		glView = GLViewImpl::create("My Game");
		glView->setFrameSize(960, 640);
		_director->setOpenGLView(glView);
	}

	// 检查OpenGL ES版本
	CCLOG("OpenGL ES Version: %s", glGetString(GL_VERSION));


	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// 创建地图
	tmap = TMXTiledMap::create("map1/map1.tmx");
	addChild(tmap);

	// 创建精灵
	sprite = Sprite::create("circle.png");
	if (sprite) {
		sprite->setPosition((Vec2)visibleSize / 2 + origin);
		sprite->setScale(0.2f);
		addChild(sprite, 3);
	} 
	// 修改着色器创建代码，添加编译状态检查
	auto* device = backend::Device::getInstance(); 

	// 创建着色器程序
	auto program = backend::Device::getInstance()->newProgram(vertSource, fragSource);


	_programState = new backend::ProgramState(program);

	// 设置光照参数
	_lightPosition = sprite->getPosition();
	// 更柔和的参数
	Vec3 lightColor(1.0f, 0.8f, 0.6f);    // 稍暗的暖黄色
	float lightRadius = 200.0f;           // 缩小光照半径
	float lightIntensity = 1.2f;          // 降低强度

	// 绑定Uniform
	auto setUniform = [&](const char* name, auto& value) {
		backend::UniformLocation loc = _programState->getUniformLocation(name);
		_programState->setUniform(loc, &value, sizeof(value));
	};

	setUniform("u_LightPos", _lightPosition);
	setUniform("u_LightColor", lightColor);
	setUniform("u_LightRadius", lightRadius);
	setUniform("u_LightIntensity", lightIntensity);
	 
	// 在设置uniform后添加
	CCLOG("Light Parameters:");
	CCLOG("- Position: (%.1f, %.1f)", _lightPosition.x, _lightPosition.y);
	CCLOG("- Color: (%.2f, %.2f, %.2f)", lightColor.x, lightColor.y, lightColor.z);
	CCLOG("- Radius: %.1f", lightRadius);
	CCLOG("- Intensity: %.2f", lightIntensity);
	// 绑定精灵纹理
	backend::UniformLocation texLoc = _programState->getUniformLocation("uTex0");
	_programState->setTexture(texLoc, 0, sprite->getTexture()->getBackendTexture());

	

	// 正确混合模式：源颜色（光照）的 alpha 通道与目标颜色（地图）进行叠加
	BlendFunc blendFunc{
		backend::BlendFactor::DST_COLOR,      // 源因子 = 源 alpha
		backend::BlendFactor::ONE // 目标因子 = 1 - 源 alpha
	};
	sprite->setBlendFunc(blendFunc);

	// 更新逻辑
	schedule([&](float dt) {
		// 动态半径变化示例（呼吸效果）
		static float pulse = 0.0f;
		pulse += dt * 2.0f;
		float dynamicRadius = lightRadius * (1.0 + sin(pulse) * 0.1f);

		backend::UniformLocation radiusLoc = _programState->getUniformLocation("u_LightRadius");
		_programState->setUniform(radiusLoc, &dynamicRadius, sizeof(dynamicRadius));
	}, "light_update");


	//拖拽事件，
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = [](const std::vector<Touch*>& touches, Event  *event)
	{
		auto touch = touches[0];

		auto diff = touch->getDelta();//获取鼠标移动的偏移
		diff.x = (float)diff.x, diff.y = (float)diff.y;
		//auto node = getChildByTag(1);
		auto node = event->getCurrentTarget();//获取事件绑定的节点
		auto currentPos = node->getPosition();
		node->setPosition(currentPos + diff);//处理偏移
	}; 
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sprite);

	return true;
}