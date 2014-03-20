#include <iostream>
#include <cstdlib>
#include "AvlTypes.h"
using namespace std;

int main(int argc, char *argv[])
{
	int num = atoi(argv[1]);

	AvlInt a = num;

	cout << a << endl;

	return 0;
}
