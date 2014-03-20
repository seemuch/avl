AVL
===

How to build
============

1. Prerequisites
	- Mac
		* Setup [MacPorts](http://guide.macports.org)
		* Install [XQuartz](https://xquartz.macosforge.org/)
		* Install dependencies
		```bash
		sudo port install freeglut glew glm glfw
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
