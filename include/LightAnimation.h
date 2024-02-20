#pragma once
#include <vector>
#include "LightFrame.h"
#include <future>
#include <functional>
#include <chrono>
#include "LightState.h"
class LightAnimation : public std::vector<LightFrame>
{
public:
	LightAnimation();
	void operator()(LightState& launchpadLights);

private:
	std::function<void(std::reference_wrapper<LightState>)> func = nullptr;

	template <typename... ParamTypes>
	void setTimeOut(int32_t milliseconds, std::function<void(ParamTypes...)> func, ParamTypes... parames);

};

template<typename ...ParamTypes>
inline void LightAnimation::setTimeOut(int32_t milliseconds, std::function<void(ParamTypes...)> func, ParamTypes ...parames) {
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
		try {
			func(parames...);
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		}).detach();
}