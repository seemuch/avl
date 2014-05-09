/* Headers:
 * insert the following lines at the beginning of
 * each target C++ file */
#include <AvlVisualizer.h>
#include <AvlTypes.h>
#include <condition_variable>
#include <cstdlib>

/* Note: these variable names are also reserved words,
 * so we add a prefix __avl__. */
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

	/* Translate type index to AvlIndex,
	 * index should be non-negative */
	AvlIndex i = 0;
	i.set_name("i");
	AvlIndex j = 0;
	j.set_name("j");
	AvlIndex k = a.size() - 1;
	k.set_name("k");


	/* substitude <begin_display> with the following two lines */
	__avl__vi->start();
	avlSleep(0.5);

	while (j < k) {
		if (a[j] >= a[k]) {
			j = j + 1;
		}
		else {
			/* swap(a, i, j) -> a.swap(i, j) */
			a.swap(i, j);
			i = i + 1;
			j++;
		}
	}

	/* same as above */
	a.swap(i, k);

	/* translation rule for subarrys,
	 * TODO: operator precedence? ':' and '+-' */
	quicksort(a.subarray(0, i));
	quicksort(a.subarray((i+1), (k+1)));

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

	/* !!FIXME!!
	 * To Shining Sun:
	 * we cannot translate "index i = 0" to
	 * "AvlIndex i = 0; i.set_name("i")" here. */
	for (AvlIndex i = 0; i < a.size(); i++) {
		/* Should rand be a keyword or a function?
		 * If function, consider inline C (like inline
		 * assembly in C) */
		AvlIndex j = rand() % a.size();
		j.set_name("j");
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
	__avl__mtx.lock();
	__avl__cv.wait(__avl__mtx, __avl__ready);
	__avl__mtx.unlock();
	avlSleep(0.5);

	/* substitude array declaration with the following two lines */
	AvlArray<AvlInt> a = {5, 51, 2, 42, 7, 3, 6, 8, 10, 3, 11, 5, 9};

	a.set_name("a");
	/* If the variable should be displayed, the following line is needed */
	__avl__vi->addObject(&a, "a");

	/* translation rule for print */
	std::cout << "before sorting: " << a << std::endl;

	/* 1. Insertion Sort */

	/* substitude <begin_display> with the following two lines */
	__avl__vi->start();
	avlSleep(0.5);

	/* !!FIXME!!
	 * same issue as in randomPermute */
	for (AvlIndex i = 1; i < a.size(); i++) {
		i.set_name("i");
		__avl__vi->addObject(&i, "i");
		AvlIndex j = i - 1;
		j.set_name("j");
		__avl__vi->addObject(&j, "j");
		while (j >= 0 && a[j] > a[j+1]) {
			a.swap(j + 1, j);
			j = j - 1;
		}
	}
	
	std::cout << "after insertion sort: " << a << std::endl;

	/* 2. random permutation */

	/* This function won't be displayed, although it is
	 * put between <begin_display> and <end_display> */
	randomPermute(a);

	std::cout << "after permutation: " << a << std::endl;

	/* 3. quicksort */

	/* substitude <begin_display> with the following two lines */
	__avl__vi->start();
	avlSleep(0.5);


	quicksort(a);

	std::cout << "after quicksort: " << a << std::endl;

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
