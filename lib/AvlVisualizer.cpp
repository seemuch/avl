#include "AvlVisualizer.h"
#include "AvlTypes.h"

int AvlVisualizer::currentWidth = 0;
int AvlVisualizer::currentHeight = 0;
std::unordered_map<std::string, AvlObject *> AvlVisualizer::objects;
std::atomic<int> AvlVisualizer::level(0);
std::stack<int> AvlVisualizer::levelBackup;

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

void AvlVisualizer::start()
{
	level++;
}

void AvlVisualizer::stop()
{
	level--;
}

void AvlVisualizer::reset()
{
	levelBackup.push(level.load());
	level.store(0);
}

void AvlVisualizer::restore()
{
	level.store(levelBackup.top());
	levelBackup.pop();
}

int AvlVisualizer::getLevel() const
{
	return level.load();
}

void AvlVisualizer::addObject(AvlObject *obj, const std::string &name)
{
	if (objects.find(name) == objects.end()) {
		objects[name] = obj;
		obj->setVisualizer(this);
	}

	placeObject();
}

void AvlVisualizer::delObject(const std::string &name)
{
	if (objects.find(name) != objects.end())
		objects.erase(name);

	placeObject();
}

void AvlVisualizer::placeObject()
{
	std::vector<AvlObject *> arrays;
	std::vector<AvlObject *> others;

	for (auto& o : objects) {
		AvlObject *obj = o.second;
		if (typeid(*obj) == typeid(AvlArray<AvlInt>) || typeid(*obj) == typeid(AvlArray<AvlChar>)
				|| typeid(*obj) == typeid(AvlArray<AvlBool>))
			arrays.push_back(obj);
		else {
			if (typeid(*obj) != typeid(AvlIndex))
				others.push_back(obj);
		}
	}

	if (!arrays.empty()) {
		int h = Default_Height / arrays.size() / 2;
		for (size_t i = 0; i < arrays.size(); i++) {
			arrays[i]->lock();
			int x = -arrays[i]->width() / 2;
			int y = h * (int(arrays.size()) / 2 - i);
			arrays[i]->set_x(x);
			arrays[i]->set_y(y);
			arrays[i]->unlock();
		}
	}

	if (!others.empty()) {
		int w = Default_Width / others.size() / 2;
		for (size_t i = 0; i < others.size(); i++) {
			others[i]->lock();
			int x = w * (int(others.size() / 2) - i);
			int y = Default_Height / 2 - AvlFont().height() * 2;
			others[i]->set_x(x);
			others[i]->set_y(y);
			others[i]->unlock();
		}
	}
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
	if (level.load() > 0) {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& obj : objects)
			obj.second->render();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}
