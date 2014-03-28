#ifndef AVL_UTILS_H_
#define AVL_UTILS_H_

#include <ctime>
#include <cerrno>

inline void die(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

inline void avlSleep(float seconds)
{
	timespec req, rem;

	req.tv_sec = int(seconds);
	req.tv_nsec = (seconds - int(seconds)) * 1000000000;

	while (nanosleep(&req, &rem) != 0) {
		if (errno != EINTR)
			die("nanosleep() failed.");

		req.tv_sec = rem.tv_sec;
		req.tv_nsec = rem.tv_nsec;
	}
}

#endif // AVL_UTILS_H_
