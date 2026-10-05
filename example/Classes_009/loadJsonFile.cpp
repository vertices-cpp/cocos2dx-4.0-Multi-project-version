#include "loadJsonFile.h"

#include "json.hpp"
#include "GameAnimationType.h"
#include "GameCharacter.h"


int global_characterID = 0; //表示是角色ID计数器
int global_textureCountID = 0;
std::unordered_map<std::string, unsigned> global_my_character_mapper;

std::unordered_map<std::string, int> mapper_function_id = {
	{"attackEvent",1}, 
};

std::unordered_map<int, std::function<void(GameCharacter*)>> mapper_function = {
	{ 1,[](GameCharacter *character) {
		character->addItem(ITEM | 1, 1, 1, 30);
}  },
};

std::string readFile(const std::string & path)
{
	std::fstream in(path, std::ios::in | std::ios::binary);
	if (!in)
	{
		return false;
	}
	in.seekg(0, std::ios::end);
	int len = (int)in.tellg();
	in.seekg(0, std::ios::beg);
	char *data = new char[len + 1];
	in.read(data, len);
	in.close();

	std::string jsonData(data, len);
	delete[]data;
	return jsonData;
}


Texture2D *  cropAndCombineSimple(Texture2D * texture,
	const std::vector<tiles_data>& tile_gid_ptr,
	int real_width, int  real_height, int tile_width, int tile_height)
{

	Size textureSize = texture->getContentSize();

	// 计算组合后纹理的大小
	int combinedWidth = real_width * tile_width;
	int combinedHeight = real_height * tile_height;

	// 创建一个空白的渲染目标
	auto renderTexture = RenderTexture::create(combinedWidth, combinedHeight);
	if (!renderTexture) {
		CCLOG("Failed to create render texture!");
		return false;
	}

	renderTexture->begin();

	for (auto &i : tile_gid_ptr)
	{
		Rect clip_rect(i.clip_x, i.clip_y, tile_width, tile_height);
		Vec2 display_xy(i.display_x * tile_width + tile_width / 2,
			(real_height - i.display_y - 1) * tile_height + tile_height / 2);

		auto sprite = Sprite::createWithTexture(texture, clip_rect);
		if (!sprite) {
			CCLOG("Failed to create sprite at (%d, %d)!", i.clip_x, i.clip_y);
			continue;
		}
		sprite->setPosition(display_xy);

		sprite->visit();
	}


	renderTexture->end();


	auto spriteFromRenderTexture = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	if (!spriteFromRenderTexture) {
		CCLOG("Failed to create sprite from render texture!");
		return nullptr;
	}

	// 进行垂直翻转操作
	//spriteFromRenderTexture->setFlippedY(false);
	spriteFromRenderTexture->setPosition(Vec2(combinedWidth / 2, combinedHeight / 2));
	// 创建一个新的 RenderTexture 用于渲染翻转后的 Sprite
	auto finalRenderTexture = RenderTexture::create(combinedWidth, combinedHeight);
	if (!finalRenderTexture) {
		CCLOG("Failed to create final render texture!");
		return nullptr;
	}

	finalRenderTexture->begin();
	spriteFromRenderTexture->visit();
	finalRenderTexture->end();

	// 获取最终合并并翻转后的纹理
	auto combinedTexture = finalRenderTexture->getSprite()->getTexture();
	combinedTexture->retain();

	return combinedTexture;
}


CharacterResources *  JsonParseTexture(const std::string & filePath)
{
	std::streampos filePathPrefixPos = filePath.find_last_of('/') + 1;
	if (filePathPrefixPos == std::string::npos)
	{
		filePathPrefixPos = filePath.size();
	}
	std::string filePathPrefix = filePath.substr(0, filePathPrefixPos);

	std::string jsonData = readFile(filePath);
	 

	nlohmann::json json_data = nlohmann::json::parse(jsonData);


	int Map_width = json_data["width"];
	int Map_height = json_data["height"];

	int tile_width = json_data["tilewidth"];
	int tile_height = json_data["tileheight"];

	std::string image_path;

	int image_width = 0, image_height = 0;

	for (auto &i : json_data["tilesets"])
	{
		image_width = i["imagewidth"];
		image_height = i["imageheight"];
		std::string  image = i["image"];
		if (image.find_last_of('/') == std::string::npos)
		{
			std::string dir = filePath.substr(0, filePath.find_last_of('/') + 1);
			image = dir + image;
		}
		image_path = image;
	}

	auto texture = Director::getInstance()->getTextureCache()->addImage(image_path);
	if (!texture) {
		CCLOG("Failed to load texture!");
		return nullptr;
	}


	CharacterResources * c_res = new CharacterResources;


	//获取图像大小
	auto texture_size = texture->getContentSize();
	auto max_x = (int)texture_size.width / tile_width;
	//	auto max_y = (int)texture_size.height / tile_height;



	for (auto &json_layer_data : json_data["layers"])
	{
		std::string TypeName = json_layer_data["type"];


		if (TypeName != "tilelayer")
		{
			continue;
		}
		std::string json_layer_name = json_layer_data["name"];

		global_textureCountID++;


		float anchorPoint_x = 0.5, anchorPoint_y = 0.5;


		auto find_properties = json_layer_data.find("properties");
		if (find_properties != json_layer_data.end())
		{


			for (auto& item : json_layer_data["properties"])

			{
				if (*item.find("name") == "anchorPointX")
				{
					anchorPoint_x = item["value"];
				}
				if (*item.find("name") == "anchorPointY")
				{
					anchorPoint_y = item["value"];
				}

			}
		}

		//获取当前的数组数量
		int Layer_width = json_layer_data["width"], Layer_height = json_layer_data["height"];
		//创建对象来获取数组
		int len = Layer_width * Layer_height;
		int * arr = new int[len];
		std::vector<tiles_data > tile_gid_ptr;

		for (int i = 0; i < Layer_height; ++i)
		{
			for (int j = 0; j < Layer_width; ++j)
			{
				int value = json_layer_data["data"][i * Layer_width + j];
				arr[i * Layer_width + j] = value;
				if (arr[i * Layer_width + j])
				{
					tiles_data mtd;

					mtd.display_x = j;
					mtd.display_y = i;
					mtd.gid = arr[i * Layer_width + j] - 1;

					//都是用矩形剪裁
					mtd.clip_x = mtd.gid  % max_x * tile_width;
					//保留tileHeight
					mtd.clip_y = mtd.gid / max_x * tile_height;

					tile_gid_ptr.push_back(mtd);
				}
			}
		}



		int picture_x = 0, picture_y = 0, picture_width = 0, picture_height = 0;
		//遍历一遍得到实际有效的尺寸x位置
		for (int col = 0; col < Layer_width; ++col)
		{
			for (int row = 0; row < Layer_height; ++row)
			{
				if (arr[row * Layer_width + col])
				{
					picture_x = col;

					col = Layer_width;
					row = Layer_height;
				}
			}
		}
		//遍历一遍得到实际有效的边界y位置
		for (int col = Layer_width - 1; col >= 0; --col)
		{
			for (int row = 0; row < Layer_height; ++row)
			{
				if (arr[row * Layer_width + col])
				{
					picture_width = col + 1;

					col = -1;
					row = Layer_height;
				}
			}
		}

		//遍历一遍得到实际有效的边界w位置
		for (int row = 0; row < Layer_height; ++row)
		{
			for (int col = 0; col < Layer_width; ++col)
			{
				if (arr[row * Layer_width + col])
				{
					picture_y = row;

					col = Layer_width;
					row = Layer_height;
				}
			}
		}
		//遍历一遍得到实际有效的边界H位置
		for (int row = Layer_height - 1; row >= 0; --row)
		{
			for (int col = 0; col < Layer_width; ++col)
			{
				if (arr[row * Layer_width + col])
				{
					picture_height = row + 1;

					col = Layer_width;
					row = -1;
				}
			}
		}

		if (arr != nullptr)
		{
			delete[] arr;
		}
		//获取实际尺寸
		int real_width = picture_width - picture_x, real_height = picture_height - picture_y;


		Texture2D* tex = cropAndCombineSimple(texture, tile_gid_ptr, real_width, real_height, tile_width, tile_height);
		std::string tex_name = json_layer_data["name"];


		c_res->addTextureMeta(global_textureCountID);

		c_res->set_frameID_mapper(json_layer_name, global_textureCountID);//添加帧ID映射
		c_res->addIdAnchorPoint(global_textureCountID, Vec2(anchorPoint_x, anchorPoint_y));
		c_res->addIdTexture2d(global_textureCountID, tex);


	}

	//	std::unordered_map < std::string, Rect> objectgroup;
	//先获取对象层的比如碰撞与攻击
	for (auto &json_layer_data : json_data["layers"])
	{
		std::string TypeName = json_layer_data["type"];
		std::string json_layer_name = json_layer_data["name"];

		if (TypeName != "objectgroup")
		{
			continue;
		}
		auto objects = json_layer_data["objects"];

		std::vector<Rect>  collision;
		std::vector<Rect>    attack;
		int add_flag = 0;//添加标记
		for (auto objects_item : objects)
		{

			if (objects_item["name"] == "collision")
			{
				float  rect_x = objects_item["x"];
				float  rect_y = objects_item["y"];
				float rect_width = objects_item["width"];
				float rect_height = objects_item["height"];
				Rect rect = { rect_x,rect_y,rect_width,rect_height };
				collision.push_back(rect);//添加碰撞盒
				add_flag |= 1;
			}
			else if (objects_item["name"] == "attack")
			{
				float  rect_x = objects_item["x"];
				float  rect_y = objects_item["y"];
				float rect_width = objects_item["width"];
				float rect_height = objects_item["height"];
				Rect rect = { rect_x,rect_y,rect_width,rect_height };
				attack.push_back(rect);
				add_flag |= 2;
			}
		}

		// 		auto it = GframeNameToId.find(_characterID);
		// 		if (it == GframeNameToId.end())
		// 			continue;
		auto sub_frame_id = c_res->get_mapper_frame_id(json_layer_name);

		if (sub_frame_id == -1)
			continue;

		if ((add_flag & 1) == 1)
			c_res->addIdCollisionAtlas(sub_frame_id, collision);

		if ((add_flag & 2) == 2)
			c_res->addIdAttackAtlas(sub_frame_id, attack);


	}
	std::streampos extPos = filePath.find_last_of('.');
	if (extPos == std::string::npos)
	{
		extPos = filePath.size();
	}
	std::string char_name = filePath.substr(filePathPrefixPos, extPos - filePathPrefixPos);


	c_res->_character_name = char_name;
	c_res->_characterID = ++global_characterID;
	global_my_character_mapper[char_name] = global_characterID;
	return c_res;
}

std::string StripPathRemoveExtension(const std::string &path)
{
	std::streampos prePos = path.find_last_of('/') + 1;

	std::string dst = path;
	if (prePos != std::string::npos)
	{
		dst = dst.substr(prePos);
	}
	std::streampos sufPos = dst.find_last_of('.');
	if (sufPos != std::string::npos)
	{
		dst = dst.substr(0, dst.size() - (dst.size() - sufPos));
	}
	return dst;
}



bool  loadAnimationJson(const std::string &filePath, CharacterResources *curRes)
{
	
	std::string jsonData = readFile(filePath);
	 
	if (jsonData.empty())
	{
		return false;
	}
//	CharacterResources *curRes = _my_character_res[characterID];

	nlohmann::json json_data = nlohmann::json::parse(jsonData);

	for (auto &i : json_data["animation_cfg"])
	{
		auto ani_type_find = i.find("animation");
		if (ani_type_find == i.end())
			continue;

		std::string ani_Name = i["animation"];
		auto it = animationStringToEnum.find(ani_Name);
		if (it == animationStringToEnum.end())
		{
			continue;
		}

		auto actionID = it->second;

		curRes->set_actionID_mapper(ani_Name, actionID);
		//检查是否循环-----------
		bool loop = false;
		auto loop_find = i.find("loop");
		if (loop_find != i.end()) {
			loop = loop_find->get<bool>();
		}
		curRes->set_action_loop(actionID,loop);
		//----------------------
		for (auto &j : i["frames"])
		{
			std::string  step_frame = j["frame"];

			int delay = j["delay"];

			auto frame__ID = curRes->get_mapper_frame_id(step_frame);
			if (frame__ID == -1)continue;


			curRes->addAnimationFrames(actionID, frame__ID, delay);

		}


		for (auto &k : i["events"])
		{
			int trigger_frame = k["trigger_frame"];
			std::string eventName = k["event"];
			auto find_event_id = mapper_function_id.find(eventName);//查找事件ID
			if (find_event_id == mapper_function_id.end())
			{
				continue;
			}
			auto event_function = mapper_function[find_event_id->second];
			curRes->addEvent(actionID, trigger_frame, event_function);
		}
	}
	return true;
}