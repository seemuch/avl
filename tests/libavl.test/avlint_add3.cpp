#include <iostream>
#include <cstdlib>
#include "AvlTypes.h"
using namespace std;

int main(int argc, char *argv[])
{
	int a = atoi(argv[1]);
	AvlInt b = atoi(argv[2]);

	cout << a + b << endl;

	return 0;
}
