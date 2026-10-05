#include "StateAnimationMapper.h"

void StateAnimationMapper::mapStateToAnimation(int stateId, ItemType itemType, int animationId) {
	stateItemAnimationMap[stateId].emplace_back(itemType, animationId);
}

int StateAnimationMapper::getAnimationIdForState(int stateId, ItemType itemType) {
	auto stateIt = stateItemAnimationMap.find(stateId);
	if (stateIt != stateItemAnimationMap.end()) {
		const auto& binds = stateIt->second;
		for (const auto& bind : binds) {
			if (bind.itemType == itemType) {
				return bind.animationId;
			}
		}
	}
	return -1;
}