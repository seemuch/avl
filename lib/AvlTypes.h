#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <string>
#include <iostream>
using std::to_string;
using std::ostream;

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

		friend ostream& operator<<(ostream &os, const AvlInt &v)
		{
			os << v.value;
			return os;
		}

		const AvlInt& operator+() const { return *this; }
		const AvlInt operator-() const
		{
			AvlInt ret = *this;
			ret.value = -value;
			return ret;
		}

		const AvlInt operator+(int v) const
		{
			AvlInt ret = *this;
			ret.value += v;
			return ret;
		}
		const AvlInt operator+(const AvlInt &v) const { return *this + v.value; }
		const AvlInt& operator+=(int v) { value += v; return *this; }
		const AvlInt& operator+=(const AvlInt &v) { return *this += v.value; }
		friend const AvlInt operator+(int v1, const AvlInt &v2) { return v2 + v1; }

		const AvlInt operator-(int v) const
		{
			AvlInt ret = *this;
			ret.value -= v;
			return ret;
		}
		const AvlInt operator-(const AvlInt &v) const { return *this - v.value; }
		const AvlInt& operator-=(int v) { value -= v; return *this; }
		const AvlInt& operator-=(const AvlInt &v) { return *this -= v.value; }
		friend const AvlInt operator-(int v1, const AvlInt &v2) { return -v2 + v1; }

		const AvlInt operator*(int v) const
		{
			AvlInt ret = *this;
			ret.value *= v;
			return ret;
		}
		const AvlInt operator*(const AvlInt &v) const { return *this * v.value; }
		const AvlInt& operator*=(int v) { value *= v; return *this; }
		const AvlInt& operator*=(const AvlInt &v) { return *this *= v.value; }
		friend const AvlInt operator*(int v1, const AvlInt &v2) { return v2 * v1; }

		const AvlInt operator/(int v) const
		{
			AvlInt ret = *this;
			ret.value /= v;
			return ret;
		}
		const AvlInt operator/(const AvlInt &v) const { return *this / v.value; }
		const AvlInt& operator/=(int v) { value /= v; return *this; }
		const AvlInt& operator/=(const AvlInt &v) { return *this /= v.value; }
		friend const AvlInt operator/(int v1, const AvlInt &v2)
		{
			AvlInt ret = v2;
			ret.value = v1 / v2.value;
			return ret;
		}

		int val() const { return value; }

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
