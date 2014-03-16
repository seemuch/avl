#ifndef AVL_VISUALIZER_H_
#define AVL_VISUALIZER_H_

#include <GL/freeglut.h>
#include <GL/glut.h>

#define OpenGL_Major_Version 2
#define OpenGL_Minor_Version 1
#define Title "Algorithm Visualization"
#define Default_Width 640
#define Default_Height 480

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
