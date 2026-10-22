 
#include "BehaviorTreeManager.h"
#include "Enemy.h"

bool BNode::Traverse(Enemy * enemy)
{
	if (condition&&condition(enemy)){
		return true;
	}
	for (auto &child : children)
	{
		// 递归检查子节点
		if (child->Traverse(enemy))
			return true;
	}
	return false;
}

float euclidean(const Vec2 & a, const Vec2 & b)
{
	float dx = abs(a.x - b.x);
	float dy = abs(a.y - b.y);
	return sqrt(dx * dx + dy * dy);
}




void BehaviorTreeManager::createTree()
{
	auto enemy001_root = std::make_shared<BNode>();
	enemy001_root->state_name = "Enemy Death";
	enemy001_root->state = Enemy_Death; // 设置初始状态

	//初始检测是不是死亡或HP < 0
	auto Enemy_Death_condition = [](Enemy *enemy)->bool
	{
		return  (enemy->getHp() < 0 || enemy->getState() == Enemy_Death);
	};
	enemy001_root->condition = Enemy_Death_condition; 

	_trees[ENEMY_001] = enemy001_root;

	//待机
	auto enemy001_idle = std::make_shared<BNode>();

	enemy001_root->addChild(enemy001_idle);
	enemy001_idle->state_name = "idle";
	enemy001_idle->state = Enemy_Idle;

	enemy001_idle->condition = [](Enemy *enemy)->bool {

		auto count_rand = rand() % 2;

		switch (count_rand)
		{
		case  1:
			//enemy->setCurrentMachine(STATE_IDLE); //则等待10
			return false;
 
		default:
		{
			return false; 
		}
		} 
	}; 

	//移动

	auto enemy001_move = std::make_shared<BNode>();
	
	enemy001_root->addChild(enemy001_move);

	enemy001_move->state_name = "move";
	enemy001_move->state = Enemy_Move;

	auto Enemy_Move_condition = [](Enemy *enemy)->bool
	{
		auto cur_play = enemy->get_play();//如果有2对象则选择一个最近对象
		int sel_play = 0;
		GameCharacter *play = nullptr;
		
		if (cur_play == 1)
		{
			play =  enemy->getPlayPtr(0);
		}
		if (cur_play == 2)
		{
			GameCharacter *play1 = enemy->getPlayPtr(0);
			GameCharacter *play2 = enemy->getPlayPtr(1);

			Vec2 playTmxPos1 = play1->getAtTmxPos()   ;
			Vec2 playTmxPos2 = play2->getAtTmxPos()  ;

			Vec2 enemy_pos = enemy->getAtTmxPos()  ;

			float distance_1 = euclidean(enemy_pos, playTmxPos1);
			float distance_2 = euclidean(enemy_pos, playTmxPos2);


			if (distance_1 >= distance_2)
			{
				sel_play++;

				play = play2;
			}
			else
			{
				play = play1; 
			}
			enemy->set_cur_play(sel_play);
		}
	

		if (play != nullptr)
		{
			
			enemy->setCurrentMachine(STATE_RUN);

			return true;
		}
		return false;
	};

	enemy001_move->condition = Enemy_Move_condition;

	
}

