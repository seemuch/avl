#include <AvlVisualizer.h>
#include <condition_variable>

AvlVisualizer *__avl__vi = NULL;
bool __avl__ready() { return __avl__vi != NULL; }
std::mutex __avl__mtx;
std::condition_variable_any __avl__cv;

void __avl__display(int argc, char **argv)
{
	__avl__mtx.lock();
	__avl__vi = new AvlVisualizer(argc, argv);
	__avl__cv.notify_one();
	__avl__mtx.unlock();

	__avl__vi->show();
}

int main(int argc, char *argv[])
{
	std::thread __avl__loop(__avl__display, argc, argv);

	__avl__mtx.lock();
	__avl__cv.wait(__avl__mtx, __avl__ready);
	__avl__mtx.unlock();
	avlSleep(0.5);

	AvlArray<AvlInt> a = {5, 51, 2, 42, 7, 3, 6, 8, 10, 3, 11, 5, 9};
	__avl__vi->addObject(&a, "a");

	__avl__vi->start();
	avlSleep(0.5);

	for (int i = 1; i < a.size(); i = i + 1) {
		int j = i - 1;
		while (j >= 0 && a[j] > a[j + 1]) {
			a.swap(j + 1, j);
			j = j - 1;
		}
	}

	__avl__vi->stop();

	__avl__loop.join();
	delete __avl__vi;

	return 0;
}
