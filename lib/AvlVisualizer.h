#ifndef AVL_VISUALIZER_H_
#define AVL_VISUALIZER_H_

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <unordered_map>
#include <stack>
#include <string>
#include <atomic>

const int OpenGL_Major_Version = 2;
const int OpenGL_Minor_Version = 1;
const char *const Title = "Algorithm Visualization";
const int Default_Width = 640;
const int Default_Height = 480;

class AvlObject;

class AvlVisualizer
{
	public:
		AvlVisualizer(int argc, char *argv[]);
		~AvlVisualizer();

		void show();

		void addObject(AvlObject *obj, const std::string &name);
		void delObject(const std::string &name);

		void start();
		void stop();
		void reset();
		void restore();
		int getLevel() const;

	private:
		static void avlResize(int width, int height);
		static void avlDisplay();

		void placeObject();

		int windowHandle;
		static int currentWidth;
		static int currentHeight;

		static std::unordered_map<std::string, AvlObject *> objects;

		static std::atomic<int> level;
		static std::stack<int> levelBackup;
};

#endif // AVL_VISUALIZER_H_
