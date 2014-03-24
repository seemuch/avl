#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <initializer_list>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <iostream>
#include "AvlUtils.h"

const float DEFAULT_DELAY = 0.5;

const int FPS = 25;

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

			_delay = DEFAULT_DELAY;
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

		float delay() const { return _delay; }

	protected:
		virtual void set_width(GLfloat width) { _width = width; }
		virtual void set_height(GLfloat height) { _height = height; }

	private:
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
		AvlFont _font;

		float _delay;
};

inline void moveObject(std::shared_ptr<AvlObject> obj, GLfloat x, GLfloat y, float seconds)
{
	int steps = FPS * seconds;
	GLfloat delta_x = x / steps;
	GLfloat delta_y = y / steps;

	for (int i = 0; i < steps; i++) {
		obj->set_x(obj->x() + delta_x);
		obj->set_y(obj->y() + delta_y);
		avlSleep(1.0 / FPS);
	}
}

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

		bool operator <(int v) const { return value < v; }
		bool operator <(const AvlInt &v) const { return value < v.value; }
		friend bool operator<(int v1, const AvlInt v2) { return v1 < v2.value; }

		bool operator <=(int v) const { return value <= v; }
		bool operator <=(const AvlInt &v) const { return value <= v.value; }
		friend bool operator<=(int v1, const AvlInt v2) { return v1 <= v2.value; }

		bool operator >(int v) const { return value > v; }
		bool operator >(const AvlInt &v) const { return value > v.value; }
		friend bool operator>(int v1, const AvlInt v2) { return v1 > v2.value; }

		bool operator >=(int v) const { return value >= v; }
		bool operator >=(const AvlInt &v) const { return value >= v.value; }
		friend bool operator>=(int v1, const AvlInt v2) { return v1 >= v2.value; }

		bool operator ==(int v) const { return value == v; }
		bool operator ==(const AvlInt &v) const { return value == v.value; }
		friend bool operator==(int v1, const AvlInt v2) { return v1 == v2.value; }

		bool operator !=(int v) const { return value != v; }
		bool operator !=(const AvlInt &v) const { return value != v.value; }
		friend bool operator!=(int v1, const AvlInt v2) { return v1 != v2.value; }

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

// ***NOTE!***: every element in this array is a pointer
template <typename T>
class AvlArray : public AvlObject, private std::vector< std::shared_ptr<T> >
{
	public:
		AvlArray(size_t size = 0, GLfloat x = 0, GLfloat y = 0,
				const AvlFont &font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font), std::vector< std::shared_ptr<T> >(size)
		{
			for (auto& v: *this)
				v = std::shared_ptr<T>(new T);

			shouldUpdate = std::shared_ptr<bool>(new bool(true));
		}
		AvlArray(const std::initializer_list<T> &l) : std::vector< std::shared_ptr<T> >(l.size())
		{
			typename std::initializer_list<T>::iterator src = l.begin();
			typename std::vector< std::shared_ptr<T> >::iterator dst = this->begin();

			while (src != l.end()) {
				*dst = std::shared_ptr<T>(new T(*src));
				src++;
				dst++;
			}

			shouldUpdate = std::shared_ptr<bool>(new bool(true));
		}

		virtual ~AvlArray() {}

		T& operator[](size_t index) { return *std::vector< std::shared_ptr<T> >::operator[](index); }
		const T& operator[](size_t index) const { return *std::vector< std::shared_ptr<T> >::operator[](index); }

		size_t size() const { return std::vector< std::shared_ptr<T> >::size(); }

		AvlArray<T> subarray(size_t s, size_t e) const
		{
			AvlArray<T> ret(e - s);

			typename std::vector< std::shared_ptr<T> >::const_iterator src = this->begin() + s;
			typename AvlArray<T>::iterator dst = ret.begin();

			while (src != this->begin() + e) {
				*dst = *src;
				src++;
				dst++;
			}

			ret.shouldUpdate = shouldUpdate;

			return ret;
		}

		virtual GLfloat width() const
		{
			GLfloat w = -font().width();
			for (auto& v: *this) {
				w += v->width();
				w += font().width();
			}
			return w;
		}

		virtual void render()
		{
			if (*shouldUpdate)
				update();

			for (auto& v: *this)
				v->render();
		}

		void swap(size_t idx1, size_t idx2)
		{
			*shouldUpdate = false;

			std::shared_ptr<T> obj[2];
			std::thread moveThread[2];

			obj[0] = std::vector< std::shared_ptr<T> >::operator[](idx1);
			obj[1] = std::vector< std::shared_ptr<T> >::operator[](idx2);

			moveThread[0] = std::thread(moveObject, obj[0], 0, obj[0]->height() * 1.5, DEFAULT_DELAY);
			moveThread[1] = std::thread(moveObject, obj[1], 0, obj[1]->height() * 1.5, DEFAULT_DELAY);
			moveThread[0].join();
			moveThread[1].join();

			moveThread[0] = std::thread(moveObject, obj[0], obj[1]->x() - obj[0]->x(), 0, DEFAULT_DELAY * abs(idx1 - idx2));
			moveThread[1] = std::thread(moveObject, obj[1], obj[0]->x() - obj[1]->x(), 0, DEFAULT_DELAY * abs(idx1 - idx2));
			moveThread[0].join();
			moveThread[1].join();

			moveThread[0] = std::thread(moveObject, obj[0], 0, -obj[0]->height() * 1.5, DEFAULT_DELAY);
			moveThread[1] = std::thread(moveObject, obj[1], 0, -obj[1]->height() * 1.5, DEFAULT_DELAY);
			moveThread[0].join();
			moveThread[1].join();

			T tmp = *std::vector< std::shared_ptr<T> >::operator[](idx1);
			*std::vector< std::shared_ptr<T> >::operator[](idx1) = *std::vector< std::shared_ptr<T> >::operator[](idx2);
			*std::vector< std::shared_ptr<T> >::operator[](idx2) = tmp;

			*shouldUpdate = true;
			avlSleep(0.1);
		}

	private:
		void update()
		{
			for (typename std::vector< std::shared_ptr<T> >::iterator it = this->begin();
					it != this->end(); it++) {
				if (it == this->begin()) {
					(*it)->set_x(AvlObject::x());
					(*it)->set_y(AvlObject::y());
					(*it)->set_font(AvlObject::font());
				}
				else {
					(*it)->set_x( (*(it-1))->x() + (*(it-1))->width() + font().width() );
					(*it)->set_y( (*(it-1))->y() );
					(*it)->set_font( (*(it-1))->font() );
				}
			}
		}

		std::shared_ptr<bool> shouldUpdate;
};

#endif // AVL_TYPES_H_
