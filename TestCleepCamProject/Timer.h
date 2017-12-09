#pragma once
#include <time.h>
class Timer
{
private:
	unsigned long beginTime;
public:
	void start() {
		beginTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long)clock() - beginTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}
	Timer();
	~Timer();
};

