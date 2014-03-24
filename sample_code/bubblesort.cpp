#include <AvlVisualizer.h>
#include <condition_variable>
using namespace std;

AvlVisualizer *vi = NULL;
bool ready() { return vi != NULL; }
mutex mtx;
condition_variable_any cv;

void display(int argc, char **argv)
{
	mtx.lock();
	vi = new AvlVisualizer(argc, argv);
	cv.notify_one();
	mtx.unlock();

	vi->show();
}

int main(int argc, char *argv[])
{
	thread loop(display, argc, argv);

	mtx.lock();
	cv.wait(mtx, ready);
	mtx.unlock();
	avlSleep(0.5);

	AvlArray<AvlInt> a = {5, 51, 2, 42, 7, 3, 6, 8, 10, 3, 11, 5, 9};
	vi->addObject(&a, "a");

	vi->start();
	avlSleep(0.5);

	for (int i = 1; i < a.size(); i = i + 1) {
		int j = i - 1;
		while (j >= 0 && a[j] > a[j + 1]) {
			a.swap(j + 1, j);
			j = j - 1;
		}
	}

	vi->stop();

	loop.join();
	delete vi;

	return 0;
}
