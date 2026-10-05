#ifndef _COLLISION_H_
#define _COLLISION_H_
#include "cocos2d.h" 
 
#include <vector>

USING_NS_CC;
  
//extern std::vector<int> Gindicate;

 
// 定义碰撞边的枚举类型
enum class CollisionSide {
	NONE = 0,
	LEFT = 1 << 0,  // 0001
	RIGHT = 1 << 1, // 0010
	TOP = 1 << 2,   // 0100
	BOTTOM = 1 << 3 // 1000
};
class Collision:public Ref
{
	std::string box_name;
	int count;
	std::vector<int> Gindicate;
public:

	struct CollisionResult {
		int boxNumber;
		int boxItem;
		CollisionSide side;
		Vec2 collisionPosition; // 新增碰撞位置信息
		Rect characterPosition;
		Rect rect;
	};
	const std::vector<int>& getGindicate() const {
		return Gindicate;
	}

	void setGindicate(int index, int value) {
		if (index < (int)Gindicate.size()) {
			Gindicate[index] = value;
		}
	}

	void resizeGindicate(size_t size) {
		Gindicate.resize(size, 0); // 初始值设为 0
	}

	int getCount() { return count; }
	std::string getBoxName() { return box_name; }
	static Collision *getInstance();
	bool isCollision(const Rect & Arect, const Rect & Brect);
 
	 CollisionSide getCollisionSide(const Rect & Arect, const Rect & Brect, int preserveMask);

	 Vec2 calculateCollisionPosition(const Rect & Arect, const Rect & Brect, CollisionSide side);

	CollisionResult Collision::checkMapCollision(std::unordered_map<std::string, std::vector<Rect>> mapCollision,
		std::vector<Rect>& my_character,
		int preserveMask);
	 bool init() { return true; }
	Collision() {}
	~Collision() {} 
 

private:
	 
};


#endif // !_COLLISION_H_


