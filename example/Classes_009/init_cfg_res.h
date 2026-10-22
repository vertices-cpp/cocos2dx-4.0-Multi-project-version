#ifndef _INIT_RES_H_
#define _INIT_RES_H_
/*
#include "GameCharacter.h"

void init_res()
{

	auto res = my_resources_manage::getInstance();
	 
	auto idle_animation = new my_animation_cfg(); // 创建空闲动画配置对象
	auto run_animation = new my_animation_cfg(); // 创建奔跑动画配置对象
	auto squat_animation_1 = new my_animation_cfg(); // 创建蹲下1动画配置对象
	auto squat_animation_2 = new my_animation_cfg(); // 创建蹲下2动画配置对象
	auto jump_animation = new my_animation_cfg(); // 创建跳跃动画配置对象
	auto hanging_animation = new my_animation_cfg(); // 创建挂杆动画配置对象
	idle_animation->addAnimationCfg("stand-01", 10); // 配置空闲动画帧为 "stand-01"，延迟 10
	run_animation->addAnimationCfg("run-01", 8, "run-02", 8, "run-03", 8); // 配置奔跑动画帧为 "run-01"、"run-02"、"run-03"，延迟 10
	squat_animation_1->addAnimationCfg("squat-01", 10); // 配置蹲下1动画帧为 "squat-01"，延迟 10
	squat_animation_2->addAnimationCfg("squat-01", 10); // 配置蹲下2动画帧为 "squat-01"，延迟 10
	jump_animation->addAnimationCfg("jmp", 10); // 配置跳跃动画帧为 "jmp"，延迟 10
	hanging_animation->addAnimationCfg("hang-01", 10); // 配置挂杆动画帧为 "hang-01"，延迟 10
	res->addAnimationPlayCfg(ANIMATION_IDLE, idle_animation); // 将空闲动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_RUN, run_animation); // 将奔跑动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_SQUAT_1, squat_animation_1); // 将蹲下1动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_SQUAT_2, squat_animation_2); // 将蹲下2动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_JUMP, jump_animation); // 将跳跃动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_HANGING, hanging_animation); // 将挂杆动画配置添加到资源管理器    


	// 假设已经有 my_animation_cfg 类和 my_resources_manage 类的定义
// 以及 ANIMATION_XXX 这些动画 ID 的定义

// 继续创建新的 my_animation_cfg 对象
	auto hanging_move = new my_animation_cfg(); // 创建挂杆移动动画配置对象
	auto hanging_roll_up = new my_animation_cfg(); // 创建翻转向上动画配置对象
	auto hanging_roll_down = new my_animation_cfg(); // 创建翻转向下动画配置对象
	auto attack_1 = new my_animation_cfg(); // 创建站攻击1动画配置对象
	auto attack_2 = new my_animation_cfg(); // 创建站攻击2动画配置对象
	auto squatting_attack_1 = new my_animation_cfg(); // 创建蹲着攻击1动画配置对象
	auto squatting_attack_2 = new my_animation_cfg(); // 创建蹲着攻击2动画配置对象
	auto jump_attack_1 = new my_animation_cfg(); // 创建跳攻击1动画配置对象
	auto jump_attack_2 = new my_animation_cfg(); // 创建跳攻击2动画配置对象
	auto hanging_attack_1 = new my_animation_cfg(); // 创建挂杆攻击1动画配置对象
	auto hanging_attack_2 = new my_animation_cfg(); // 创建挂杆攻击2动画配置对象
	auto throw_item = new my_animation_cfg(); // 创建扔道具动画配置对象
	auto squatting_throw_item = new my_animation_cfg(); // 创建蹲着扔道具动画配置对象
	auto jump_throw_item = new my_animation_cfg(); // 创建跳着扔道具动画配置对象
	auto hanging_throw_item = new my_animation_cfg(); // 创建挂杆扔道具
	auto special_attack = new my_animation_cfg(); //特殊攻击动画配置对象

	// 挂杆移动
	hanging_move->addAnimationCfg("hang-forward-01-01", 10, "hang-forward-01-02", 10, "hang-forward-01-03", 10); // 配置挂杆移动动画帧及延迟
	// 翻转
	hanging_roll_up->addAnimationCfg("roll-01-01", 5, "roll-01-02", 5, "roll-01-03", 5, "roll-01-04", 5); // 配置翻转向上动画帧及延迟
	hanging_roll_down->addAnimationCfg("roll-01-04", 5, "roll-01-03", 5, "roll-01-02", 5, "roll-01-01", 5); // 配置翻转向下动画帧及延迟
	// 站攻击
	attack_1->addAnimationCfg("attack-01-01", 3, "attack-01-02", 3, "attack-01-03", 3); // 配置站攻击1动画帧及延迟
	attack_2->addAnimationCfg("attack-02-01", 3, "attack-02-02", 3, "attack-02-03", 3, "attack-02-04", 3); // 配置站攻击2动画帧及延迟
	// 蹲着攻击
	squatting_attack_1->addAnimationCfg("squat-attack-01-01", 3, "squat-attack-01-02", 3, "squat-attack-01-03", 3); // 配置蹲着攻击1动画帧及延迟
	squatting_attack_2->addAnimationCfg("squat-attack-02-01", 3, "squat-attack-02-02", 3, "squat-attack-02-03", 3); // 配置蹲着攻击2动画帧及延迟
	// 跳攻击
	jump_attack_1->addAnimationCfg("jmp-attack-01-01", 3, "jmp-attack-01-02", 3, "jmp-attack-01-03", 3); // 配置跳攻击1动画帧及延迟
	jump_attack_2->addAnimationCfg("jmp-attack-02-01", 3, "jmp-attack-02-03", 3, "jmp-attack-02-02", 3); // 配置跳攻击2动画帧及延迟
	// 挂杆攻击
	hanging_attack_1->addAnimationCfg("hang-attack-01-01", 3, "hang-attack-01-02", 3, "hang-attack-01-03", 3); // 配置挂杆攻击1动画帧及延迟
	hanging_attack_2->addAnimationCfg("hang-attack-02-01", 3, "hang-attack-02-02", 3, "hang-attack-02-03", 3); // 配置挂杆攻击2动画帧及延迟
	// 扔道具
	throw_item->addAnimationCfg("attack-05-01", 5, "attack-05-02", 5); // 配置扔道具动画帧及延迟
	squatting_throw_item->addAnimationCfg("squat-attack-03-01", 5, "squat-attack-03-02", 5); // 配置蹲着扔道具动画帧及延迟
	jump_throw_item->addAnimationCfg("jmp-attack-03-01", 5, "jmp-attack-03-02", 5); // 配置跳着扔道具动画帧及延迟
	hanging_throw_item->addAnimationCfg("hang-attack-03-01", 5, "hang-attack-03-02", 5); // 配置挂杆扔道具动画帧及延迟

	special_attack->addAnimationCfg("attack-06-01", 10, "attack-06-02", 10, "attack-06-03", 10, "attack-06-03", 20,
		"attack-06-04", 30, "attack-06-04", 10); // 配置特殊攻击动画帧及延迟

	//添加触发攻击事件的
	auto attackEvent = [](GameCharacter *character) {
		character->addItem(ITEM | 1, 1, 1, 10); 
	};
	//frame,event
	attack_1->addEvent(1, attackEvent);
	attack_2->addEvent(1, attackEvent);
	squatting_attack_1->addEvent(1, attackEvent);
	squatting_attack_2->addEvent(1, attackEvent);
	jump_attack_1->addEvent(1, attackEvent);
	jump_attack_2->addEvent(1, attackEvent);
	hanging_attack_1->addEvent(1, attackEvent);
	hanging_attack_2->addEvent(1, attackEvent);

// 将这些配置添加到资源管理器中
	res->addAnimationPlayCfg(ANIMATION_HANGING_MOVE, hanging_move); // 将挂杆移动动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_HANGING_ROLL_UP, hanging_roll_up); // 将翻转向上动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_HANGING_ROLL_DOWN, hanging_roll_down); // 将翻转向下动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_ATTACK_1, attack_1); // 将站攻击1动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_ATTACK_2, attack_2); // 将站攻击2动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_SQUATTING_ATTACK_1, squatting_attack_1); // 将蹲着攻击1动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_SQUATTING_ATTACK_2, squatting_attack_2); // 将蹲着攻击2动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_JUMP_ATTACK_1, jump_attack_1); // 将跳攻击1动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_JUMP_ATTACK_2, jump_attack_2); // 将跳攻击2动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_HANGING_ATTACK_1, hanging_attack_1); // 将挂杆攻击1动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_HANGING_ATTACK_2, hanging_attack_2); // 将挂杆攻击2动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_THROW_ITEM, throw_item); // 将扔道具动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_SQUATTING_THROW_ITEM, squatting_throw_item); // 将蹲着扔道具动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_JUMP_THROW_ITEM, jump_throw_item); // 将跳着扔道具动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_HANGING_THROW_ITEM, hanging_throw_item); // 将挂杆扔道具动画配置添加到资源管理器
	res->addAnimationPlayCfg(ANIMATION_SPECIAL_ATTACK, special_attack); // 将特殊攻击动画配置添加到资源管理器        
}
*/
#endif
