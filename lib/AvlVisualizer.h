#ifndef AVL_VISUALIZER_H_
#define AVL_VISUALIZER_H_

#include <GL/freeglut.h>
#include <GL/glut.h>
#include "AvlTypes.h"

const int OpenGL_Major_Version = 2;
const int OpenGL_Minor_Version = 1;
const char *const Title = "Algorithm Visualization";
const int Default_Width = 640;
const int Default_Height = 480;

class AvlVisualizer
{
	public:
		AvlVisualizer(int argc, char *argv[]);
		~AvlVisualizer();

		void show();

	private:
		static void avlResize(int width, int height);

		static void avlDisplay();

		int windowHandle;
		static int currentWidth;
		static int currentHeight;
};

#endif // AVL_VISUALIZER_H_
