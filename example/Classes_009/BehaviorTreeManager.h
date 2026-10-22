// Classes/Item.h
#ifndef _BEHAVIOR_TREE_MANAGER_H_
#define _BEHAVIOR_TREE_MANAGER_H_

#include "cocos2d.h"

#include <functional>
#include <unordered_map>
 
 
class Enemy;

#include <memory>
#include <vector>

enum Enemy_State {
	Enemy_None,
	Enemy_Death,
	Enemy_Idle,
	Enemy_Move,
	Enemy_Attack,
};

struct BNode/*:public  std::enable_shared_from_this<BNode> */{
	Enemy_State state;
	std::string state_name;

	std::function<bool(Enemy* )> condition = nullptr; 
	
	std::vector<std::shared_ptr<BNode>> children; // 
	~BNode() {
		children.clear();
	}
	// 添加子节点
	void addChild(std::shared_ptr<BNode> childNode) {
		if (childNode) {
			children.push_back(std::move(childNode));
		}
	}

	// 删除子节点（按索引）
	void removeChild(size_t index) {
		if (index < children.size()) {
			children.erase(children.begin() + index);
		}
	}

	 
	bool Traverse(Enemy* enemy);
};


class BehaviorTreeManager {
	std::unordered_map<int, std::shared_ptr<BNode>> _trees;

public:
	static BehaviorTreeManager* getInstance() {
		static BehaviorTreeManager instance;
		return &instance;
	}

	BNode* getNode(int id) {
		auto it = _trees.find(id);
		return it != _trees.end() ? it->second.get() : nullptr;
	}

	void addTree(int id, std::shared_ptr<BNode> tree) {
		_trees[id] = std::move(tree);
	}

	void removeTree(int id) {
		_trees.erase(id); // 自动释放内存
	}
	// 行为树管理器更新
	void getBehavior(int id, Enemy* enemy) {
		if (!enemy|| _trees.empty())
			return  ;

		// 1. 查找对应ID的行为树
		auto treeIt = _trees.find(id);
		if (treeIt == _trees.end()) 
			return  ;
		auto& tree = treeIt->second;

		// 2. 遍历找到当前满足条件的节点
		  tree->Traverse(enemy);
	 
	}
	void createTree();
};

#endif //  _BEHAVIOR_TREE_MANAGER_H_