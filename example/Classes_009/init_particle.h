#ifndef _INIT_PARTICLES_H_
#define _INIT_PARTICLES_H_
#include <algorithm>

#include "GameCharacter.h"


// struct ParticleColor {
// 	int alpha;  // 透明度 0-255
// 	uint8_t r;  // 红色分量
// 	uint8_t g;  // 绿色分量
// 	uint8_t b;  // 蓝色分量
// };
// 
// ParticleColor calculateParticleEffect(int currentLife, int totalLife) {
// 	// 输入保护
// 	if (totalLife <= 0 || currentLife <= 0) return { 0, 255, 215, 0 };
// 
// 	const float lifeRatio =  ::clampf(currentLife / (float)totalLife, 0.0f, 1.0f);
// 
// 	// 基础黄金色 (255,215,0)
// 	constexpr uint8_t baseR = 255;
// 	constexpr uint8_t baseG = 215;
// 	constexpr uint8_t baseB = 0;
// 
// 	// 基础透明度曲线
// 	float alpha = 0.8f * lifeRatio * lifeRatio + 0.2f * lifeRatio;
// 
// 	// 动态色相控制（黄金色基础上的偏移）
// 	const float colorFreq = 8.0f; // 颜色变化频率
// 	const float phaseShift = 0.5f * M_PI; // 相位差使颜色与闪烁错开
// 
// 	// 红紫闪烁参数
// 	float redShift = 0.5f * sin(colorFreq * lifeRatio * 2 * M_PI);          // 红色波动 [-0.5,0.5]
// 	float purpleShift = 0.3f * sin(colorFreq * lifeRatio * 2 * M_PI + phaseShift); // 紫色波动
// 
// 	// 颜色合成
// 	uint8_t r = static_cast<uint8_t>(baseR + 20 * redShift);   // 红色增强
// 	uint8_t g = static_cast<uint8_t>(baseG - 30 * redShift);   // 绿色衰减
// 	uint8_t b = static_cast<uint8_t>(baseB + 20 * purpleShift);// 蓝色波动制造紫色
// 
// 	// 透明度闪烁（与颜色波动不同步）
// 	const float alphaFlicker = 0.15f * sin(12 * lifeRatio * 2 * M_PI);
// 	alpha = ::clampf(alpha + alphaFlicker, 0.0f, 1.0f);
// 
// 	return {
// 		static_cast<int>(alpha * 255),
// 		::clamp(r, 0, 255),
// 		::clamp(g, 0, 255),
// 		::clamp(b, 0, 255)
// 	};
// }

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
	if (min_inclusive > max_inclusive) {
		std::swap(min_inclusive, max_inclusive);
	}
	return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;

}
inline uint8_t clamp(int value, int min_inclusive, int max_inclusive)
{
	if (min_inclusive > max_inclusive) {
		std::swap(min_inclusive, max_inclusive);
	}
	return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;

}
int calculateParticleAlpha(int currentLife, int totalLife) {
	// 无效输入保护
	if (totalLife <= 0 || currentLife <= 0) return 0;

	// 计算生命周期进度比例（1.0->0.0）
	float lifeRatio =  (currentLife / (float)totalLife);
	 
	float alpha = 0.8f * lifeRatio * lifeRatio  // 主导衰减项
		+ 0.2f * lifeRatio;         

	// 黄金闪烁特效（叠加高频波动）
	const float flickerFreq = 12.0f; // 闪烁频率（值越大闪烁越快）
	const float flickerAmp = 0.28f; // 闪烁强度（建议范围0.1-0.3）
	float flicker = flickerAmp * sin(flickerFreq * lifeRatio * 2 * M_PI);

	// 合成最终透明度
	alpha =  ::clampf(alpha + flicker, 0.0f, 1.0f); // 防止溢出

// 转换为0-255范围并限制溢出
	return alpha * 255.0f;
}

void init_particle() {
	//方法1
	auto global_particle = GameParticle::getinstance();

	auto SpriteParticleInit = [](ParticleInstance& psq) {
		
		

		auto character = psq.owner;
	 
		//for (auto i = 0; i < psq.number; ++i) {
			auto sprite = GameCharacter::createWithTexture(character->getTexture());
			sprite->setOffsetVal(character->getOffsetVal());//获取偏移
			sprite->setFlippedX(character->getFlippedX());//获取翻转
			sprite->setOpacity(150);
			sprite->setGravityEnable(false);
			sprite->setAnchorPoint(character->getAnchorPoint());
			//sprite->setColor(Color3B(30,45,221));
			auto sp_data = new MyUserData;
			sp_data->life = psq.life;
			sp_data->curlife = 0;
			sp_data->stop = false;

			sprite->setCustomData(sp_data);

			Vec2 pos = character->getAtTmxPos();//获取角色位置
			sprite->setAtTmxPos(pos);
			
			auto _mapOffset = GameCore::getInstance()->getTmxPos();
			sprite->setPos(_mapOffset);

			psq.nodes.push_back(sprite);
			psq.zOrder = 2;
		//}
	};
	auto SpriteParticleUpdate = [](ParticleInstance& psq) ->bool{
		auto character = psq.owner;
		auto nodes = psq.nodes;


		auto _mapOffset = GameCore::getInstance()->getTmxPos();
		auto isActive = psq.isActive;//如果调用的自身数量等于0，则表示已经暂停
		
		bool quit = false;
		
		for (auto &i : psq.nodes)
		{

			auto sprite = (GameCharacter*)i;
			

			MyUserData* data = (MyUserData *)sprite->getCustomData();

			if (data->stop)//如果暂停了则不处理了
			{
				sprite->setVisible(false);
				continue;
			}

			data->curlife--;
			auto alpha = calculateParticleAlpha(data->curlife,data->life);
			sprite->setOpacity( alpha);
		//	sprite->setColor(Color3B(rgba.r, rgba. b, rgba.b));
			Vec2 pos = sprite->getAtTmxPos();
			if (data->curlife < 0  )//如果暂停了，则不生成路径
			{ 
				if (!psq.isActive)
				{
					data->stop = true;

				}
				else {
					data->curlife = data->life;
					sprite->setOffsetVal(character->getOffsetVal());//获取偏移
					sprite->setFlippedX(character->getFlippedX());//获取翻转
					sprite->setTexture(character->getTexture());
					sprite->setAnchorPoint(character->getAnchorPoint());
					sprite->setContentSize(character->getContentSize());//已自定义加入纹理矩形设定
				//	sprite->setTextureRect(Rect(Vec2::ZERO,character->getContentSize()));//设置纹理矩形

					Vec2 pos = character->getAtTmxPos();
					sprite->setAtTmxPos(pos);
					sprite->setOpacity(255);
	
				}
			}

			sprite->setPos(_mapOffset);
			  

			quit = true;
		}
		if (!quit)//如果已经暂停则不处理
		{
			return true;
		}
		
		int num = psq.nodes.size();
		if (psq.nodes.size() < psq.number && psq.cur_delay >= psq.delay)
		{
			psq.cur_delay = 0;
			psq.methor.defaultInit(psq);
			psq.parentNode->addChild(psq.nodes[num],2);
		}
		psq.cur_delay++;
		return false;
	};

	global_particle->registerParticleType(1, SpriteParticleInit, SpriteParticleUpdate);


	//---------------------------方法2---------------------------

	auto particleMethor2init = [](ParticleInstance& psq)  {
		auto character = psq.owner;
		auto particle = ParticleSystemQuad::create();
		if (particle) {
			// 设置粒子的纹理
			particle->setTexture(Director::getInstance()->getTextureCache()->addImage("pump_smoke_06.png"));

			

			particle->setTotalParticles(1000);
			particle->setGravity(Vec2(0, 200));

			particle->setPosVar(Vec2(0, 20));
			particle->setSourcePosition(Vec2(0, -30));
			//	particle->setBatchNode(PbatchBode);
			particle->setDuration(-1);//必须设置时长 
			// 设置粒子系统的位置
			particle->setPosition(character->getPosition());



			// 设置粒子的发射率
			particle->setEmissionRate(200);

			// 设置粒子的生命周期
			particle->setLife(3.8);

			// 设置粒子的初始大小
			particle->setStartSize(15.0f);

			// 设置粒子的结束大小
			particle->setEndSize(25.0f);
			// 黄金色基础参数
			const float goldR = 1.0f;    // 红色分量加强
			const float goldG = 0.84f;   // 金色特征绿色分量
			const float goldB = 0.0f;    // 降低蓝色分量
			const float whiteMix = 0.2f; // 白色混合系数

			// 设置起始颜色（黄金色基调）
			particle->setStartColor(Color4F(
				goldR * (1 - whiteMix) + whiteMix,  // R: 1.0 -> 保留金色主调
				goldG * (1 - whiteMix) + whiteMix,  // G: 0.84 -> 向白色过渡
				goldB * (1 - whiteMix) + whiteMix,  // B: 0.0 -> 微量蓝色避免发绿
				1.0f                                // 初始不透明
			));

			// 颜色变化控制（保持金色主调）
			particle->setStartColorVar(Color4F(
				0.1f,  // R变化量（保持高红）
				0.15f, // G变化量（适度波动）
				0.05f, // B变化量（最低限度）
				0.0f   // 透明度不变
			));

			// 结束颜色设置（向白金色过渡）
			particle->setEndColor(Color4F(
				1.0f,      // 最终红色保持最大
				0.95f,     // 绿色接近白色
				0.8f,      // 加入蓝色分量制造冷光效果
				0.0f       // 完全透明
			));

			// 增强视觉效果
			particle->setBlendAdditive(true);


		//	// 设置粒子的起始颜色和透明度
		//	particle->setStartColor(Color4F(0.215,1, 0.429, 1));  
		////	particle->setStartColorVar(Color4F(1, 1, 0.829, 1));
		//	// 设置粒子的结束颜色和透明度
		//	particle->setEndColor(Color4F(1, 1, 0, 0)); 
		//	particle->setEndColorVar(Color4F(1, 1, 1, 0));
			// 新增设置

			// 设置粒子的起始旋转角度
			particle->setStartSpin(0.0f);
			// 设置粒子的结束旋转角度
			particle->setEndSpin(360.0f);

			// 设置粒子的起始速度
			particle->setSpeed(100);
			// 		// 设置粒子速度的变化范围
			particle->setSpeedVar(100);

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
			psq.nodes.push_back(particle);
			psq.zOrder = 4;
		}

		
	};
	auto particleMethor2Update = [](ParticleInstance& psq) ->bool {
		auto character = psq.owner;
		auto charPos = Director::getInstance()->convertToGL(character->getAtTmxPos()+ GameCore::getInstance()->getTmxPos());
 
		//_mapOffset.y = -_mapOffset.y;
		for (auto &particle: psq.nodes)
		{
			ParticleSystemQuad* p = (ParticleSystemQuad*)particle;
			
			p->setPosition(charPos+Vec2(0,10));
			//p->setOffsetPosition(-_mapOffset);
			if (character->isFlippedX())
			{
				p->setAngle(0);
				p->setSourcePosition(Vec2(-5, -30));
			}
			else
			{
				p->setAngle(180);
				p->setSourcePosition(Vec2(24, -30));
			}
		}
		return false;
	};
	global_particle->registerParticleType(2, particleMethor2init, particleMethor2Update);


	//---------------------方法3-------------------
	auto pMethor3init = [](ParticleInstance& psq) {
		auto character = psq.owner;
		auto particle = ParticleSystemQuad::create();
		particle->initWithFile("2.plist");
	//	particle->setSourcePosition(Vec2(12, -30));
		psq.nodes.push_back(particle);
		psq.zOrder = 4;

		Vec2 pos = character->getAtTmxPos();//获取角色位置
		 

		auto _mapOffset =pos +  GameCore::getInstance()->getTmxPos();
		Vec2 cur_pos = Director::getInstance()->convertToGL(_mapOffset);
		particle->setPosition(cur_pos);
	};
	auto pMethor3Update = [](ParticleInstance& psq) ->bool {
		auto character = psq.owner;
// 		auto charPos = character->getAtTmxPos();
// 		Vec2 _mapOffset = GameCore::getInstance()->getTmxPos();
//		Vec2 _curPos = charPos + _mapOffset;

		//_mapOffset.y = -_mapOffset.y;

		Vec2 pos1 = character->getAtTmxPos();//获取角色位置
		Vec2 pos2 = GameCore::getInstance()->getTmxPos();
		auto _mapOffset = pos1 + pos2;
	 
		Vec2 cur_pos = Director::getInstance()->convertToGL(_mapOffset);
		 

		for (auto &particle : psq.nodes)
		{
			ParticleSystemQuad* p = (ParticleSystemQuad*)particle;
			 
		 
			if (character->isFlippedX())
			{
				p->setAngle(0);
			 	 p->setPosition(cur_pos+Vec2(30,-15));
			//	p->setOffsetPosition(-_mapOffset);
			}
			else
			{
				p->setAngle(180);
			 	p->setPosition(cur_pos + Vec2(-15, -15));
			//	p->setOffsetPosition(-_mapOffset);
			}
		}
		return false;
	};
	global_particle->registerParticleType(3, pMethor3init, pMethor3Update);

	//--------------------道具部分--------------------


	my_resources_manage *my_res = my_resources_manage::getInstance();

	auto Item1init = [my_res](ParticleInstance& psq) {
		auto character = psq.owner;

		auto sprite = GameCharacter::createWithTexture(my_res->QuaryTexture(character->getCharId(),"ScarWave"));
		sprite->setOffsetVal(character->getOffsetVal() + Vec2(0, 8));//获取偏移

// 		if (character->getCurAnimationId() >= ANIMATION_HANGING_ATTACK_1 &&
// 			character->getCurAnimationId() <= ANIMATION_HANGING_ATTACK_2)
			sprite->setOffsetVal(character->getOffsetVal());//获取偏移

		sprite->setFlippedX(character->getFlippedX());//获取翻转

		//sprite->setOpacity(100);
		sprite->setGravityEnable(false);
		//	sprite->setAnchorPoint(character->getAnchorPoint());
		sprite->setColor(Color3B(255, 255, 255));
		auto sp_data = new MyUserData;
		sp_data->life = psq.life;
		sp_data->curlife = 0;
		sp_data->stop = false;
		sprite->setCustomData(sp_data);

		psq.zOrder = 4;

		psq.nodes.push_back(sprite);

		//移动速度
		if (character->isFlippedX())
		{
			sprite->setStep(Vec2(-5, 0));
		}
		else
			sprite->setStep(Vec2(5, 0));

		Vec2 pos = character->getAtTmxPos();//获取角色位置
		sprite->setAtTmxPos(pos);

		auto _mapOffset = GameCore::getInstance()->getTmxPos();
		sprite->setPos(_mapOffset);

	};

	auto Item1Update = [](ParticleInstance& psq) ->bool {
		auto character = psq.owner;
		auto nodes = psq.nodes;
		auto _mapOffset = GameCore::getInstance()->getTmxPos();
		
		for (auto &node:nodes)
		{
			auto convertNode = (GameCharacter*)node;
			auto sp_data = (MyUserData*)convertNode->getCustomData();
			if (sp_data->curlife > sp_data->life)
			{
				sp_data->stop = true;
				return true;
			}
			sp_data->curlife++;
			convertNode->runTmxStep();
			convertNode->setPos(_mapOffset);
		}
		return false;
	};
	global_particle->registerParticleType(ITEM | 1, Item1init, Item1Update);
}

#endif
