#ifndef _GMAE_CHARACTER_MACHINE_H_
#define _GMAE_CHARACTER_MACHINE_H_
#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

#include "GameEnums.h"
#include "GameCharacter.h"

 

class GameCharacter;

// 状态转换规则
  struct StateTransition {
	int input;
	int from;
	int to;
	std::function<bool(GameCharacter*)> condition = nullptr;
	std::vector< std::function<bool(GameCharacter*)>> filters = {};
	//条件评估
	bool canTransition(GameCharacter* character) const;
} ;

  // 状态生命周期函数
  struct StateCallbacks {
	  std::function<void(GameCharacter*)> onEnter = nullptr;
	  std::function<void(GameCharacter*)> onExit = nullptr;
	  std::function<void(GameCharacter*, float)> update = nullptr;
	  std::function<void(GameCharacter*)> handleEvent = nullptr;
  };
// 核心状态机类
class GameStateMachine {
public:
	

	// 注册状态

	void registerState(int state,
		std::function<void(GameCharacter*)> onEnter = nullptr,
		std::function<void(GameCharacter*)> onExit = nullptr,
		std::function<void(GameCharacter*, float)> update = nullptr,
		std::function<void(GameCharacter*)> handleEvent = nullptr);
 

	// 添加状态转换
	template<typename ...FilterArgs>
	inline void  addTransition(int input, int from, int to, std::function<bool(GameCharacter*)> condition, FilterArgs && ...filterArgs)
	{
		std::vector<std::function<bool(GameCharacter*)>> filters;
		// 修正：对 filterArgs 进行完美转发
		int dummy[] = { 0, (filters.push_back(std::forward<FilterArgs>(filterArgs)), 0)... };
		(void)dummy; // 避免未使用变量警告 
		// 转换规则
		transitions_[from].push_back({ input, from, to, condition, filters });
	}

	// 状态机操作
	bool  setStateMachine(int state, GameCharacter* character);
	void update(GameCharacter* character, float dt);
	
	void handleInput(int input, GameCharacter* character);
	void handleEvent(GameCharacter* character);

// 	int getCurrentState() const;
// 	int getPreState() const;

private:
	std::unordered_map<int, StateCallbacks> states_;
	std::unordered_map<int, std::vector< StateTransition>> transitions_;
// 	int currentState_ = 0;
// 	int preState_ = 0;
};



class StateTable {
	 
	std::unordered_map<int, GameStateMachine*> _state_container;
 
public:
	
	~StateTable() {
		for (auto &i:_state_container)
		{
			if (i.second)
			{
				delete i.second;
			}
		}
		_state_container.clear();
	}
	static StateTable* getInstance() {
		static StateTable _instance;
		return &_instance;
	}
	void add_state(int character_id, GameStateMachine * state)
	{
		auto it = _state_container.find(character_id);
		it == _state_container.end() ? _state_container[character_id] = state : nullptr;
	}
	GameStateMachine * get_character_state(int character_id)
	{
		auto it = _state_container.find(character_id);
		return  it != _state_container.end() ? _state_container[character_id] : nullptr;
	}
};

#endif
 