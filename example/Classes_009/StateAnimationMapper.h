#ifndef STATE_ANIMATION_MAPPER_H
#define STATE_ANIMATION_MAPPER_H

#include <unordered_map>

// 定义道具条件类型的枚举
enum class ItemType {
	ITEM0,
	ITEM1,
	ITEM2,
	ITEM3
};

class StateAnimationMapper {
public:
    struct TypeBindAnimation {
        ItemType itemType;
        int animationId;
		TypeBindAnimation(ItemType type, int id) : itemType(type), animationId(id) {}
    };

    // 重载 mapStateToAnimation 方法，支持根据道具类型映射动画
    void mapStateToAnimation(int stateId, ItemType itemType, int animationId);
	int  getAnimationIdForState(int stateId, ItemType itemType);

private:
    // 使用 unordered_map 存储状态、道具类型和动画资源的映射关系
    std::unordered_map<int, std::vector<TypeBindAnimation>> stateItemAnimationMap;
};

#endif    