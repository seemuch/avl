#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>

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

		AvlObject operator=(const AvlObject &obj)
		{
			_x = obj._x;
			_y = obj._y;
			_width = obj._width;
			_height = obj._height;
			_font = obj._font;

			return *this;
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

	private:
		int value;
};

#endif // AVL_TYPES_H_
