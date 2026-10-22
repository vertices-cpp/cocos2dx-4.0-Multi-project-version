#ifndef _MY_GAME_LIBARAY_
#define _MY_GAME_LIBARAY_

#include "cocos2d.h"

USING_NS_CC;

class Game_Animation :public Animation {

	static Game_Animation*  create()
	{
		Game_Animation *game_animation = new (std::nothrow) Game_Animation();
		game_animation->init();
		game_animation->autorelease();

		return game_animation;
	}
	bool init()
	{
		_loops = 1;
		_delayPerUnit = 0.0f;

		return true;
	}

	void addSpriteFrame(SpriteFrame* spriteFrame, float delay = 1.0f)
	{
		AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, 1.0f, ValueMap());
		_frames.pushBack(animFrame);

		// update duration
		_totalDelayUnits++;

		if (!_delayPerUnit)
			_delayPerUnit = delay;
	}

	void  addFrame(const std::string &filename, const Rect &rect, float delay = 0.2f)
	{
		AnimationFrame *animFrame = AnimationFrame::create(SpriteFrame::create(filename, rect), 1.0f, ValueMap());
		_frames.pushBack(animFrame);

		// update duration
		_totalDelayUnits++;

		if (!_delayPerUnit)
			_delayPerUnit = delay;
	}

	void  addFrame(const std::string &filename, float delay = 0.2f)
	{
		addFrame(filename, Rect::ZERO, delay);
	}
	void  addAnimationFrame(AnimationFrame* animFrame)
	{
		//	AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, 1.0f, ValueMap());
		assert(animFrame != nullptr);

		_frames.pushBack(animFrame);

		// update duration
		_totalDelayUnits++;
	}
};

bool GameIntersectsRect(const Rect & lhs,const Rect& rhs)   {
	return !(lhs.getMaxX() <= rhs.getMinX() ||
		rhs.getMaxX() <= lhs.getMinX() ||
		lhs.getMaxY() <= rhs.getMinY() ||
		rhs.getMaxY() <= lhs.getMinY());
}


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


#endif