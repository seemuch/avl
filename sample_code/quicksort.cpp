#include <AvlVisualizer.h>
using namespace std;

AvlVisualizer *vi = NULL;

void display(int argc, char **argv)
{
	vi = new AvlVisualizer(argc, argv);

	vi->show();
}

void quicksort(AvlArray<AvlInt> a)
{
	if (a.size() <= 1)
		return;

	int i = -1;
	int j = 0;
	int k = a.size() - 1;

	vi->start();
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

	vi->stop();
}

int main(int argc, char *argv[])
{
	thread loop(display, argc, argv);

	while (vi == NULL)
		avlSleep(0.1);
	avlSleep(0.5);

	AvlArray<AvlInt> a = {5, 51, 2, 42, 7, 3, 6, 8, 10, 3, 11, 5, 9};
	vi->addObject(&a, "a");
	
	vi->start();
	avlSleep(0.5);

	quicksort(a);

	vi->stop();

	loop.join();
	delete vi;

	return 0;
}
