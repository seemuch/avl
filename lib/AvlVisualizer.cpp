#include "AvlVisualizer.h"

int AvlVisualizer::currentWidth = 0;
int AvlVisualizer::currentHeight = 0;
unordered_map<string, const AvlObject *> AvlVisualizer::objects;

AvlVisualizer::AvlVisualizer(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(OpenGL_Major_Version, OpenGL_Minor_Version);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(Default_Width, Default_Height);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	windowHandle = glutCreateWindow(Title);

	glutReshapeFunc(AvlVisualizer::avlResize);
	glutDisplayFunc(AvlVisualizer::avlDisplay);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}

AvlVisualizer::~AvlVisualizer()
{
}

void AvlVisualizer::show()
{
	glutMainLoop();
}

void AvlVisualizer::addObject(const AvlObject *obj, const string &name)
{
	if (objects.find(name) == objects.end())
		objects[name] = obj;
}

void AvlVisualizer::delObject(const string &name)
{
	if (objects.find(name) != objects.end())
		objects.erase(name);
}

void AvlVisualizer::avlResize(int width, int height)
{
	GLdouble size;
	GLdouble aspect;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	size = (GLdouble)(width >= height ? width : height) / 2.0;
	if (width <= height) {
		aspect = (GLdouble)height / (GLdouble)width;
		glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
	}
	else {
		aspect = (GLdouble)width / (GLdouble)height;
		glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
	}

	glScaled(aspect, aspect, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void AvlVisualizer::avlDisplay()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& obj: objects)
		obj.second->render();

	glutSwapBuffers();
	glutPostRedisplay();
}
