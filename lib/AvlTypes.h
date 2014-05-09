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
#include "AvlVisualizer.h"

const float DEFAULT_DELAY = 0.5;

const int FPS = 20;

typedef unsigned int AvlColor;
const AvlColor AVL_RED = 0xFF0000;
const AvlColor AVL_GREEN = 0x00FF00;
const AvlColor AVL_WIGHT = 0xFFFFFF;
const AvlColor AVL_COLOR_HIGHLIGHT = AVL_GREEN;
const AvlColor AVL_COLOR_DEFAULT = AVL_RED;
const AvlColor AVL_AUXILIARY_COLOR = AVL_WIGHT;

class AvlFont
{
	public:
		// constructor
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

		// assignment operator 
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
		
		// getter functions 
		GLfloat width() const { return _width; }
		GLfloat height() const { return _height; }
		void *font() const { return _font; }

	private:
		void *_font;
		GLfloat _width;
		GLfloat _height;
}; // end of AvlFont

class AvlObject
{
	public:
		// constructor
		AvlObject(GLfloat x = 0, GLfloat y = 0, const AvlFont &font = GLUT_BITMAP_9_BY_15)
		{
			_x = x;
			_y = y;
			_width = font.width();
			_height = font.height();
			_font = font;
			_color = AVL_COLOR_DEFAULT;

			_vi = NULL;
		}

		// copy constructor
		AvlObject(const AvlObject &obj)
		{
			_x = obj._x;
			_y = obj._y;
			_width = obj._width;
			_height = obj._height;
			_font = obj._font;
			_color = obj._color;

			_vi = obj._vi;
		}

		// assignment operator
		const AvlObject& operator=(const AvlObject &obj)
		{
			_x = obj._x;
			_y = obj._y;
			_width = obj._width;
			_height = obj._height;
			_font = obj._font;
			_color = obj._color;

			_vi = obj._vi;

			return *this;
		}

		// destructor (virtual)
		virtual ~AvlObject() {}

		// getter functions
		GLfloat x() const { return _x; }
		GLfloat y() const { return _y; }
		GLfloat width() const { return _width; }
		GLfloat height() const { return _height; }
		AvlFont font() const { return _font; }
		AvlColor color() const { return _color; }
		std::string name() const { return _name; }

		// setter functions
		void set_x(GLfloat x) { _x = x; }
		void set_y(GLfloat y) { _y = y; }
		void set_font(const AvlFont &font) { _font = font; }
		void set_color(AvlColor color) { _color = color; }
		void set_name(const std::string& name) { _name = name; }

		virtual void render() = 0;

		// locks
		void lock() { mtx.lock(); }
		bool try_lock() { return mtx.try_lock(); }
		void unlock() { mtx.unlock(); }

		// set visualizer
		void setVisualizer(const AvlVisualizer *vi) { _vi = vi; }

	protected:
		// position setter
		void set_width(GLfloat width) { _width = width; }
		void set_height(GLfloat height) { _height = height; }

		bool isDisplaying() const { return _vi->getLevel() > 0; }

	private:
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
		AvlFont _font;
		AvlColor _color;

		std::mutex mtx;

		const AvlVisualizer *_vi;

		std::string _name;
}; // end of AvlObject

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
		// constructor
		AvlInt(int v = 0, GLfloat x = 0, GLfloat y = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font)
		{
			value = v;
			update();
		}

		// destructor
		virtual ~AvlInt() {}

		// assignment operator
		const AvlInt& operator=(int v) { value = v; update(); return *this; }

		// << operator
		friend std::ostream& operator<<(std::ostream &os, const AvlInt &v)
		{
			os << v.value;
			return os;
		}

		// unary plus and minus
		const AvlInt& operator+() const { return *this; }
		const AvlInt operator-() const
		{
			AvlInt ret = *this;
			ret.value = -value;
			ret.update();
			return ret;
		}

		// pre-increment and post-increment
		const AvlInt& operator++() { value++; update(); return *this; }
		const AvlInt operator++(int)
		{
			AvlInt ret = *this;
			value++;
			update();
			return ret;
		}

		// pre-decrement and post-decrement
		const AvlInt& operator--() { value--; update(); return *this; }
		const AvlInt operator--(int)
		{
			AvlInt ret = *this;
			value--;
			update();
			return ret;
		}

		// binary plus
		const AvlInt operator+(int v) const
		{
			AvlInt ret = *this;
			ret.value += v;
			ret.update();
			return ret;
		}

		// binary plus
		const AvlInt operator+(const AvlInt &v) const { return *this + v.value; }
		friend const AvlInt operator+(int v1, const AvlInt &v2) { return v2 + v1; }

		// compound plus 
		const AvlInt& operator+=(int v) { value += v; update(); return *this; }
		const AvlInt& operator+=(const AvlInt &v) { return *this += v.value; }
		
		
		// binary minus
		const AvlInt operator-(int v) const
		{
			AvlInt ret = *this;
			ret.value -= v;
			ret.update();
			return ret;
		}
		const AvlInt operator-(const AvlInt &v) const { return *this - v.value; }
		friend const AvlInt operator-(int v1, const AvlInt &v2) { return -v2 + v1; }

		// compound minus
		const AvlInt& operator-=(int v) { value -= v; update(); return *this; }
		const AvlInt& operator-=(const AvlInt &v) { return *this -= v.value; }


		// multiplication
		const AvlInt operator*(int v) const
		{
			AvlInt ret = *this;
			ret.value *= v;
			ret.update();
			return ret;
		}
		const AvlInt operator*(const AvlInt &v) const { return *this * v.value; }
		friend const AvlInt operator*(int v1, const AvlInt &v2) { return v2 * v1; }
		
		// compound multiplication
		const AvlInt& operator*=(int v) { value *= v; update(); return *this; }
		const AvlInt& operator*=(const AvlInt &v) { return *this *= v.value; }

		// devision
		const AvlInt operator/(int v) const
		{
			AvlInt ret = *this;
			ret.value /= v;
			ret.update();
			return ret;
		}
		const AvlInt operator/(const AvlInt &v) const { return *this / v.value; }
		friend const AvlInt operator/(int v1, const AvlInt &v2)
		{
			AvlInt ret = v2;
			ret.value = v1 / v2.value;
			ret.update();
			return ret;
		}

		// compound division
		const AvlInt& operator/=(int v) { value /= v; update(); return *this; }
		const AvlInt& operator/=(const AvlInt &v) { return *this /= v.value; }
	
		// comparison operators
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

		// value getter
		int val() const { return value; }

		// render function
		virtual void render()
		{
			unsigned int red = color() / 0x10000;
			unsigned int green = color() % 0x10000 / 0x100;
			unsigned int blue = color() % 0x100;
			glColor4f(red / 255.0, green / 255.0, blue / 255.0, 0.0);

			glRasterPos2f(x(), y());
			glutBitmapString(font().font(), (const unsigned char *)(std::to_string(value).c_str()));
		}

		void highlight() { 
			set_color(AVL_COLOR_HIGHLIGHT); 
		}
		void lowlight() { 
			set_color(AVL_COLOR_DEFAULT); 
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
}; // end of AvlInt

class AvlChar: public AvlObject
{
	public:
		// constructor
	 	AvlChar (char v = 0, GLfloat x = 0, GLfloat y = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font)
		{
			value = v;
		}

		// destructor
		virtual ~AvlChar() {}

		// assignment operator
		const AvlChar & operator=(char v) { value = v; return *this; }

		// << operator
		friend std::ostream& operator<<(std::ostream &os, const AvlChar &v)
		{
			os << v.value;
			return os;
		}

		// unary plus and minus
		const AvlChar & operator+() const { return *this; }
		const AvlChar operator-() const
		{
			AvlChar ret = *this;
			ret.value = -value;
			return ret;
		}

		// pre-increment and post-increment
		const AvlChar & operator++() { value++; return *this; }
		const AvlChar operator++(int)
		{
			AvlChar ret = *this;
			value++;
			return ret;
		}

		// pre-decrement and post-decrement
		const AvlChar & operator--() { value--; return *this; }
		const AvlChar operator--(int)
		{
			AvlChar ret = *this;
			value--;
			return ret;
		}

		// binary plus
		const AvlChar operator+(int v) const
		{
			AvlChar ret = *this;
			ret.value += v;
			return ret;
		}

		const AvlChar operator+(char v) const
		{
			AvlChar ret = *this;
			ret.value += v;
			return ret;
		}

		// binary plus
		const AvlChar operator+(const AvlInt &v) const { return *this + v.val(); }
		const AvlChar operator+(const AvlChar &v) const { return *this +  v.val(); }
		friend const AvlChar operator+(int v1, const AvlChar &v2) { return v2 + v1; }
		friend const AvlChar operator+(char v1, const AvlChar &v2) { return v2 + v1; }
		friend const AvlChar operator+(AvlInt v1, const AvlChar &v2) { return v2 + v1.val(); }
		friend const AvlChar operator+(AvlChar v1, const AvlChar &v2) { return v2 + v1.val(); }

		// compound plus 
		const AvlChar & operator+=(int v) { value += v; return *this; }
		const AvlChar & operator+=(char v) { value += v; return *this; }
		const AvlChar & operator+=(const AvlInt &v) { return *this += v.val(); }
		const AvlChar & operator+=(const AvlChar &v) { return *this += v.val(); }

		
		// binary minus
		const AvlChar operator-(int v) const
		{
			AvlChar ret = *this;
			ret.value -= v;
			return ret;
		}

		const AvlChar operator-(char v) const
		{
			AvlChar ret = *this;
			ret.value -= v;
			return ret;
		}

		const AvlChar operator-(const AvlInt &v) const { return *this - v.val(); }
		const AvlChar operator-(const AvlChar &v) const { return *this - v.val(); }
		friend const AvlChar operator-(int v1, const AvlChar &v2) { return -v2 + v1; }
		friend const AvlChar operator-(char v1, const AvlChar &v2) { return -v2 + v1; }
		friend const AvlChar operator-(AvlInt v1, const AvlChar &v2) { return -v2 + v1.val(); }
		friend const AvlChar operator-(AvlChar v1, const AvlChar &v2) { return -v2 + v1.val(); }

		// compound minus
		const AvlChar & operator-=(int v) { value -= v; return *this; }
		const AvlChar & operator-=(char v) { value -= v; return *this; }
		const AvlChar & operator-=(const AvlInt &v) { return *this -= v.val(); }
		const AvlChar & operator-=(const AvlChar &v) { return *this -= v.val(); }


		// multiplication
		const AvlChar operator*(int v) const
		{
			AvlChar ret = *this;
			ret.value *= v;
			return ret;
		}

		const AvlChar operator*(char v) const
		{
			AvlChar ret = *this;
			ret.value *= v;
			return ret;
		}

		const AvlChar operator*(const AvlInt &v) const { return *this * v.val(); }
		const AvlChar operator*(const AvlChar &v) const { return *this * v.val(); }
		friend const AvlChar operator*(int v1, const AvlChar &v2) { return v2 * v1; }
		friend const AvlChar operator*(char v1, const AvlChar &v2) { return v2 * v1; }
		friend const AvlChar operator*(AvlInt v1, const AvlChar &v2) { return v2 * v1.val(); }
		friend const AvlChar operator*(AvlChar v1, const AvlChar &v2) { return v2 * v1.val(); }
		
		// compound multiplication
		const AvlChar & operator*=(int v) { value *= v; return *this; }
		const AvlChar & operator*=(char v) { value *= v; return *this; }
		const AvlChar & operator*=(const AvlInt &v) { return *this *= v.val(); }
		const AvlChar & operator*=(const AvlChar &v) { return *this *= v.val(); }

		// division
		const AvlChar operator/(int v) const
		{
			AvlChar ret = *this;
			ret.value /= v;
			return ret;
		}

		const AvlChar operator/(char v) const
		{
			AvlChar ret = *this;
			ret.value /= v;
			return ret;
		}

		const AvlChar operator/(const AvlInt &v) const { return *this / v.val(); }
		const AvlChar operator/(const AvlChar &v) const { return *this / v.val(); }
		friend const AvlChar operator/(int v1, const AvlChar &v2) { return v2 / v1; }
		friend const AvlChar operator/(char v1, const AvlChar &v2) { return v2 / v1; }
		friend const AvlChar operator/(AvlInt v1, const AvlChar &v2) { return v2 / v1.val(); }
		friend const AvlChar operator/(AvlChar v1, const AvlChar &v2) { return v2 / v1.val(); }

		// compound division
		const AvlChar & operator/=(int v) { value /= v; return *this; }
		const AvlChar & operator/=(char v) { value /= v; return *this; }
		const AvlChar & operator/=(const AvlInt &v) { return *this /= v.val(); }
		const AvlChar & operator/=(const AvlChar &v) { return *this /= v.val(); }
	
		// comparison operators
		bool operator <(int v) const { return value < v; }
		bool operator <(char v) const { return value < v; }
		bool operator <(const AvlInt &v) const { return value < v.val(); }
		bool operator <(const AvlChar &v) const { return value < v.value; }
		friend bool operator<(int v1, const AvlChar v2) { return v1 < v2.value; }
		friend bool operator<(AvlInt v1, const AvlChar v2) { return v1.val() < v2.value; }

		bool operator <=(int v) const { return value <= v; }
		bool operator <=(char v) const { return value <= v; }
		bool operator <=(const AvlInt &v) const { return value <= v.val(); }
		bool operator <=(const AvlChar &v) const { return value <= v.val(); }
		friend bool operator<=(int v1, const AvlChar v2) { return v1 <= v2.value; }
		friend bool operator<=(AvlInt v1, const AvlChar v2) { return v1.val() <= v2.value; }

		bool operator >(int v) const { return value > v; }
		bool operator >(char v) const { return value > v; }
		bool operator >(const AvlInt &v) const { return value > v.val(); }
		bool operator >(const AvlChar &v) const { return value > v.value; }
		friend bool operator>(int v1, const AvlChar v2) { return v1 > v2.value; }
		friend bool operator>(AvlInt v1, const AvlChar v2) { return v1.val() > v2.value; }

		bool operator >=(int v) const { return value >= v; }
		bool operator >=(char v) const { return value >= v; }
		bool operator >=(const AvlInt &v) const { return value >= v.val(); }
		bool operator >=(const AvlChar &v) const { return value >= v.value; }
		friend bool operator>=(int v1, const AvlChar v2) { return v1 >= v2.value; }
		friend bool operator>=(AvlInt v1, const AvlChar v2) { return v1.val() >= v2.value; }

		bool operator ==(int v) const { return value == v; }
		bool operator ==(char v) const { return value == v; }
		bool operator ==(const AvlChar &v) const { return value == v.value; }
		bool operator ==(const AvlInt &v) const { return value == v.val(); }
		friend bool operator==(char v1, const AvlChar v2) { return v1 == v2.value; }
		friend bool operator==(AvlInt v1, const AvlChar v2) { return v1.val() == v2.value; }

		bool operator !=(int v) const { return value != v; }
		bool operator !=(char v) const { return value != v; }
		bool operator !=(const AvlInt &v) const { return value != v.val(); }
		bool operator !=(const AvlChar &v) const { return value != v.value; }
		friend bool operator!=(int v1, const AvlChar v2) { return v1 != v2.value; }
		friend bool operator!=(AvlInt v1, const AvlChar v2) { return v1.val() != v2.value; }


		// value getter
		char val() const { return value; }

		// render function
		virtual void render()
		{
			unsigned int red = color() / 0x10000;
			unsigned int green = color() % 0x10000 / 0x100;
			unsigned int blue = color() % 0x100;
			glColor4f(red / 255.0, green / 255.0, blue / 255.0, 0.0);

			glRasterPos2f(x(), y());
			glutBitmapString(font().font(), (const unsigned char *)string(1, value).c_str());
		}

		void highlight() { 
			set_color(AVL_COLOR_HIGHLIGHT); 
		}
		void lowlight() { 
			set_color(AVL_COLOR_DEFAULT); 
		}

	private:
		char value;
}; // end of AvlChar

class array_entry {
public:
	AvlObject* obj;
	GLfloat x;
	GLfloat y;
};

class AvlIndex : public AvlObject {
public:
	// constructor 
	AvlIndex () {
		this->value = 0;
	}
	
	AvlIndex (int a) {
		this->value = a;
	}

	// copy constructor
	AvlIndex (const AvlIndex & that) {
		this->value = that.value;
	}

	// assignment operator
	const AvlIndex& operator= (const AvlIndex& that) {
		this->value = that.value;
		return *this;
	}

	// assignment operator for integer type
	const AvlIndex& operator= (int a) {
		this->value = a;
		return *this;
	}

	// assignment operator for AvlInt type
	const AvlIndex& operator= (AvlInt a) {
		this->value = a.val();
		return *this;
	}


	// unary plus and minus
	const AvlIndex& operator+() const { return *this; }
	const AvlIndex operator-() const
	{
		AvlIndex ret = *this;
		ret.value = -value;
		ret.update();
		return ret;
	}

	// pre-increment and post-increment
	const AvlIndex& operator++() { value++; update(); return *this; }
	const AvlIndex operator++(int)
	{
		AvlIndex ret = *this;
		value++;
		update();
		return ret;
	}

	// pre-decrement and post-decrement
	const AvlIndex& operator--() { value--; update(); return *this; }
	const AvlIndex operator--(int)
	{
		AvlIndex ret = *this;
		value--;
		update();
		return ret;
	}

	// binary plus
	const AvlIndex operator+(int v) const
	{
		AvlIndex ret = *this;
		ret.value += v;
		ret.update();
		return ret;
	}

	const AvlIndex operator+(char v) const
	{
		AvlIndex ret = *this;
		ret.value += (int) v;
		ret.update();
		return ret;
	}

	// binary plus
	const AvlIndex operator+(const AvlInt &v) const { return *this + v.val(); }
	const AvlIndex operator+(const AvlChar &v) const { return *this + v.val(); }
	const AvlIndex operator+(const AvlIndex &v) const { return *this + v.value; }
	friend const AvlIndex operator+(int v1, const AvlIndex &v2) { return v2 + v1; }
	friend const AvlIndex operator+(char v1, const AvlIndex &v2) { return v2 + v1; }
	friend const AvlIndex operator+(AvlInt v1, const AvlIndex &v2) { return v2 + v1.val(); }
	friend const AvlIndex operator+(AvlChar v1, const AvlIndex &v2) { return v2 + v1.val(); }

	// compound plus 
	const AvlIndex& operator+=(int v) { value += v; update(); return *this; }
	const AvlIndex& operator+=(char v) { value += v; update(); return *this; }
	const AvlIndex& operator+=(const AvlInt &v) { return *this += v.val(); }
	const AvlIndex& operator+=(const AvlChar &v) { return *this += v.val(); }
	const AvlIndex& operator+=(const AvlIndex &v) { return *this += v.value; }
	
	// binary minus
	const AvlIndex operator-(int v) const
	{
		AvlIndex ret = *this;
		ret.value -= v;
		ret.update();
		return ret;
	}

	const AvlIndex operator-(char v) const
	{
		AvlIndex ret = *this;
		ret.value -= v;
		ret.update();
		return ret;
	}

	const AvlIndex operator-(const AvlInt &v) const { return *this - v.val(); }
	const AvlIndex operator-(const AvlChar &v) const { return *this - v.val(); }
	const AvlIndex operator-(const AvlIndex &v) const { return *this - v.value; }
	friend const AvlIndex operator-(int v1, const AvlIndex &v2) { return -v2 + v1; }
	friend const AvlIndex operator-(char v1, const AvlIndex &v2) { return -v2 + v1; }
	friend const AvlIndex operator-(AvlInt v1, const AvlIndex &v2) { return -v2 + v1.val(); }
	friend const AvlIndex operator-(AvlChar v1, const AvlIndex &v2) { return -v2 + v1.val(); }

	// compound minus
	const AvlIndex& operator-=(int v) { value -= v; update(); return *this; }
	const AvlIndex& operator-=(char v) { value -= v; update(); return *this; }
	const AvlIndex& operator-=(const AvlInt &v) { return *this -= v.val(); }
	const AvlIndex& operator-=(const AvlIndex &v) { return *this -= v.value; }
	const AvlIndex& operator-=(const AvlChar &v) { return *this -= v.val(); }

	// multiplication
	const AvlIndex operator*(int v) const
	{
		AvlIndex ret = *this;
		ret.value *= v;
		ret.update();
		return ret;
	}
	
	const AvlIndex operator*(char v) const
	{
		AvlIndex ret = *this;
		ret.value *= v;
		ret.update();
		return ret;
	}
	
	const AvlIndex operator*(const AvlInt &v) const { return *this * v.val(); }
	const AvlIndex operator*(const AvlChar &v) const { return *this * v.val(); }
	const AvlIndex operator*(const AvlIndex &v) const { return *this * v.value; }
	friend const AvlIndex operator*(int v1, const AvlIndex &v2) { return v2 * v1; }
	friend const AvlIndex operator*(char v1, const AvlIndex &v2) { return v2 * v1; }
	friend const AvlIndex operator*(AvlInt v1, const AvlIndex &v2) { return v2 * v1.val(); }
	friend const AvlIndex operator*(AvlChar v1, const AvlIndex &v2) { return v2 * v1.val(); }

	
	// compound multiplication
	const AvlIndex& operator*=(int v) { value *= v; update(); return *this; }
	const AvlIndex& operator*=(char v) { value *= v; update(); return *this; }
	const AvlIndex& operator*=(const AvlInt &v) { return *this *= v.val(); }
	const AvlIndex& operator*=(const AvlChar &v) { return *this *= v.val(); }
	const AvlIndex& operator*=(const AvlIndex &v) { return *this *= v.value; }

	// devision
	const AvlIndex operator/(int v) const
	{
		AvlIndex ret = *this;
		ret.value /= v;
		ret.update();
		return ret;
	}

	const AvlIndex operator/(char v) const
	{
		AvlIndex ret = *this;
		ret.value /= v;
		ret.update();
		return ret;
	}

	const AvlIndex operator/(const AvlInt &v) const { return *this / v.val(); }
	const AvlIndex operator/(const AvlChar &v) const { return *this / v.val(); }
	const AvlIndex operator/(const AvlIndex &v) const { return *this / v.value; }
	friend const AvlIndex operator/(int v1, const AvlIndex &v2) { return v2 / v1; }
	friend const AvlIndex operator/(char v1, const AvlIndex &v2) { return v2 / v1; }
	friend const AvlIndex operator/(AvlInt v1, const AvlIndex &v2) { return v2 / v1.val(); }
	friend const AvlIndex operator/(AvlChar v1, const AvlIndex &v2) { return v2 / v1.val(); }

	// compound division
	const AvlIndex & operator/=(int v) { value /= v; update(); return *this; }
	const AvlIndex & operator/=(char v) { value /= v; update(); return *this; }
	const AvlIndex & operator/=(const AvlInt &v) { return *this /= v.val(); }
	const AvlIndex & operator/=(const AvlChar &v) { return *this /= v.val(); }
	const AvlIndex & operator/=(const AvlIndex &v) { return *this /= v.value; }

	// comparison operators
	bool operator <(int v) const { return value < v; }
	bool operator <(size_t v) const { return value < v; }
	bool operator <(char v) const { return value < v; }
	bool operator <(const AvlInt &v) const { return value < v.val(); }
	bool operator <(const AvlChar &v) const { return value < v.val(); }
	bool operator <(const AvlIndex &v) const { return value < v.val(); }
	friend bool operator<(int v1, const AvlIndex v2) { return v1 < v2.value; }
	friend bool operator<(char v1, const AvlIndex v2) { return v1 < v2.value; }
	friend bool operator<(AvlInt v1, const AvlIndex v2) { return v1.val() < v2.value; }
	friend bool operator<(AvlChar v1, const AvlIndex v2) { return v1.val() < v2.value; }

	bool operator <=(int v) const { return value <= v; }
	bool operator <=(char v) const { return value <= v; }
	bool operator <=(size_t v) const { return value <= v; }
	bool operator <=(const AvlInt &v) const { return value <= v.val(); }
	bool operator <=(const AvlChar &v) const { return value <= v.val(); }
	bool operator <=(const AvlIndex &v) const { return value <= v.value; }
	friend bool operator<=(int v1, const AvlIndex v2) { return v1 <= v2.value; }
	friend bool operator<=(char v1, const AvlIndex v2) { return v1 <= v2.value; }
	friend bool operator<=(AvlInt v1, const AvlIndex v2) { return v1.val() <= v2.value; }
	friend bool operator<=(AvlChar v1, const AvlIndex v2) { return v1.val() <= v2.value; }

	bool operator >(int v) const { return value > v; }
	bool operator >(char v) const { return value > v; }
	bool operator >(size_t v) const { return value > v; }
	bool operator >(const AvlInt &v) const { return value > v.val(); }
	bool operator >(const AvlChar &v) const { return value > v.val(); }
	bool operator >(const AvlIndex &v) const { return value > v.value; }
	friend bool operator>(int v1, const AvlIndex v2) { return v1 > v2.value; }
	friend bool operator>(char v1, const AvlIndex v2) { return v1 > v2.value; }
	friend bool operator>(AvlInt v1, const AvlIndex v2) { return v1.val() > v2.value; }
	friend bool operator>(AvlChar v1, const AvlIndex v2) { return v1.val() > v2.value; }

	bool operator >=(int v) const { return value >= v; }
	bool operator >=(char v) const { return value >= v; }
	bool operator >=(size_t v) const { return value >= v; }
	bool operator >=(const AvlInt &v) const { return value >= v.val(); }
	bool operator >=(const AvlChar &v) const { return value >= v.val(); }
	bool operator >=(const AvlIndex &v) const { return value >= v.value; }
	friend bool operator>=(int v1, const AvlIndex v2) { return v1 >= v2.value; }
	friend bool operator>=(char v1, const AvlIndex v2) { return v1 >= v2.value; }
	friend bool operator>=(AvlInt v1, const AvlIndex v2) { return v1.val() >= v2.value; }
	friend bool operator>=(AvlChar v1, const AvlIndex v2) { return v1.val() >= v2.value; }

	bool operator ==(int v) const { return value == v; }
	bool operator ==(char v) const { return value == v; }
	bool operator ==(size_t v) const { return value == v; }
	bool operator ==(const AvlInt &v) const { return value == v.val(); }
	bool operator ==(const AvlChar &v) const { return value == v.val(); }
	bool operator ==(const AvlIndex &v) const { return value == v.value; }
	friend bool operator==(int v1, const AvlIndex v2) { return v1 == v2.value; }
	friend bool operator==(char v1, const AvlIndex v2) { return v1 == v2.value; }
	friend bool operator==(AvlInt v1, const AvlIndex v2) { return v1.val() == v2.value; }
	friend bool operator==(AvlChar v1, const AvlIndex v2) { return v1.val() == v2.value; }

	bool operator !=(int v) const { return value != v; }
	bool operator !=(char v) const { return value != v; }
	bool operator !=(size_t v) const { return value != v; }
	bool operator !=(const AvlInt &v) const { return value != v.val(); }
	bool operator !=(const AvlChar &v) const { return value != v.val(); }
	bool operator !=(const AvlIndex &v) const { return value != v.value; }
	friend bool operator!=(int v1, const AvlIndex v2) { return v1 != v2.value; }
	friend bool operator!=(char v1, const AvlIndex v2) { return v1 != v2.value; }
	friend bool operator!=(AvlInt v1, const AvlIndex v2) { return v1.val() != v2.value; }
	friend bool operator!=(AvlChar v1, const AvlIndex v2) { return v1.val() != v2.value; }

	// value getter
	int val() const { return value; }

	// render function
	virtual void render()
	{
		unsigned int red = color() / 0x10000;
		unsigned int green = color() % 0x10000 / 0x100;
		unsigned int blue = color() % 0x100;
		glColor4f(red / 255.0, green / 255.0, blue / 255.0, 0.0);

		for (std::vector<array_entry>::iterator itr = array_set.begin(); itr != array_set.end(); itr ++) {
			glRasterPos2f(itr->x, itr->y);
			glutBitmapString(AvlFont().font(), (const unsigned char *)(name().c_str()));

		}
	}

	void highlight() { 
		set_color(AVL_COLOR_HIGHLIGHT); 
	}
	void lowlight() { 
		set_color(AVL_COLOR_DEFAULT); 
	}

	void add_array(AvlObject* obj, GLfloat a, GLfloat b) {
		for (size_t i = 0; i < array_set.size(); i ++) {
			if (array_set[i].x == a && array_set[i].y == b)
				return;
			if (obj == array_set[i].obj)
				return;
		}
		array_entry ae;
		ae.obj = obj;
		ae.x = a;
		ae.y = b;
		array_set.push_back(ae);
	}

private:
	int value;
	std::vector<array_entry> array_set;
	void update() { set_width(name.size() * font().width()); }


}; // end of AvlIndex



class AvlString: public AvlObject
{
	public:
		// constructor
	 	AvlString (const char *v, GLfloat x = 0, GLfloat y = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font)
		{
			value = v;
			update();
		}

	 	AvlString (char v, GLfloat x = 0, GLfloat y = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font)
		{
			value = v;
			update();
		}
	

		// destructor
		virtual ~AvlString() {}

		// assignment operator
		const AvlString & operator=(const char *v) { value = v; update(); return *this; }
		const AvlString & operator=(char v) { value = v; update(); return *this; }

		// << operator
		friend std::ostream& operator<<(std::ostream &os, const AvlString &v)
		{
			os << v.value;
			return os;
		}



		// binary plus
		const AvlString operator+(const char *v) const
		{
			AvlString ret = *this;
			ret.value += v;
			ret.update();
			return ret;
		}
	
		const AvlString operator+(char v) const
		{
			AvlString ret = *this;
			ret.value += v;
			ret.update();
			return ret;
		}

		// binary plus
		const AvlString operator+(const AvlChar &v) const { return *this +  v.val(); }
		const AvlString operator+(const AvlString &v) const 
		{ 
			AvlString ret = *this;
			ret.value += v.val();
			ret.update();
			return ret;
		}
		friend const AvlString operator+( char v1, const AvlString &v2 )
		{
			AvlString ret = v1;
			ret.value += v2.val();
			ret.update();
			return ret;
		}
		friend const AvlString operator+(const char *v1, const AvlString &v2)
		{
			AvlString ret = v1;
			ret.value += v2.val();
			ret.update();
			return ret;
		}
		friend const AvlString operator+(AvlChar v1, const AvlString &v2) { return v1.val() + v2; }

		// compound plus 
		const AvlString & operator+=(char v) { value += v; update(); return *this; }
		const AvlString & operator+=(const AvlChar &v) { return *this += v.val(); }
		const AvlString & operator+=(const char *v) { value += v; update(); return *this; }
		const AvlString & operator+=(const AvlString &v) { value += v.val(); update(); return *this; }



		// value getter
		std::string val() const { return value; }

		// render function
		virtual void render()
		{
			unsigned int red = color() / 0x10000;
			unsigned int green = color() % 0x10000 / 0x100;
			unsigned int blue = color() % 0x100;
			glColor4f(red / 255.0, green / 255.0, blue / 255.0, 0.0);

			glRasterPos2f(x(), y());
			glutBitmapString(font().font(), reinterpret_cast<const unsigned char *> (value.c_str()) );
		}

		void highlight() { 
			set_color(AVL_COLOR_HIGHLIGHT); 
		}
		void lowlight() { 
			set_color(AVL_COLOR_DEFAULT); 
		}

	private:
		void update() { set_width(value.size()*font().width()); }
		std::string value;
}; // end of AvlString


// ***NOTE!***: every element in this array is a pointer
template <typename T>
class AvlArray : public AvlObject
{
	public:
		// default constructor
		AvlArray(size_t size = 0, GLfloat x = 0, GLfloat y = 0,
				const AvlFont &font = GLUT_BITMAP_9_BY_15) : AvlObject(x, y, font), arr(size), index_x(size), index_y(size)
		{
			//std::cout << "Default constructor" << std::endl;
			
			for (int i = 0; i < arr.size(); i ++) {
				arr[i] = std::shared_ptr<T>(new T);
			}

			updateMutex = std::shared_ptr<std::mutex>(new std::mutex);
			toplevelArray = this;

			update();
		}

		// constructor with initializer list
		AvlArray(const std::initializer_list<T> &l) : arr(l.size()), index_x(l.size()), index_y(l.size())
		{
			//std::cout << "Initializer list constructor" << std::endl;

			typename std::initializer_list<T>::const_iterator src = l.begin();
			typename std::vector< std::shared_ptr<T> >::iterator dst = arr.begin();

			//!!!FIXME!!!: what this count for???
			int count = 0;
			while (src != l.end()) {
				*dst = std::shared_ptr<T>(new T(*src));
				src++;
				dst++;
				count++;
			}

			updateMutex = std::shared_ptr<std::mutex>(new std::mutex);
			toplevelArray = this;

			update();
		}

		// destructor
		virtual ~AvlArray() {}

		friend std::ostream& operator<<(std::ostream &os, const AvlArray<T> &ar)
		{
			typename std::vector< std::shared_ptr<T> >::const_iterator src = ar.arr.begin();

			while (src != ar.arr.end()) {
				os << **src;
				if (src != ar.arr.end() - 1)
					os << " ";
				src++;
			}

			return os;
		}

		// subscript operator
		T& operator[](size_t index) { return *arr[index]; }
		const T& operator[](size_t index) const { return *arr[index]; }

		// subscript operator with AvlIndex type
		T& operator[] (AvlIndex &index) {
			index.add_array(this, index_x[index.val()], index_y[index.val()] - 20);
			//render();
			return *arr[index.val()];
		}

		const T& operator[] (const AvlIndex &index) const {
			//index_variables.push_back(&index);
			//render();
			return *arr[index.val()];
		}

		// size
		size_t size() const { return arr.size(); }

		// sub-array
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
			ret.toplevelArray = toplevelArray;

			return ret;
		}

		// sub-array with AvlIndex
		AvlArray<T> subarray(const AvlIndex &a, const AvlIndex &b) const
		{
			size_t start = a.val();
			size_t end = b.val();
			AvlArray<T> ret(end - start);

			typename std::vector< std::shared_ptr<T> >::const_iterator src = arr.begin() + start;
			typename std::vector< std::shared_ptr<T> >::iterator dst = ret.arr.begin();

			while (src != arr.begin() + end) {
				*dst = *src;
				src++;
				dst++;
			}

			ret.updateMutex = updateMutex;
			ret.toplevelArray = toplevelArray;

			return ret;
		}

		// render
		virtual void render()
		{
			if (updateMutex->try_lock()) {
				update();
				updateMutex->unlock();
			}

			auxiliary_display();
			

			for (auto& v : arr) {
				v->render();
			}
		}

		void highlight()
		{
			for (auto& v: arr)
				v->highlight();
		}

		void lowlight()
		{
			for (auto& v: toplevelArray->arr)
				v->lowlight();
		}

		// swap two elements in an array
		void swap(size_t idx1, size_t idx2)
		{
			if (!isDisplaying()) {
				T tmp = *arr[idx1];
				*arr[idx1] = *arr[idx2];
				*arr[idx2] = tmp;
				return;
			}

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

		// swap two elements in an array using AvlIndex
		void swap(const AvlIndex &a, const AvlIndex &b)
		{
			size_t idx1 = a.val();
			size_t idx2 = b.val();

			if (!isDisplaying()) {
				T tmp = *arr[idx1];
				*arr[idx1] = *arr[idx2];
				*arr[idx2] = tmp;
				return;
			}

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
		void auxiliary_display() {
			// display "Content"
			unsigned int red = AVL_AUXILIARY_COLOR / 0x10000;
			unsigned int green = AVL_AUXILIARY_COLOR % 0x10000 / 0x100;
			unsigned int blue = AVL_AUXILIARY_COLOR % 0x100;
			glColor4f(red / 255.0, green / 255.0, blue / 255.0, 0.0);

			glRasterPos2f(x() - 9*(name().length() + 2), y());
			glutBitmapString(AvlFont().font(), (const unsigned char *)(std::string(this->name() + ":").c_str()));

			// display "Index" and index 
			glRasterPos2f(x() - 63, y() - 20);
			glutBitmapString(AvlFont().font(), (const unsigned char *)(std::string("Index: ").c_str()));

			for (int i = 0; i < arr.size(); i ++) {
				glRasterPos2f(index_x[i], index_y[i]);
				glutBitmapString(AvlFont().font(), (const unsigned char *)(std::to_string(i).c_str()));
			}
		}

		void update()
		{
			float digit_gat = 1.5;

			GLfloat w = -font().width();

			for (typename std::vector< std::shared_ptr<T> >::iterator it = arr.begin();
					it != arr.end(); it++) {
				if (it == arr.begin()) {
					(*it)->set_x( x() );
					(*it)->set_y( y() );
					(*it)->set_font( font() );
					w += (*it)->width() + font().width();
					
					index_x[it - arr.begin()] = x();
					index_y[it - arr.begin()] = y() - 20;
				}

				else {
					(*it)->set_x( (*(it-1))->x() + (*(it-1))->width() + digit_gat * font().width() );
					(*it)->set_y( (*(it-1))->y() );
					(*it)->set_font( (*(it-1))->font() );
					w += (*it)->width() + font().width();

					index_x[it - arr.begin()] = (*(it-1))->x() + (*(it-1))->width() + digit_gat * font().width();
					index_y[it - arr.begin()] = (*(it-1))->y() - 20;
				}

			}

			set_width(w);
		}

		std::vector< std::shared_ptr<T> > arr;
		std::shared_ptr<std::mutex> updateMutex;

		// array of the previous level, or parent level
		AvlArray<T> *toplevelArray;

		friend class AvlIndex;

		std::vector<GLfloat> index_x;
		std::vector<GLfloat> index_y;
}; //end of AvlArray

class AvlBool: public AvlObject
{
	public:
		// constructor
		AvlBool(bool v = false, GLfloat x = 0, GLfloat y = 0, void *font = GLUT_BITMAP_9_BY_15)
			: AvlObject(x, y, font)
		{
			value = v;
		}

		// destructor
		virtual ~AvlBool() {}

		// assignment operator
		const AvlBool& operator=(bool v) { value = v; return *this; }

		// << operator
		friend std::ostream& operator<<(std::ostream &os, const AvlBool &v)
		{
			os << v.value;
			return os;
		}

		// unary operator
		const AvlBool operator!() const
		{
			AvlBool ret = *this;
			ret.value = !value;
			return ret;
		}

		// comparison operators
		bool operator==(bool v) const { return value == v; }
		bool operator==(const AvlBool &v) const { return value == v.value; }
		friend bool operator==(bool v1, const AvlBool v2) { return v1 == v2.value; }

		bool operator!=(bool v) const { return value != v; }
		bool operator!=(const AvlBool &v) const { return value != v.value; }
		friend bool operator!=(bool v1, const AvlBool v2) { return v1 != v2.value; }

		bool operator&&(bool v) const { return value && v; }
		bool operator&&(const AvlBool &v) const { return value && v.value; }
		friend bool operator&&(bool v1, const AvlBool v2) { return v1 && v2.value; }

		bool operator||(bool v) const { return value || v; }
		bool operator||(const AvlBool &v) const { return value || v.value; }
		friend bool operator||(bool v1, const AvlBool v2) { return v1 || v2.value; }

		// value getter
		bool val() const { return value; }

		// render function
		virtual void render()
		{
			char display_value = 'T';
			if (value) { display_value = 'F'; }

			unsigned int red = color() / 0x10000;
			unsigned int green = color() % 0x10000 / 0x100;
			unsigned int blue = color() % 0x100;
			glColor4f(red / 255.0, green / 255.0, blue / 255.0, 0.0);

			glRasterPos2f(x(), y());
			string tmp = (value ? "T" : "F");
			glutBitmapString(font().font(), (const unsigned char *)tmp.c_str());
		}

		void highlight() { 
			set_color(AVL_COLOR_HIGHLIGHT); 
		}
		void lowlight() { 
			set_color(AVL_COLOR_DEFAULT); 
		}

	private:
		bool value;
}; // end of AvlBool


#endif // AVL_TYPES_H_
