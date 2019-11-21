#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>

/*引用ostream对象的输出类
 *可以连缀输出，Display与DisplayLine的模板参数只要重载了ostream<<运算符就可以传入输出*/
class Displayer
{
public:

	Displayer(std::ostream& out) :
		m_out(out) {}

	template<class T>
	/*输出到ostream流中，可以是数据(ostream支持的)，也可以是flags(如std::fixed,std::setw等)*/
	inline Displayer& Display(const T& content);

	/* 显示指定位数的小数，@std::fixed @std::setprecision */
	inline Displayer& DisplayPrecision(double v, size_t precision);

	/* 显示科学计数法的小数，@std::scientific */
	inline Displayer& DisplayScientific(double v);

	/* 预留对齐空间，@std::setw */
	inline Displayer& DisplayRoom(size_t width);

	template<class T>
	/* 对齐显示，@std::setw */
	inline Displayer& DisplayRoom(const T& content, size_t width);

	/* 左对齐显示，@std::ios::left */
	inline Displayer& DisplayLeft();

	/* 右对齐显示，@std::ios::right */
	inline Displayer& DisplayRight();

	inline Displayer& DisplayLine();

	template<class T>
	inline Displayer& DisplayLine(const T& content);

	inline Displayer& DisplayTab(unsigned count = 1);

	inline Displayer& DisplaySpace(unsigned count = 1);

	template<class T>
	inline Displayer& DisplayOK(const T& content);

	template<class T>
	inline Displayer& DisplayError(const T& content);

	/*设置ios::flags @ostream::setf(std flag[eg.std::ios::scientific])*/
	inline Displayer& DisplayIOSFlag(int mask);

	/*取消ios::flags @ostream::unsetf(std flag[eg.std::ios::scientific])*/
	inline Displayer& UnDisplayIOSFlag(int mask);

	void Flush();

	void FlushMessages();

private:
	std::stringstream m_buf;
	std::stringstream m_msgBuf;
	std::ostream& m_out;
};
/*引用cout，ConsolePrint*/
extern Displayer cp;

template<class T>
inline Displayer& Displayer::Display(const T& content)
{
	m_buf << content;
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayRoom(const T& content, size_t width)
{
	m_buf << std::setw(width) << content;
	return *this;
}

inline Displayer& Displayer::DisplayPrecision(double v, size_t precision)
{
	m_buf << std::fixed << std::setprecision(precision) << v << std::setprecision(6);
	m_buf.unsetf(m_buf.fixed);
	return *this;
}

inline Displayer& Displayer::DisplayScientific(double v)
{
	m_buf << std::scientific << v;
	m_buf.unsetf(m_buf.scientific);
	return *this;
}

inline Displayer& Displayer::DisplayRoom(size_t width)
{
	m_buf << std::setw(width);
	return *this;
}

inline Displayer& Displayer::DisplayLeft()
{
	m_buf.setf(m_buf.left);
	return *this;
}

inline Displayer& Displayer::DisplayRight()
{
	m_buf.setf(m_buf.right);
	return *this;
}

inline Displayer& Displayer::DisplayLine()
{
	m_buf << '\n';
	return *this;
}

inline Displayer& Displayer::DisplayTab(unsigned count)
{
	m_buf << std::string(count, '\t');
	return *this;
}

inline Displayer& Displayer::DisplaySpace(unsigned count)
{
	m_buf << std::string(count, ' ');
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayLine(const T& content)
{
	m_buf << content << '\n';
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayOK(const T& content)
{
	m_msgBuf << "√ " << content << '\n';
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayError(const T& content)
{
	m_msgBuf << "× " << content << '\n';
	return *this;
}

inline Displayer& Displayer::DisplayIOSFlag(int mask)
{
	m_buf.setf(mask);
	return *this;
}

inline Displayer& Displayer::UnDisplayIOSFlag(int mask)
{
	m_buf.setf(mask);
	return *this;
}
