#include <AvlVisualizer.h>
#include <condition_variable>

AvlVisualizer *__avl__vi = NULL;
bool __avl__ready() { return __avl__vi != NULL; }
std::mutex __avl_mtx;
std::condition_variable_any __avl__cv;

void display(int argc, char **argv)
{
	__avl_mtx.lock();
	__avl__vi = new AvlVisualizer(argc, argv);
	__avl__cv.notify_one();
	__avl_mtx.unlock();

	__avl__vi->show();
}

void quicksort(AvlArray<AvlInt> a)
{
	if (a.size() <= 1)
		return;

	int i = -1;
	int j = 0;
	int k = a.size() - 1;

	__avl__vi->start();
	avlSleep(0.5);

	while (j < k) {
		if (a[j] >= a[k]) {
			j = j + 1;
		}
		else {
			a.swap(i + 1, j);
			i = i + 1;
			j++;
		}
	}

	a.swap(i+1, k);

	quicksort(a.subarray(0, i+1));
	quicksort(a.subarray(i+2, k+1));

	__avl__vi->stop();
}

int main(int argc, char *argv[])
{
	std::thread __avl__loop(display, argc, argv);

	__avl_mtx.lock();
	__avl__cv.wait(__avl_mtx, __avl__ready);
	__avl_mtx.unlock();
	avlSleep(0.5);

	AvlArray<AvlInt> a = {5, 51, 2, 42, 7, 3, 6, 8, 10, 3, 11, 5, 9};
	__avl__vi->addObject(&a, "a");
	
	__avl__vi->start();
	avlSleep(0.5);

	quicksort(a);

	__avl__vi->stop();

	__avl__loop.join();
	delete __avl__vi;

	return 0;
}
