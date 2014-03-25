#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <initializer_list>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <iostream>
#include "AvlUtils.h"

const float DEFAULT_DELAY = 0.5;

const int FPS = 20;

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

		AvlObject(const AvlObject &obj)
		{
			_x = obj._x;
			_y = obj._y;
			_width = obj._width;
			_height = obj._height;
			_font = obj._font;
		}

		const AvlObject& operator=(const AvlObject &obj)
		{
			_x = obj._x;
			_y = obj._y;
			_width = obj._width;
			_height = obj._height;
			_font = obj._font;

			return *this;
		}

		virtual ~AvlObject() {}

		GLfloat x() const { return _x; }
		GLfloat y() const { return _y; }
		GLfloat width() const { return _width; }
		GLfloat height() const { return _height; }
		AvlFont font() const { return _font; }

		void set_x(GLfloat x) { _x = x; }
		void set_y(GLfloat y) { _y = y; }
		void set_font(const AvlFont &font) { _font = font; }

		virtual void render() = 0;

		void lock() { mtx.lock(); }
		bool try_lock() { return mtx.try_lock(); }
		void unlock() { mtx.unlock(); }

	protected:
		void set_width(GLfloat width) { _width = width; }
		void set_height(GLfloat height) { _height = height; }

	private:
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
		AvlFont _font;

		std::mutex mtx;
};

inline void moveObject(std::shared_ptr<AvlObject> obj, GLfloat x, GLfloat y, float seconds)
{
	int steps = FPS * seconds;
	GLfloat delta_x = x / steps;
	GLfloat delta_y = y / steps;

	for (int i = 0; i < steps; i++) {
		obj->lock();
		obj->set_x(obj->x() + delta_x);
		obj->set_y(obj->y() + delta_y);
		obj->unlock();
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
			update();
		}

		virtual ~AvlInt() {}

		const AvlInt& operator=(int v) { value = v; update(); return *this; }

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
			ret.update();
			return ret;
		}

		const AvlInt& operator++() { value++; update(); return *this; }
		const AvlInt operator++(int)
		{
			AvlInt ret = *this;
			value++;
			update();
			return ret;
		}

		const AvlInt& operator--() { value--; update(); return *this; }
		const AvlInt operator--(int)
		{
			AvlInt ret = *this;
			value--;
			update();
			return ret;
		}

		const AvlInt operator+(int v) const
		{
			AvlInt ret = *this;
			ret.value += v;
			ret.update();
			return ret;
		}
		const AvlInt operator+(const AvlInt &v) const { return *this + v.value; }
		const AvlInt& operator+=(int v) { value += v; update(); return *this; }
		const AvlInt& operator+=(const AvlInt &v) { return *this += v.value; }
		friend const AvlInt operator+(int v1, const AvlInt &v2) { return v2 + v1; }

		const AvlInt operator-(int v) const
		{
			AvlInt ret = *this;
			ret.value -= v;
			ret.update();
			return ret;
		}
		const AvlInt operator-(const AvlInt &v) const { return *this - v.value; }
		const AvlInt& operator-=(int v) { value -= v; update(); return *this; }
		const AvlInt& operator-=(const AvlInt &v) { return *this -= v.value; }
		friend const AvlInt operator-(int v1, const AvlInt &v2) { return -v2 + v1; }

		const AvlInt operator*(int v) const
		{
			AvlInt ret = *this;
			ret.value *= v;
			ret.update();
			return ret;
		}
		const AvlInt operator*(const AvlInt &v) const { return *this * v.value; }
		const AvlInt& operator*=(int v) { value *= v; update(); return *this; }
		const AvlInt& operator*=(const AvlInt &v) { return *this *= v.value; }
		friend const AvlInt operator*(int v1, const AvlInt &v2) { return v2 * v1; }

		const AvlInt operator/(int v) const
		{
			AvlInt ret = *this;
			ret.value /= v;
			ret.update();
			return ret;
		}
		const AvlInt operator/(const AvlInt &v) const { return *this / v.value; }
		const AvlInt& operator/=(int v) { value /= v; update(); return *this; }
		const AvlInt& operator/=(const AvlInt &v) { return *this /= v.value; }
		friend const AvlInt operator/(int v1, const AvlInt &v2)
		{
			AvlInt ret = v2;
			ret.value = v1 / v2.value;
			ret.update();
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
class AvlArray : public AvlObject
{
	public:
		AvlArray(size_t size = 0, GLfloat x = 0, GLfloat y = 0,
				const AvlFont &font = GLUT_BITMAP_9_BY_15) : AvlObject(x, y, font), arr(size)
		{
			for (auto& v : arr)
				v = std::shared_ptr<T>(new T);

			updateMutex = std::shared_ptr<std::mutex>(new std::mutex);
		}

		AvlArray(const std::initializer_list<T> &l) : arr(l.size())
		{
			typename std::initializer_list<T>::const_iterator src = l.begin();
			typename std::vector< std::shared_ptr<T> >::iterator dst = arr.begin();

			while (src != l.end()) {
				*dst = std::shared_ptr<T>(new T(*src));
				src++;
				dst++;
			}

			updateMutex = std::shared_ptr<std::mutex>(new std::mutex);
		}

		virtual ~AvlArray() {}

		T& operator[](size_t index) { return *arr[index]; }
		const T& operator[](size_t index) const { return *arr[index]; }

		size_t size() const { return arr.size(); }

		AvlArray<T> subarray(size_t start, size_t end) const
		{
			AvlArray<T> ret(end - start);

			typename std::vector< std::shared_ptr<T> >::const_iterator src = arr.begin() + start;
			typename std::vector< std::shared_ptr<T> >::iterator dst = ret.arr.begin();

			while (src != arr.begin() + end) {
				*dst = *src;
				src++;
				dst++;
			}

			ret.updateMutex = updateMutex;

			return ret;
		}

		virtual void render()
		{
			if (updateMutex->try_lock()) {
				update();
				updateMutex->unlock();
			}

			for (auto& v : arr)
				v->render();
		}

		void swap(size_t idx1, size_t idx2)
		{
			std::shared_ptr<T> obj[2];
			std::thread moveThread[2];

			obj[0] = arr[idx1];
			obj[1] = arr[idx2];

			updateMutex->lock();

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

			T tmp = *arr[idx1];
			*arr[idx1] = *arr[idx2];
			*arr[idx2] = tmp;

			updateMutex->unlock();

			avlSleep(0.1);
		}

	private:
		void update()
		{
			GLfloat w = -font().width();

			for (typename std::vector< std::shared_ptr<T> >::iterator it = arr.begin();
					it != arr.end(); it++) {
				if (it == arr.begin()) {
					(*it)->set_x( x() );
					(*it)->set_y( y() );
					(*it)->set_font( font() );
					w += (*it)->width() + font().width();
				}
				else {
					(*it)->set_x( (*(it-1))->x() + (*(it-1))->width() + font().width() );
					(*it)->set_y( (*(it-1))->y() );
					(*it)->set_font( (*(it-1))->font() );
					w += (*it)->width() + font().width();
				}
			}

			set_width(w);
		}

		std::vector< std::shared_ptr<T> > arr;
		std::shared_ptr<std::mutex> updateMutex;
};

#endif // AVL_TYPES_H_
