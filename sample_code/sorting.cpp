/* Headers:
 * insert the following lines at the beginning of
 * each target C++ file */
#include <AvlVisualizer.h>
#include <AvlTypes.h>
#include <condition_variable>
#include <cstdlib>

AvlVisualizer *__avl__vi = NULL;
bool __avl__ready() { return __avl__vi != NULL; }
std::mutex __avl_mtx;
std::condition_variable_any __avl__cv;

void __avl__display(int argc, char **argv)
{
	__avl_mtx.lock();
	__avl__vi = new AvlVisualizer(argc, argv);
	__avl__cv.notify_one();
	__avl_mtx.unlock();

	__avl__vi->show();
}
/* End of Headers */

/* User defined function:
 * NOTE: the parameters should be passed by value */
void quicksort(AvlArray<AvlInt> a)
{
	/* Insert the following three lines at the
	 * beginning of each function except main() */
	__avl__vi->reset();
	a.lowlight();
	a.highlight();

	/* len(a) -> a.size() */
	if (a.size() <= 1) {
		/* NOTE: 'return' is equivalent to end of function
		 * so the following two lines are needed. Additional
		 * '{' and '}' are also required.
		 * !!!FIXME!!!: Is a return between <begin_display> and
		 * <end_display> allowed? Seems no problem. */
		a.lowlight();
		__avl__vi->restore();
		return;
	}

	/* Currently, just translate type index to int */
	int i = -1;
	int j = 0;
	int k = a.size() - 1;

	/* substitude <begin_display> with the following two lines */
	__avl__vi->start();
	avlSleep(0.5);

	while (j < k) {
		if (a[j] >= a[k]) {
			j = j + 1;
		}
		else {
			/* swap(a, i, j) -> a.swap(i, j) */
			a.swap(i + 1, j);
			i = i + 1;
			j++;
		}
	}

	/* same as above */
	a.swap(i+1, k);

	/* translation rule for subarrys */
	quicksort(a.subarray(0, (i+1)));
	quicksort(a.subarray((i+2), (k+1)));

	/* substitude <end_display> with the following two lines */
	avlSleep(0.1);
	__avl__vi->stop();

	/* Insert the following two lines at the end of each function */
	a.lowlight();
	__avl__vi->restore();
}

/* This is an example of user defined function
 * which does not contain <begin_display> and
 * <end_display>, it should not be displayed */
void randomPermute(AvlArray<AvlInt> a)
{
	/* Insert the following three lines at the
	 * beginning of each function except main */
	__avl__vi->reset();
	a.lowlight();
	a.highlight();

	for (int i = 0; i < a.size(); i++) {
		/* Should rand be a keyword or a function?
		 * If function, consider inline C (like inline
		 * assembly in C) */
		int j = rand() % a.size();
		a.swap(i, j);
	}

	/* Insert the following two lines at the end of each function */
	a.lowlight();
	__avl__vi->restore();
}

/* main:
 * NOTE: argc and argv are required */
int main(int argc, char *argv[])
{
	/* only for main:
	 * insert the following five lines at the beginning of main */
	std::thread __avl__loop(__avl__display, argc, argv);
	__avl_mtx.lock();
	__avl__cv.wait(__avl_mtx, __avl__ready);
	__avl_mtx.unlock();
	avlSleep(0.5);

	/* substitude array declaration with the following two lines */
	AvlArray<AvlInt> a = {5, 51, 2, 42, 7, 3, 6, 8, 10, 3, 11, 5, 9};
	__avl__vi->addObject(&a, "a");

	/* 1. bubblesort */

	/* substitude <begin_display> with the following two lines */
	__avl__vi->start();
	avlSleep(0.5);

	for (int i = 1; i < a.size(); i++) {
		int j = i - 1;
		while (j >= 0 && a[j] > a[j+1]) {
			a.swap(j + 1, j);
			j = j - 1;
		}
	}

	/* 2. random permutation */

	/* This function won't be displayed, although it is
	 * put between <begin_display> and <end_display> */
	randomPermute(a);

	/* 3. quicksort */

	/* substitude <begin_display> with the following two lines */
	__avl__vi->start();
	avlSleep(0.5);

	quicksort(a);

	/* substitude <end_display> with the following two lines */
	avlSleep(0.1);
	__avl__vi->stop();

	/* only for main:
	 * insert the following two lines at the end of main
	 * BEFORE return 0 */
	__avl__loop.join();
	delete __avl__vi;

	return 0;
}
