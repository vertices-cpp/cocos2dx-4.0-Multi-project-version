#include "my_resources_manage.h"
#include "GameCharacter.h"
#include "loadJsonFile.h"
 
 bool rect_equal(const Rect & lhs, const Rect &rhs)
{
	return lhs.origin.x == rhs.origin.x &&
		lhs.origin.y == rhs.origin.y &&
		lhs.size.width == rhs.size.width &&
		lhs.size.height == rhs.size.height;
}
 bool GameIntersectsRect(const Rect & lhs, const Rect& rhs) {
	return !(lhs.getMaxX() <= rhs.getMinX() ||
		rhs.getMaxX() <= lhs.getMinX() ||
		lhs.getMaxY() <= rhs.getMinY() ||
		rhs.getMaxY() <= lhs.getMinY());
}

 Rect vec2_rect_plus(Vec2 &v, Rect &r) {
	 return Rect(v.x + r.origin.x, v.y + r.origin.y, r.size.width, r.size.height);
  }

 std::ostream& operator<<(std::ostream& os, const Rect & rhs)
 {
	 return os << "(x:" << rhs.origin.x << " y:" << rhs.origin.y << " width:" << rhs.size.width << " height:" << rhs.size.height << ")";
 }

  


 my_resources_manage * my_resources_manage::getInstance()
 {
	 static my_resources_manage _instance;
	 return &_instance; 
 }

 bool my_resources_manage::init()
{
	return true;
}

my_resources_manage::my_resources_manage()
{
}
my_resources_manage::~my_resources_manage()
{
	freeRes();
}
void my_resources_manage::freeRes() {
	for (auto &i : _my_character_res)
	{
		if (i.second != nullptr)
		{
			delete i.second;
		}
	}
	_my_character_res.clear();
}


bool my_resources_manage::addJsonTexture(const std::string & tmjPath)
{
	std::string characterName = StripPathRemoveExtension(tmjPath);

	auto find_id = global_my_character_mapper.find(characterName);
	if (find_id != global_my_character_mapper.end())
	{
		return false;
	}
	std::string filePath = FileUtils::getInstance()->fullPathForFilename(tmjPath);
	 
	auto  ret = JsonParseTexture(filePath);

	if (ret != nullptr)
	{ 
		_my_character_res[global_characterID] = ret;
		return true;
	}
	return false;
}
 

bool my_resources_manage::addJsonAnimationJson(const std::string &jsonPath)
{
	std::string filePath = FileUtils::getInstance()->fullPathForFilename(jsonPath);

	std::string characterName = StripPathRemoveExtension(filePath);

	auto find_id = global_my_character_mapper.find(characterName);
	if (find_id == global_my_character_mapper.end())
	{
		return false;
	}
	auto characterID = find_id->second;
	CharacterResources *curRes = _my_character_res[characterID];


	return loadAnimationJson(filePath,curRes);
}

// Texture2D* my_resources_manage::readTexture(const std::string& textureName)
// {
// 	return (GframeNameToId.find(textureName) != GframeNameToId.end()) ? readTexture(GframeNameToId[textureName]): nullptr;
// }



 


 