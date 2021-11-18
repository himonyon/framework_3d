#include "../../../framework.h"
#include "../../../environment.h"

float Timer::frameTime = 0;
float Timer::beforeTime = 0;

void Timer::FrameTimeExecute() {
	frameTime = (float)clock() - beforeTime;
	beforeTime = (float)clock();
}

void Timer::Start() {
	time = 0;
	flag = true;
}

void Timer::Stop() { flag = false; }

void Timer::Resume() { flag = true; }

void Timer::Execute() { time += frameTime; }