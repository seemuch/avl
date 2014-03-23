AVL
===
Please see [Issues](https://github.com/wqfish/avl/issues) for bugs and new tasks.

How to build
============

1. Prerequisites
	- Mac
		* Setup [MacPorts](http://guide.macports.org)
		* Install [XQuartz](https://xquartz.macosforge.org/)
		* Install dependencies
		```bash
		sudo port install automake libtool freeglut glew glm glfw
		```
	- Linux
		* Only need to install libraries
		```bash
		sudo apt-get install **
		```

2. Create configure script.
	```bash
	autoreconf --install
	```

3. Create Makefiles.
	```bash
	./configure
	```

4. Make.
	```bash
	make
	```

5. Install
	```bash
	sudo make install
	```
This would install avl to /usr/local/bin, header files to /usr/local/include
and libraries to /usr/local/lib.

How to run test cases
=====================

```bash
make check
```
You can run ```make check -j [N]``` if you have multiple CPUs.

How to add a test case
======================

Suppose you want to test ```if``` control flow.

1. Create ```if1.avl``` in ```tests/avl.test/```, suppose you have multiple test cases for ```if```.

2. Write a bash script ```if1.sh```, also put it into ```tests/avl.test/```:
	```bash
	#!/bin/bash
	
	avl -o if1 if1.avl
	```
Don't forget ```chmod +x if1.sh```.

3. Modify ```tests/avl.test/Makefile.am```, add ```if1.sh``` to ```TESTS``` variable.
If the test case is expected to fail, also add the script to ```XFAIL_TESTS``` variable.
Whether the test case passes or fails depends on the exit code of the script (0 for success).

4. Rerun ```make check```.
