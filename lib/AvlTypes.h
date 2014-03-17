#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <string>
using std::to_string;

class AvlObject
{
	public:
		AvlObject(int x = 0, int y = 0, int width = 0, int height = 0, void *font = GLUT_BITMAP_9_BY_15)
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

		int x() const { return _x; }
		int y() const { return _y; }
		int width() const { return _width; }
		int height() const { return _height; }
		void *font() const { return _font; }

		void set_x(int x) { _x = x; }
		void set_y(int y) { _y = y; }
		void set_width(int width) { _width = width; }
		void set_height(int height) { _height = height; }
		void set_font(void *font) { _font = font; }

	protected:
		virtual void render() const = 0;

	private:
		int _x;
		int _y;
		int _width;
		int _height;
		void *_font;
};

class AvlInt : public AvlObject
{
	public:
		AvlInt(int v = 0, int x = 0, int y = 0, int width = 0,
				int height = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, width, height, font)
		{
			value = v;
		}

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

		void render() const
		{
			glColor4f(1.0, 0.0, 0.0, 0.0);
			glRasterPos2f(x(), y());
			glutBitmapString(font(), (const unsigned char *)(to_string(value).c_str()));
		}

	private:
		int value;
};

#endif // AVL_TYPES_H_
