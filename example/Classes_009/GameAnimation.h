#ifndef _GAME_ANIMATION_H_ // 修正拼写错误
#define _GAME_ANIMATION_H_

#include "cocos2d.h"
#include "my_resources_manage.h"

USING_NS_CC;

#include <vector>
#include <functional>
#include <unordered_map>
#include <iostream>

class GameCharacter;

class GameAnimationPlay {
private:
	bool _is_running; // 使用 bool 类型
	int _delay;
	int _curFrame;

	GameCharacter* _character;

	my_animation_cfg *_cfg;

public:
	GameAnimationPlay()
		: _is_running(false), _delay(0), _curFrame(0),
		_character(nullptr), _cfg(nullptr) 
		 {}

	~GameAnimationPlay() = default;

	// 禁用拷贝构造和赋值
	GameAnimationPlay(const GameAnimationPlay&) = delete;
	GameAnimationPlay& operator=(const GameAnimationPlay&) = delete;

	void initialize(GameCharacter* character, my_animation_cfg *cfg) {
		_is_running = true;
		_delay = 0;
		_curFrame = 0;

		if (cfg == nullptr)
		{
			return;
		}

		

		_character = character;
		_cfg = cfg;

		// 触发第 0 帧事件
		notifyObservers();
	}

	void notifyObservers() {
	    if (_cfg->frameEvents.empty())
	    {
			return;
	    }
		auto it = _cfg->frameEvents.find(_curFrame);
		if (it != _cfg->frameEvents.end()) {
			it->second(_character);
		}
	}

	bool isRun() const {
		return _is_running;
	}

	int getcurFrameTextueId() const {
 
		return _cfg->frameSequenceId[_curFrame];
	}

	friend std::ostream& operator<<(std::ostream& os, const GameAnimationPlay& rhs) {
		os << "(_is_running:" << rhs._is_running << " delay:" << rhs._delay << " curFrame:" << rhs._curFrame << " ";
		if (rhs._cfg->frameDelays.size()) {
			for (auto& i : rhs._cfg->frameDelays) {
				os << i << " ";
			}
		}
		else {
			os << "frameDelay:null ";
		}
		return os << "totalFrame:" << rhs._cfg->totalframe << ")";
	}

	void update() { // 接受 deltaTime 参数
	 

		_delay++;
		int curDelay = _cfg->frameDelays[_curFrame];
		if (_delay >= curDelay) {
			_delay = 0;
			_curFrame++;

			if (_curFrame >= _cfg->totalframe) {
				if (_cfg->frameLoop) //如果是循环则初始化
				{
					initialize(_character, _cfg);
					return;
				}
				_is_running = false;
			}
			else {
				notifyObservers(); // 触发新帧事件
			}
		}
	}

	float getTotalAnimationDelay() const {
	 
		float ret = 0;
		for (auto& i : _cfg->frameDelays) {
			ret += i;
		}
		return ret;
	}

	int get_curFrame() const {
		return _curFrame;
	}
};

#endif