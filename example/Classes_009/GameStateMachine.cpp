#include "GameStateMachine.h"

void GameStateMachine::registerState(int state, 
	std::function<void(GameCharacter*)> onEnter  ,
	std::function<void(GameCharacter*)> onExit,
	std::function<void(GameCharacter*, float)> update,
		std::function<void(GameCharacter*)> handleEvent   ){
	states_[state] = { onEnter ,onExit  ,update,handleEvent };
}

bool GameStateMachine::setStateMachine(int state, GameCharacter * character)
{
	auto it = states_.find(state);
	if (it == states_.end())
		return false;

	// 调用旧状态的onExit
	int oldState = character->getCurrentState();

	//角色类本身的状态
	character->setPreState(oldState);
	character->setCurrentState(state);

	
	auto oldIt = states_.find(oldState); 
	if (oldIt != states_.end() && oldIt->second.onExit) {
		oldIt->second.onExit(character);
	}

	// 更新状态并调用onEnter
// 	preState_ = currentState_;
// 	currentState_ = state;

	

	if (it->second.onEnter ) {
		it->second.onEnter(character);
	}

	return true;
}



void GameStateMachine::handleInput(int input, GameCharacter * character)
{
	auto currentState = character->getCurrentState();
	for (const auto& transition : transitions_[currentState]) {
		if ((transition.input & input) && transition.canTransition(character)) {
			if (setStateMachine(transition.to, character)) {
				break;
			}
		}
	}
}

void GameStateMachine::handleEvent(GameCharacter * character)
{
	auto currentState = character->getCurrentState();
	auto it = states_.find(currentState);
	if (it != states_.end()) {
		if (it->second.handleEvent) {
			it->second.handleEvent(character); // 调用当前状态的update
		}
	}
}
void GameStateMachine::update(GameCharacter * character, float dt)
{
	auto currentState = character->getCurrentState();
	auto it = states_.find(currentState);

	if (it != states_.end()) {
		if (it->second.update) {
			it->second.update(character, dt); // 调用当前状态的update
		}
	}
}
// 
// int GameStateMachine::getCurrentState() const
// {
// 	return currentState_;
// }
// 
// int GameStateMachine::getPreState() const
// {
// 	return preState_;
// }

bool StateTransition::canTransition(GameCharacter * character) const

{
	if (filters.size())//无过滤则跳过,有过滤则检测是否成立
	{
		for (const auto& filter : filters) {
			if (!filter(character)) {
				return false;
			}
		}
	}
	return !condition || condition(character);//无条件默认true,有则需要检测是否成立
}


