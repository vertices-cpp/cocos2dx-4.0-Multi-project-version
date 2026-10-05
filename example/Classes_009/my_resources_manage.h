#ifndef _MY_RESOURCES_MANAGE_H_
#define _MY_RESOURCES_MANAGE_H_
#pragma 

#include "cocos2d.h"
#include <unordered_map>
#include <string>

USING_NS_CC;

#include <tchar.h>

#define CREATE_DEBUG_CONSOLE \
 FILE* fpDebugOut = NULL; \
 FILE* fpDebugIn = NULL; \
 FILE* fpDebugErr = NULL; \
 if( !AllocConsole() ) \
 MessageBox(NULL, _T("控制台生成失败。"), NULL, 0); \
 SetConsoleTitle(_T("Debug")); \
 _tfreopen_s(&fpDebugOut, _T("CONOUT$"),_T("w"), stdout); \
 _tfreopen_s(&fpDebugIn, _T("CONIN$"), _T("r"), stdin); \
 _tfreopen_s(&fpDebugErr, _T("CONERR$"),_T("w"), stderr); \
 _tsetlocale(LC_ALL, _T("chs"))

#define RELEASE_DEBUG_CONSOLE \
 fclose(fpDebugOut); \
 fclose(fpDebugIn); \
 fclose(fpDebugErr); \
 FreeConsole()

#include <string>

#define TOSTRING(vec2) \
std::to_string((int)vec2.x)+ "/"+std::to_string((int)vec2.y)


extern bool rect_equal(const Rect & lhs, const Rect &rhs);
extern Rect vec2_rect_plus(Vec2 &v, Rect &r);
extern bool GameIntersectsRect(const Rect & lhs, const Rect& rhs);
#define SET_DIRTY_RECURSIVELY() {                       \
                    if (! _recursiveDirty) {            \
                        _recursiveDirty = true;         \
                        setDirty(true);                 \
                        if (!_children.empty())         \
                            setDirtyRecursively(true);  \
                        }                               \
                    }
 
extern std::ostream& operator<<(std::ostream& os, const Rect & rhs);



class GameCharacter;


 
#include "loadJsonFile.h"

class my_resources_manage {//资源总类
	 
	std::unordered_map<unsigned, CharacterResources*> _my_character_res;//表示是角色ID
	 
public:
	static my_resources_manage* getInstance();
	bool init();
	my_resources_manage();
	~my_resources_manage();
	void freeRes();
	bool addJsonTexture(const std::string &tmjPath);
	bool addJsonAnimationJson(const std::string &jsonPath);

	
	CharacterResources* getCharacterResources(int STATE_id) {
		auto it = _my_character_res.find(STATE_id);
		return it != _my_character_res.end() ? it->second : nullptr;
	}

	

	CharacterResources* getCharacterResources(const std::string& character_name) {
		 
		std::string dst = StripPathRemoveExtension(character_name);
		 
		auto it = global_my_character_mapper.find(dst);
		return it != global_my_character_mapper.end() ? getCharacterResources(it->second) : nullptr;
	}
	CharacterResources* get_char_data(const std::string& character_name) {
		auto it = std::find_if(_my_character_res.begin(), _my_character_res.end(), [character_name](const std::pair<const unsigned, CharacterResources*>& item) {
			return item.second->_character_name == character_name;
		});
		return it != _my_character_res.end() ? it->second : nullptr;
	} 

// 	Texture2D* readTexture(int STATE_id,const std::string& textureName)
// 	{
// 		auto it = GframeNameToId.find(STATE_id);
// 		 
// 		return (it != GframeNameToId.end()) ? _my_character_res[STATE_id]->readTexture(it->second[textureName]) : nullptr;
// 	}
	Texture2D * QuaryTexture(int STATE_id, const std::string& frameName)
	{
		auto ret = getCharacterResources(STATE_id);
		return ret != nullptr ? ret->readTexture(frameName):nullptr;
	}
// 	my_animation_cfg  *getAnimationPlayCfg(int STATE_id, int animation_id)
// 	{
// 		auto it = _my_character_res.find(STATE_id);
// 		return (it != _my_character_res.end()) ? _my_character_res[STATE_id]->getAnimationPlayCfg(animation_id) : nullptr;
// 	}
};


#endif