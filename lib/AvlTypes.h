#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <initializer_list>
#include <vector>
#include <string>
#include <iostream>

class AvlObject
{
	public:
		AvlObject(GLfloat x = 0, GLfloat y = 0, GLfloat width = 9, GLfloat height = 15,
				void *font = GLUT_BITMAP_9_BY_15)
		{
			_x = x;
			_y = y;
			_width = width;
			_height = height;
			_font = font;
		}

		virtual ~AvlObject() {}

		virtual GLfloat x() const { return _x; }
		virtual GLfloat y() const { return _y; }
		virtual GLfloat width() const { return _width; }
		virtual GLfloat height() const { return _height; }
		virtual void *font() const { return _font; }

		virtual void set_x(GLfloat x) { _x = x; }
		virtual void set_y(GLfloat y) { _y = y; }
		virtual void set_width(GLfloat width) { _width = width; }
		virtual void set_height(GLfloat height) { _height = height; }
		virtual void set_font(void *font) { _font = font; }

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
		AvlInt(int v = 0, GLfloat x = 0, GLfloat y = 0, GLfloat width = 9,
				GLfloat height = 15, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, width, height, font)
		{
			value = v;
		}

		virtual ~AvlInt() {}

		friend std::ostream& operator<<(std::ostream &os, const AvlInt &v)
		{
			os << v.value;
			return os;
		}

		const AvlInt& operator=(int v) { value = v; return *this; }

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
			glutBitmapString(font(), (const unsigned char *)(std::to_string(value).c_str()));
		}

	private:
		int value;
};

template <typename T>
class AvlArray : public AvlObject, public std::vector<T>
{
	public:
		AvlArray(size_t size) : std::vector<T>(size)
		{
			for (size_t i = 1; i < size; i++) {
				(*this)[i].set_x((*this)[i-1].x() + (*this)[i-1].width() * 1.5);
				(*this)[i].set_y((*this)[i-1].y());
			}
		}
		AvlArray(const std::initializer_list<T> &l) : std::vector<T>(l) {
			for (size_t i = 1; i < l.size(); i++) {
				(*this)[i].set_x((*this)[i-1].x() + (*this)[i-1].width() * 1.5);
				(*this)[i].set_y((*this)[i-1].y());
			}
		}

		virtual ~AvlArray() {}

		virtual GLfloat x() const { return (*this)[0].x(); }
		virtual GLfloat y() const { return (*this)[0].y(); }
		virtual GLfloat width() const
		{
			GLfloat w = 0;
			for (auto& v: *this)
				w += v.width();
			return w;
		}
		virtual GLfloat height() const
		{
			GLfloat h = 0;
			for (auto& v: *this)
				h = v.height() > h ? v.height() : h;
			return h;
		}
		virtual void *font() const { return (*this)[0].font(); }

		virtual void set_x(GLfloat x)
		{
			(*this)[0].set_x(x);
			for (size_t i = 1; i < this->size(); i++)
				(*this)[i].set_x((*this)[i-1].x() + (*this)[i-1].width());
		}
		virtual void set_y(GLfloat y)
		{
			for (auto& v: *this)
				v.set_y(y);
		}
		virtual void set_width(GLfloat width)
		{
			for (auto& v: *this)
				v.set_width(width / this->size());
			set_x((*this)[0].x());
		}
		virtual void set_height(GLfloat y)
		{
			for (auto& v: *this)
				v.set_height(y);
		}
		virtual void set_font(void *font)
		{
			for (auto& v: *this)
				v.set_font(font);
		}

		virtual void render() const
		{
			for (auto& v: *this)
				v.render();
		}
};

#endif // AVL_TYPES_H_
