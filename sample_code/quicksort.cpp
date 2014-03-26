/* Headers:
 * insert the following lines at the beginning of each
 * object C++ file */
#include <AvlVisualizer.h>
#include <condition_variable>

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

/* user defined function:
 * NOTE: the parameters should be passed by value */
void quicksort(AvlArray<AvlInt> a)
{
	/* Insert the following two lines at the
	 * beginning of each function */
	a.lowlight();
	a.highlight();

	/* len(a) -> a.size() */
	if (a.size() <= 1)
		return;

	/* Currently, translate type index to int */
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

	a.swap(i+1, k);

	/* translation rule for subarrys */
	quicksort(a.subarray(0, i+1));
	quicksort(a.subarray(i+2, k+1));

	/* substitude <end_display> with the following two lines */
	avlSleep(0.1);
	__avl__vi->stop();

	/* Insert the following line at the end of each function */
	a.lowlight();
}

/* main:
 * NOTE: argc and argv is needed */
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

	/* substitude <end_display> with the following two lines */
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
