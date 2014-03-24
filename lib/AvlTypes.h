#ifndef AVL_TYPES_H_
#define AVL_TYPES_H_

#include <GL/freeglut.h>
#include <initializer_list>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <pthread.h>
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

			pthread_mutex_init(&mutex, NULL);
		}

		virtual ~AvlObject()
		{
			pthread_mutex_destroy(&mutex);
		}

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

		void lock() { pthread_mutex_lock(&mutex); }
		void unlock() { pthread_mutex_unlock(&mutex); }

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

		pthread_mutex_t mutex;
};

struct moveArg
{
	std::shared_ptr<AvlObject> obj;
	GLfloat x;
	GLfloat y;
	float seconds;
};

inline void *moveThread(void *arglist)
{
	moveArg *args = (moveArg *)arglist;

	int steps = FPS * args->seconds;
	GLfloat delta_x = args->x / steps;
	GLfloat delta_y = args->y / steps;

	for (int i = 0; i < steps; i++) {
		args->obj->set_x(args->obj->x() + delta_x);
		args->obj->set_y(args->obj->y() + delta_y);
		avlSleep(1.0 / FPS);
	}

	return NULL;
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

			shouldUpdate = true;
		}
		AvlArray(const std::initializer_list<T> &l) : std::vector< std::shared_ptr<T> >(l.size())
		{
			typename std::initializer_list<T>::iterator it1 = l.begin();
			typename std::vector< std::shared_ptr<T> >::iterator it2 = this->begin();

			while (it1 != l.end()) {
				*it2 = std::shared_ptr<T>(new T(*it1));
				it1++;
				it2++;
			}

			shouldUpdate = true;
		}

		virtual ~AvlArray() {}

		T& operator[](size_t index) { return *(std::vector< std::shared_ptr<T> >::operator[](index)); }
		const T& operator[](size_t index) const { return *(std::vector< std::shared_ptr<T> >::operator[](index)); }

		size_t size() const { return std::vector< std::shared_ptr<T> >::size(); }

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
			if (shouldUpdate)
				update();

			for (auto& v: *this)
				v->render();
		}

		void swap(size_t idx1, size_t idx2)
		{
			shouldUpdate = false;

			pthread_t thread[2];
			moveArg args[2];

			args[0].obj = std::vector< std::shared_ptr<T> >::operator[](idx1);
			args[1].obj = std::vector< std::shared_ptr<T> >::operator[](idx2);

			args[0].x = 0;
			args[1].x = 0;
			args[0].y = args[0].obj->height() * 1.5;
			args[1].y = args[1].obj->height() * 1.5;
			args[0].seconds = DEFAULT_DELAY;
			args[1].seconds = DEFAULT_DELAY;

			pthread_create(&thread[0], NULL, moveThread, (void *)(&args[0]));
			pthread_create(&thread[1], NULL, moveThread, (void *)(&args[1]));
			pthread_join(thread[0], NULL);
			pthread_join(thread[1], NULL);

			args[0].x = args[1].obj->x() - args[0].obj->x();
			args[1].x = args[0].obj->x() - args[1].obj->x();
			args[0].y = 0;
			args[1].y = 0;
			args[0].seconds = DEFAULT_DELAY * abs(idx1 - idx2);
			args[1].seconds = DEFAULT_DELAY * abs(idx1 - idx2);

			pthread_create(&thread[0], NULL, moveThread, (void *)(&args[0]));
			pthread_create(&thread[1], NULL, moveThread, (void *)(&args[1]));
			pthread_join(thread[0], NULL);
			pthread_join(thread[1], NULL);

			args[0].x = 0;
			args[1].x = 0;
			args[0].y = - args[0].obj->height() * 1.5;
			args[1].y = - args[1].obj->height() * 1.5;
			args[0].seconds = DEFAULT_DELAY;
			args[1].seconds = DEFAULT_DELAY;

			pthread_create(&thread[0], NULL, moveThread, (void *)(&args[0]));
			pthread_create(&thread[1], NULL, moveThread, (void *)(&args[1]));
			pthread_join(thread[0], NULL);
			pthread_join(thread[1], NULL);

			std::shared_ptr<T> tmp = std::vector< std::shared_ptr<T> >::operator[](idx1);
			std::vector< std::shared_ptr<T> >::operator[](idx1) = std::vector< std::shared_ptr<T> >::operator[](idx2);
			std::vector< std::shared_ptr<T> >::operator[](idx2) = tmp;

			shouldUpdate = true;
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

		bool shouldUpdate;
};

#endif // AVL_TYPES_H_
