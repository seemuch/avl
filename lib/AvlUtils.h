#ifndef AVL_UTILS_H_
#define AVL_UTILS_H_

#include <sys/select.h>
#include <iostream>

inline void avlSleep(float seconds)
{
	timeval tv;

	tv.tv_sec = int(seconds);
	tv.tv_usec = (seconds - int(seconds)) * 1000000;

	select(0, NULL, NULL, NULL, &tv);
}

#endif // AVL_UTILS_H_
