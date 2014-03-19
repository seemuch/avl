#ifndef AVL_VISUALIZER_H_
#define AVL_VISUALIZER_H_

#include <GL/freeglut.h>
#include <GL/glut.h>
#include "AvlTypes.h"

#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

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

		void addObject(AvlObject *obj, const string &name);
		void delObject(const string &name);

	private:
		static void avlResize(int width, int height);

		static void avlDisplay();

		int windowHandle;
		static int currentWidth;
		static int currentHeight;

		static unordered_map<string, AvlObject *> objects;
};

#endif // AVL_VISUALIZER_H_
