#include "Collision.h"

Collision* Collision::getInstance() {
	static Collision instance;
	return &instance;
}

bool Collision::isCollision(const Rect& Arect, const Rect& Brect) {
	return !(Arect.origin.x >= Brect.origin.x + Brect.size.width ||
		Arect.origin.x + Arect.size.width <= Brect.origin.x ||
		Arect.origin.y >= Brect.origin.y + Brect.size.height ||
		Arect.origin.y + Arect.size.height <= Brect.origin.y);
}
 

// 辅助函数，用于找出数组中最小值的索引
int findMinIndex(float values[], int size) {
	int minIndex = -1;
	float minValue = std::numeric_limits<float>::max();
	for (int i = 0; i < size; ++i) {
		if (values[i] < minValue) {
			minValue = values[i];
			minIndex = i;
		}
	}
	return minIndex;
}
#include <iostream>
using namespace std;


CollisionSide Collision::getCollisionSide(const Rect & Arect, const Rect & Brect, int preserveMask) {
	const float EPSILON = 0.001f;
	const float THRESHOLD = 0.0f; // 自定义阈值
	float diffs[4] = {0};
	int validCount = 0;
	int validIndices[4] = { 0 }; // 用于记录有效差值的原始索引

	// 重新调整差值计算逻辑，根据preserveMask计算有效差值
	
	if (!(preserveMask & (int)CollisionSide::LEFT)) {
		diffs[validCount] = (Arect.origin.x + Arect.size.width) - Brect.origin.x;
		validIndices[validCount] = 0;
		validCount++;
	}
	if (!(preserveMask & (int)CollisionSide::RIGHT)) {
		diffs[validCount] = (Brect.origin.x + Brect.size.width) - Arect.origin.x;
		validIndices[validCount] = 1;
		validCount++;
	}
	if (!(preserveMask & (int)CollisionSide::TOP)) {
		diffs[validCount] = (Arect.origin.y + Arect.size.height) - Brect.origin.y;
		validIndices[validCount] = 2;
		validCount++;
	}
	if (!(preserveMask & (int)CollisionSide::BOTTOM)) {
		diffs[validCount] = (Brect.origin.y + Brect.size.height) - Arect.origin.y;
		validIndices[validCount] = 3;
		validCount++;
	}

	if (validCount == 0) {
		return CollisionSide::NONE;
	}

	int minIndex = findMinIndex(diffs, validCount);

	if (validCount > 1) {
		int secondMinIndex = -1;
		float secondMinValue = std::numeric_limits<float>::max();
		for (int i = 0; i < validCount; ++i) {
			if (i != minIndex && diffs[i] < secondMinValue) {
				secondMinValue = diffs[i];
				secondMinIndex = i;
			}
		}
		if (secondMinIndex != -1 && std::abs(diffs[minIndex] - diffs[secondMinIndex]) < THRESHOLD) {
			cout << "认为没有明确的碰撞边" << endl;
			return CollisionSide::NONE; // 认为没有明确的碰撞边
		}
	}

	// 根据有效索引确定碰撞边
	int originalMinIndex = validIndices[minIndex];
	switch (originalMinIndex) {
	case 0: return CollisionSide::LEFT;
	case 1: return CollisionSide::RIGHT;
	case 2: return CollisionSide::TOP;
	case 3: return CollisionSide::BOTTOM;
	default: return CollisionSide::NONE;
	}
}


// 修正后的计算碰撞位置的函数
Vec2 Collision::calculateCollisionPosition(const Rect &Arect, const Rect &Brect, CollisionSide side) {
	switch (side) {
	case CollisionSide::LEFT:
		// 当碰撞边为Arect左侧时，x 坐标为 Brect 的左边界，y 坐标取 Arect 和 Brect 中 y 坐标的最大值
		return Vec2(Brect.origin.x, std::max(Arect.origin.y, Brect.origin.y));
	case CollisionSide::RIGHT:
		// 当碰撞边为Arect右侧时，x 坐标为 Brect 的右边界，y 坐标取 Arect 和 Brect 中 y 坐标的最大值
		return Vec2(Brect.origin.x + Brect.size.width, std::max(Arect.origin.y, Brect.origin.y));
	case CollisionSide::TOP:
		// 当碰撞边为Arect顶部时，x 坐标取 Arect 和 Brect 中 x 坐标的最大值，y 坐标为 Brect 的左上角 y 坐标
		return Vec2(std::max(Arect.origin.x, Brect.origin.x), Brect.origin.y);
	case CollisionSide::BOTTOM:
		// 当碰撞边为Arect底部时，x 坐标取 Arect 和 Brect 中 x 坐标的最大值，y 坐标为 Brect 的下边界
		return Vec2(std::max(Arect.origin.x, Brect.origin.x), Brect.origin.y + Brect.size.height);
	default:
		// 如果没有明确的碰撞边，返回原点
		return Vec2(0, 0);
	}
}


Collision::CollisionResult Collision::checkMapCollision(std::unordered_map<std::string,std::vector<Rect>> mapCollision,
	std::vector<Rect>& my_character,
	int preserveMask) 
{
	box_name = "";
	count = 0;
	CollisionResult result = { -1,-1, CollisionSide::NONE ,Vec2::ZERO };

	for (auto& map_cp : mapCollision) {
		auto cp = map_cp.second;
		for (auto i = 0; i < cp.size(); i++) {
			Rect Arect = { cp[i].origin, cp[i].size };//地图A矩形

			for (int j = 0; j < my_character.size(); ++j) {
				Rect Brect = { my_character[j].origin, my_character[j].size };//人物B矩形

				if (isCollision(Arect, Brect)) {
					if (count >= Gindicate.size()) {
						Gindicate.resize(count + 1, 0);
					}
					CollisionSide side = getCollisionSide(Arect, Brect, preserveMask);
					Gindicate[count] |= static_cast<int>(side); // 位处理
					box_name = map_cp.first;
					try {
						result.boxNumber = std::stoi(box_name);
						result.side = side;
						result.boxItem = j;
						result.collisionPosition = calculateCollisionPosition(Arect, Brect, side);
						result.rect = Arect;
						result.characterPosition = Brect;
						return result;
					}
					catch (const std::invalid_argument& e) {
						// 处理 box_name 不是有效数字字符串的情况
						result.boxNumber = -1;

						result.side = CollisionSide::NONE;
						result.collisionPosition = cocos2d::Vec2(0, 0);
						return result;
					}
				}
				else {
					if (count < Gindicate.size()) {
						//CollisionSide side = getCollisionSide(Arect, Brect, preserveMask);
						Gindicate[count] = (Gindicate[count] & preserveMask);// | (Gindicate[count] & static_cast<int>(side));
		 
					}
				}
				count++;
			}
		}
	}

	return result;
}