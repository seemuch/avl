#include "AvlTypes.h"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	AvlInt a = atoi(argv[1]);
	AvlInt b = atoi(argv[2]);

	cout << a + b << endl;

	return 0;
}
