#include <AvlVisualizer.h>
#include <AvlTypes.h>
#include <condition_variable>
#include <cstdlib>

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

void merge(AvlArray<AvlInt> a, AvlArray<AvlInt> b, AvlArray<AvlInt> c)
{
	AvlIndex i = 0;
	AvlIndex j = 0;
	AvlIndex k = 0;

	while (true) {
		if (i >= a.size() || j >= b.size())
			break;

		c[k] = a[i] < b[j] ? a[i++] : b[j++];
		k++;
	}

	for (; i < a.size(); i++) {
		c[k] = a[i];
		k++;
	}
	for (; j < b.size(); j++) {
		c[k] = b[j];
		k++;
	}
}

int main(int argc, char *argv[])
{
	std::thread __avl__loop(__avl__display, argc, argv);
	__avl__mtx.lock();
	__avl__cv.wait(__avl__mtx, __avl__ready);
	__avl__mtx.unlock();
	avlSleep(0.5);

	__avl__vi->start();
	avlSleep(0.5);

	AvlArray<AvlInt> a = {1, 5, 7, 9, 20};
	a.set_name("a");
	__avl__vi->addObject(&a, "a");
	AvlArray<AvlInt> b = {2, 4, 8, 16};
	b.set_name("b");
	__avl__vi->addObject(&b, "b");
	AvlArray<AvlInt> c(9);
	c.set_name("c");
	__avl__vi->addObject(&c, "c");

	merge(a, b, c);

	avlSleep(0.1);
	__avl__vi->stop();

	std::cout << c << std::endl;

	__avl__vi->delObject("a");
	__avl__vi->delObject("b");
	__avl__vi->delObject("c");

	__avl__loop.join();
	delete __avl__vi;

	return 0;
}
