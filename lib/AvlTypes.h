#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <string>
using std::to_string;

class AvlObject
{
	public:
		AvlObject(GLfloat x = 0, GLfloat y = 0, GLfloat width = 0, GLfloat height = 0,
				void *font = GLUT_BITMAP_9_BY_15)
		{
			_x = x;
			_y = y;
			_width = width;
			_height = height;
			_font = font;
		}

		AvlObject(const AvlObject &obj)
		{
			_x = obj._x;
			_y = obj._y;
			_width = obj._width;
			_height = obj._height;
			_font = obj._font;
		}

		virtual ~AvlObject() {}

		GLfloat x() const { return _x; }
		GLfloat y() const { return _y; }
		GLfloat width() const { return _width; }
		GLfloat height() const { return _height; }
		void *font() const { return _font; }

		void set_x(GLfloat x) { _x = x; }
		void set_y(GLfloat y) { _y = y; }
		void set_width(GLfloat width) { _width = width; }
		void set_height(GLfloat height) { _height = height; }
		void set_font(void *font) { _font = font; }

		virtual void render() const = 0;

	private:
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
		void *_font;
};

class AvlInt : public AvlObject
{
	public:
		AvlInt(int v = 0, GLfloat x = 0, GLfloat y = 0, GLfloat width = 0,
				GLfloat height = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, width, height, font)
		{
			value = v;
		}

		virtual ~AvlInt() {}

		AvlInt operator+(int v) const
		{
			AvlInt ret = *this;
			ret.value += v;
			return ret;
		}

		AvlInt operator+=(int v)
		{
			value += v;
			return *this;
		}

		AvlInt operator+(const AvlInt &v) const
		{
			return *this + v.value;
		}

		AvlInt operator+=(const AvlInt &v)
		{
			return *this += v.value;
		}

		int val() const
		{
			return value;
		}

		virtual void render() const
		{
			glColor4f(1.0, 0.0, 0.0, 0.0);
			glRasterPos2f(x(), y());
			glutBitmapString(font(), (const unsigned char *)(to_string(value).c_str()));
		}

	private:
		int value;
};

#endif // AVL_TYPES_H_
