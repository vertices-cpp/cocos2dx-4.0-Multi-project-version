#ifndef _LOAD_JSON_FILE_H_
#define _LOAD_JSON_FILE_H_

#include "cocos2d.h"
USING_NS_CC;

#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <fstream>

extern int global_characterID; //表示是角色ID计数器
extern int global_textureCountID;
extern std::unordered_map<std::string, unsigned> global_my_character_mapper;

extern std::string readFile(const std::string & path);
extern std::string StripPathRemoveExtension(const std::string &path);



 

class GameCharacter;

//映射函数
extern  std::unordered_map<std::string, int> mapper_function_id;  // 新增帧名映射
extern  std::unordered_map<int, std::function<void(GameCharacter*)>> mapper_function;  // 新增帧名映射

struct my_animation_cfg {

	bool frameLoop;
	std::unordered_map<int, std::function<void(GameCharacter*)>> frameEvents; // 帧事件

	std::vector<int> frameSequenceId;//所包含纹理名称转换后 的动画ID
	std::vector <int> frameDelays;//每帧延迟数
	int totalframe = 0;

	void addEvent(int frame, std::function<void(GameCharacter*)> frameEvent)//帧。事件回调用
	{
		if (frame < totalframe)
		{
			frameEvents[frame] = frameEvent;
		}
	}
	// 终止递归的基函数
	void addAnimationCfg() {}

	// 使用 SFINAE 进行参数检查
// 	template<typename First, typename Second, typename... Args>
// 	typename std::enable_if<
// 		std::is_convertible<First, std::string>::value &&
// 		std::is_arithmetic<Second>::value
// 	>::type
// 		addAnimationCfg(First first, Second second, Args... args) {
// 		auto frameId = GframeNameToId[std::string(first)];
// 		frameSequenceId.push_back(frameId);
// 		frameDelays.push_back(static_cast<int>(second));
// 		totalframe = frameSequenceId.size();
// 		addAnimationCfg(args...); // 继续处理剩余参数
// 	} 
	// 使用 SFINAE 进行参数检查
	template<typename T_, typename... Args>
	typename std::enable_if<
		std::is_arithmetic<T_>::value
	>::type
		addAnimationCfg(T_ first, T_ second, Args... args) {
		//auto frameId = GframeNameToId[std::string(first)];
		frameSequenceId.push_back(first);
		frameDelays.push_back(static_cast<int>(second));
		totalframe = frameSequenceId.size();
		addAnimationCfg(args...); // 继续处理剩余参数
	}
};

struct Texture2dMetadata {
	Texture2D * texture;//纹理
	std::vector<Rect> collisiion_atlas;//碰撞盒
	std::vector<Rect > attack_atlas;//攻击盒
	Vec2   anchor_point;// 精灵锚点

};

struct CharacterResources {
	using int_frame_id = int;
	using int_action_id = int;

	int _characterID;
	std::string _character_name;

	std::unordered_map<int_frame_id, Texture2dMetadata*> _my_frame_res;//frame id +数据集 
	std::unordered_map <int_action_id, my_animation_cfg*> _my_animation_play_cfg; // 动画资源,这个int表示动画ID


	std::unordered_map<std::string, int_frame_id> mapper_frame;//映射的帧ID
	std::unordered_map<std::string, int_action_id> mapper_action;//映射的动作ID

public:
	int getCharacterID() {
		return _characterID;
	}
	std::string getCharacterName() { return _character_name; }

	void checkAction(int_action_id actionId)
	{
		// 确保动作ID对应的动画配置已存在，不存在则创建
		if (_my_animation_play_cfg.find(actionId) == _my_animation_play_cfg.end()) {
			_my_animation_play_cfg[actionId] = new my_animation_cfg();
		}
	}
	void set_action_loop(int action_id, bool loop)
	{
		checkAction(action_id);
		_my_animation_play_cfg[action_id]->frameLoop = loop;
	}

	void addEvent(int action_id, int frame, std::function<void(GameCharacter*)> frameEvent)//帧。事件回调用
	{
		checkAction(action_id);
		_my_animation_play_cfg[action_id]->addEvent(frame, frameEvent);
	}
	// 2. 递归添加动画帧序列（针对某个动作ID）
	// 参数格式：action_id, "frame1", delay1, "frame2", delay2, ...
	template<typename... Args>
	void addAnimationFrames(int_action_id action_id, Args... args) {
		checkAction(action_id);
		// 调用my_animation_cfg的addAnimationCfg递归添加帧
		_my_animation_play_cfg[action_id]->addAnimationCfg(args...);
	}

	void set_frameID_mapper(const std::string &frame_name, int frame_id)
	{
		mapper_frame[frame_name] = frame_id;
	}
	void set_actionID_mapper(const std::string &action_name, int action_id)
	{
		mapper_action[action_name] = action_id;
	}
	int_frame_id get_mapper_frame_id(const std::string &frame_name) //获取映射的帧ID
	{
		auto it = mapper_frame.find(frame_name);
		return it != mapper_frame.end() ? it->second : -1;
	}
	int_action_id get_mapper_action_id(const std::string &action_name)//获取映射的动作ID
	{
		auto it = mapper_action.find(action_name);
		return it != mapper_action.end() ? it->second : -1;
	}

	void addIdCollisionAtlas(int i, std::vector<Rect> c)
	{
		_my_frame_res[i]->collisiion_atlas = c;
	}
	void addIdAttackAtlas(int i, std::vector<Rect> c)
	{
		_my_frame_res[i]->attack_atlas = c;
	}


	void addTextureMeta(int frame_Id)
	{
		_my_frame_res[frame_Id] = new Texture2dMetadata;
	}

	void addIdAnchorPoint(int frame_Id, Vec2 anchor_point)
	{
		_my_frame_res[frame_Id]->anchor_point = anchor_point;
	}
	void addIdTexture2d(int frame_Id, Texture2D * texture2d)
	{
		_my_frame_res[frame_Id]->texture = texture2d;
	}
	std::vector<Rect> get_attack_atlas(int frame_id) {
		auto it = _my_frame_res.find(frame_id);
		std::vector<Rect > r;

		if (it != _my_frame_res.end())
			r = it->second->attack_atlas;
		return r;

	}
	// 返回 _my_collisiion_atlas 的指针
	std::vector<Rect>  get_collision_atlas(int frame_id) {
		auto it = _my_frame_res.find(frame_id);
		std::vector<Rect > r;

		if (it != _my_frame_res.end())
			r = it->second->collisiion_atlas;
		return r;

	}

	// 返回 _my_anchor_point 的指针
	Vec2 get_anchor_point(int frame_id) {
		auto it = _my_frame_res.find(frame_id);
		Vec2 r;

		if (it != _my_frame_res.end())
			r = it->second->anchor_point;
		return r;
	}

	my_animation_cfg  *getAnimationPlayCfg(int animation_id)
	{
		auto it = _my_animation_play_cfg.find(animation_id);
		if (it != _my_animation_play_cfg.end())
			return it->second;
		return nullptr;
	}

	~CharacterResources() {
		freeRes();
	}
	Texture2D*  readTexture(int frame_id)
	{
		auto it = _my_frame_res.find(frame_id);
		//assert(it != my_texture_atlas.end());
		if (it != _my_frame_res.end())
		{
			return it->second->texture;
		}
		return nullptr;
	}
	Texture2D*  readTexture(const std::string& frame_name)
	{
		auto it = mapper_frame.find(frame_name);
		//assert(it != my_texture_atlas.end());
		if (it != mapper_frame.end())
		{
			return readTexture(it->second);
		}
		return nullptr;
	}
	Texture2D* readMapperTexture(int animation_id, int curFrame)
	{
		// 层级安全检查
		if (!_my_animation_play_cfg.empty()) {
			auto anim_it = _my_animation_play_cfg.find(animation_id);
			if (anim_it != _my_animation_play_cfg.end() && anim_it->second)
			{
				my_animation_cfg* cfg = anim_it->second;

				// 帧索引范围检查
				if (!cfg->frameSequenceId.empty() &&
					curFrame >= 0 &&
					curFrame < static_cast<int>(cfg->frameSequenceId.size()))
				{
					int texId = cfg->frameSequenceId[curFrame];
					return readTexture(texId); // 复用已有安全读取方法
				}
				else {
					CCLOGWARN("Invalid frame index %d for animation %d", curFrame, animation_id);
				}
			}
			else {
				CCLOGWARN("Animation config %d not found", animation_id);
			}
		}
		else {
			CCLOGWARN("Animation resource pool is empty");
		}
		return nullptr;
	}

	void addAnimationPlayCfg(int animation_id, my_animation_cfg  *cfg)
	{
		_my_animation_play_cfg[animation_id] = cfg;
	}


	void freeRes() {
		for (auto &i : _my_frame_res)
		{
			if (i.second != nullptr)
			{
				delete i.second;
			}
		}
		_my_frame_res.clear();

		for (auto &i : _my_animation_play_cfg)
		{
			if (i.second != nullptr)
			{
				delete i.second;
			}
		}
		_my_animation_play_cfg.clear();
	}
};

struct  tiles_data {
public:
	int clip_x, clip_y, display_x, display_y, gid;
};

extern  Texture2D *  cropAndCombineSimple(Texture2D * texture,
	const std::vector<tiles_data>& tile_gid_ptr,
	int real_width, int  real_height, int tile_width, int tile_height);
extern  CharacterResources *  JsonParseTexture(const std::string & filePath);//加载纹理与碰撞等
extern bool loadAnimationJson(const std::string & filePath, CharacterResources * curRes);//加载动画配置

#endif