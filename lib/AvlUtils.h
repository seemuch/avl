#ifndef AVL_UTILS_H_
#define AVL_UTILS_H_

#include <thread>
#include <chrono>

inline void avlSleep(float seconds)
{
	int ms = seconds * 1000;
	std::chrono::duration<int, std::milli> milliseconds(ms);

	std::this_thread::sleep_for(milliseconds);
}

#endif // AVL_UTILS_H_
