#include "AvlVisualizer.h"

int AvlVisualizer::currentWidth = 0;
int AvlVisualizer::currentHeight = 0;

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

void AvlVisualizer::avlResize(int width, int height)
{
	currentWidth = width;
	currentHeight = height;
	glViewport(0, 0, currentWidth, currentHeight);
}

void AvlVisualizer::avlDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
	glutPostRedisplay();
}
