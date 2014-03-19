#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <initializer_list>
#include <vector>
#include <string>
#include <iostream>

class AvlFont
{
	public:
		AvlFont(void *font = GLUT_BITMAP_9_BY_15)
		{
			_font = font;
			if (font == GLUT_BITMAP_9_BY_15) {
				_width = 9;
				_height = 15;
			}
			else {
				_width = 0;
				_height = 0;
			}
		}

		const AvlFont& operator=(void *font)
		{
			_font = font;
			if (font == GLUT_BITMAP_9_BY_15) {
				_width = 9;
				_height = 15;
			}
			else {
				_width = 0;
				_height = 0;
			}

			return *this;
		}

		GLfloat width() const { return _width; }
		GLfloat height() const { return _height; }
		void *font() const { return _font; }

	private:
		void *_font;
		GLfloat _width;
		GLfloat _height;
};

class AvlObject
{
	public:
		AvlObject(GLfloat x = 0, GLfloat y = 0, const AvlFont &font = GLUT_BITMAP_9_BY_15)
		{
			_x = x;
			_y = y;
			_width = font.width();
			_height = font.height();
			_font = font;
		}

		virtual ~AvlObject() {}

		virtual GLfloat x() const { return _x; }
		virtual GLfloat y() const { return _y; }
		virtual GLfloat width() const { return _width; }
		virtual GLfloat height() const { return _height; }
		virtual AvlFont font() const { return _font; }

		virtual void set_x(GLfloat x) { _x = x; }
		virtual void set_y(GLfloat y) { _y = y; }
		virtual void set_font(const AvlFont &font) { _font = font; }

		virtual void render() = 0;

	protected:
		virtual void set_width(GLfloat width) { _width = width; }
		virtual void set_height(GLfloat height) { _height = height; }

	private:
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
		AvlFont _font;
};

class AvlInt : public AvlObject
{
	public:
		AvlInt(int v = 0, GLfloat x = 0, GLfloat y = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font)
		{
			value = v;
		}

		virtual ~AvlInt() {}

		const AvlInt& operator=(int v) { value = v; return *this; }

		friend std::ostream& operator<<(std::ostream &os, const AvlInt &v)
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

		virtual void render()
		{
			update();

			glColor4f(1.0, 0.0, 0.0, 0.0);
			glRasterPos2f(x(), y());
			glutBitmapString(font().font(), (const unsigned char *)(std::to_string(value).c_str()));
		}

	private:
		void update() { set_width(numOfDigit() * font().width()); }

		int numOfDigit() const
		{
			if (value == 0)
				return 1;

			int num = value >= 0 ? 0 : 1;
			int k = value;

			while (k != 0) {
				k /= 10;
				num++;
			}

			return num;
		}

		int value;
};

template <typename T>
class AvlArray : public AvlObject, public std::vector<T>
{
	public:
		AvlArray(size_t size = 0, GLfloat x = 0, GLfloat y = 0,
				const AvlFont &font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font), std::vector<T>(size) {}
		AvlArray(const std::initializer_list<T> &l)
			: std::vector<T>(l) {}

		virtual ~AvlArray() {}

		virtual GLfloat x() const { return AvlObject::x(); }
		virtual GLfloat y() const { return AvlObject::y(); }
		virtual GLfloat width() const
		{
			GLfloat w = -font().width();
			for (auto& v: *this) {
				w += v.width();
				w += font().width();
			}
			return w;
		}
		virtual GLfloat height() const { return AvlObject::height(); }
		virtual AvlFont font() const { return AvlObject::font(); }

		virtual void set_x(GLfloat x) { AvlObject::set_x(x); }
		virtual void set_y(GLfloat y) { AvlObject::set_y(y); }
		virtual void set_font(const AvlFont &font) { AvlObject::set_font(font); }

		virtual void render()
		{
			update();

			for (auto& v: *this)
				v.render();
		}

	private:
		void update()
		{
			for (typename std::vector<T>::iterator it = this->begin();
					it != this->end(); it++) {
				if (it == this->begin()) {
					it->set_x(AvlObject::x());
					it->set_y(AvlObject::y());
					it->set_font(AvlObject::font());
				}
				else {
					it->set_x((it-1)->x() + (it-1)->width() + font().width());
					it->set_y((it-1)->y());
					it->set_font(this->begin()->font());
				}
			}
		}
};

#endif // AVL_TYPES_H_
