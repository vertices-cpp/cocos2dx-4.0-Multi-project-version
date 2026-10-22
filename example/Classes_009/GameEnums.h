#ifndef _GAME_ENUMS_H_
#define _GAME_ENUMS_H_


// 在合适的头文件中定义枚举类型，例如在 GameStateMachine.h 中
enum  CHARACTER_STATE {
	STATE_NONE = -1, // 无状态，作为一个特殊标记，可能用于初始化或表示未定义状态
	STATE_IDLE = 1, // 角色闲置状态
	STATE_RUN, // 角色奔跑状态
	STATE_SQUAT_1, // 角色下蹲状态 1
	STATE_SQUAT_2, // 角色下蹲状态 2
	STATE_JUMP, // 角色跳跃状态
	STATE_ATTACK_1, // 角色普通攻击状态 1
	STATE_JUMP_ATTACK_1, // 角色跳跃时攻击状态 1
	STATE_SQUATTING_ATTACK, // 角色下蹲时攻击状态
	STATE_THROW_ITEM, // 角色投掷物品状态
	STATE_HANGING, // 角色悬挂状态
	STATE_SPECIAL_SKILL // 角色使用特殊技状态
};


#endif