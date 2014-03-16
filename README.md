AVL
===

How to build
============

1. Prerequisites
	- Mac
		* Setup [MacPorts](http://guide.macports.org)
		* Intall [XQuartz](https://xquartz.macosforge.org/)
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
	- Mac
	```bash
    	./configure -CXXFLAGS=/opt/local/include
	```
	- Linux
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
and libavl.a to /usr/local/lib.
