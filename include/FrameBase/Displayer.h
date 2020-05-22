#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <unordered_map>

/*引用ostream对象的输出类
 *可以连缀输出，Display与DisplayLine的模板参数只要重载了ostream<<运算符就可以传入输出*/
class Displayer
{
public:
	/*无该缓冲流*/
	class NoSuchBufferException :public std::runtime_error
	{
	public:
		NoSuchBufferException() = default;
		NoSuchBufferException(const std::string& msg) :
			runtime_error(msg) {}
	};

	using BufferIndexType = unsigned char;

	/*总共可以容纳的流的数量*/
	const BufferIndexType MAX_BUFFER_COUNT = 0xff;
	/*默认流索引*/
	const BufferIndexType DEFAULT_BUFFER_INDEX = 0;
	/*信息流索引*/
	const BufferIndexType MESSAGE_BUFFER_INDEX = 1;
	/*拼接流索引*/
	const BufferIndexType CONCAT_BUFFER_INDEX = 2;

	Displayer(std::ostream& out) :
		m_out(out), m_buffers(3), m_bufIndex(0) {}

	template<class T>
	/*输出到ostream流中，可以是数据(ostream支持的)，也可以是flags(如std::fixed,std::setw等)*/
	inline Displayer& Display(const T& content);

	/* 显示指定位数的小数，输出完成自动清除标志位，@std::fixed @std::setprecision */
	inline Displayer& DisplayPrecision(double v, size_t precision);

	/* 显示科学计数法的小数，输出完成自动清除标志位，@std::scientific */
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

	/*设置ios::flags @ostream::setf(std flag[eg.std::ios::scientific])*/
	inline Displayer& DisplayIOSFlag(std::ios::fmtflags mask);

	/*取消ios::flags @ostream::unsetf(std flag[eg.std::ios::scientific])*/
	inline Displayer& UnDisplayIOSFlag(std::ios::fmtflags mask);

	template<class T>
	/* 输出到Message缓冲区中，每条消息为一行，并且在前面加入标志*/
	inline Displayer& DisplayOK(const T& content);

	/*输出到Message缓冲区中，每条消息为一行，并且在前面加入标志*/
	template<class T>
	inline Displayer& DisplayError(const T& content);

	/*创建一个缓冲流，自定义的流编号从2开始，做多只能有 @MAX_BUFFER_COUNT 个流*/
	inline Displayer& CreateDisplayBuffer();

	/*创建一个缓冲流，除了流编号同上外，为之命名，多个流不应该有同一个名字，做多只能有 @MAX_BUFFER_COUNT 个流*/
	inline Displayer& CreateDisplayBuffer(const std::string& bufName);

	/*是否存在该流*/
	inline bool ExistBuffer(BufferIndexType bufIndex);

	/*是否存在该流*/
	inline bool ExistBuffer(const std::string& bufName);

	/*是否存在该流*/
	inline BufferIndexType CurrentBuffer()const;

	/*切换至缓冲流*/
	inline Displayer& Switch(BufferIndexType bufIndex);

	/*切换至缓冲流*/
	inline Displayer& Switch(const std::string& bufName);

	/*切换至默认输出缓冲流*/
	inline Displayer& SwitchDefault();

	/*切换至信息输出缓冲流*/
	inline Displayer& SwitchMessage();

	/*切换至拼接输出缓冲流*/
	inline Displayer& SwitchConcat();

	/*刷出缓冲流*/
	Displayer& Flush();

	/*刷出特定缓冲流*/
	Displayer& Flush(BufferIndexType bufIndex);

	/*刷出特定缓冲流*/
	Displayer& Flush(const std::string& bufName);

	/*刷出信息缓冲流*/
	Displayer& FlushDefault();

	/*刷出信息缓冲流*/
	Displayer& FlushMessages();

	/*向某个流刷出当前缓冲流*/
	Displayer& FlushTo(BufferIndexType bufIndex);

	/*向某个流刷出当前缓冲流*/
	Displayer& FlushTo(const std::string& bufName);

	/*向外刷出当前缓冲流*/
	std::string FlushOut();

	/*获取当前缓冲流内容，但是不刷出*/
	std::string GetBuffer();

private:
	std::ostream& m_out;
	BufferIndexType m_bufIndex;
	std::vector<std::ostringstream> m_buffers;
	std::unordered_map <std::string, BufferIndexType> m_mappingIndex;

	inline bool CanCreate()const;

	inline void CheckBufferPar(BufferIndexType bufIndex);

	inline void CheckBufferPar(const std::string& bufName);
};

/*引用cout，ConsolePrint*/
extern Displayer cp;

template<class T>
inline Displayer& Displayer::Display(const T& content)
{
	m_buffers[m_bufIndex] << content;
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayRoom(const T& content, size_t width)
{
	m_buffers[m_bufIndex] << std::setw(width) << content;
	return *this;
}

inline Displayer& Displayer::DisplayPrecision(double v, size_t precision)
{
	m_buffers[m_bufIndex] << std::fixed << std::setprecision(precision) << v << std::setprecision(6);
	m_buffers[m_bufIndex].unsetf(m_buffers[m_bufIndex].fixed);
	return *this;
}

inline Displayer& Displayer::DisplayScientific(double v)
{
	m_buffers[m_bufIndex] << std::scientific << v;
	m_buffers[m_bufIndex].unsetf(m_buffers[m_bufIndex].scientific);
	return *this;
}

inline Displayer& Displayer::DisplayRoom(size_t width)
{
	m_buffers[m_bufIndex] << std::setw(width);
	return *this;
}

inline Displayer& Displayer::DisplayLeft()
{
	m_buffers[m_bufIndex] << std::left;
	return *this;
}

inline Displayer& Displayer::DisplayRight()
{
	m_buffers[m_bufIndex] << std::right;
	return *this;
}

inline Displayer& Displayer::DisplayLine()
{
	m_buffers[m_bufIndex] << '\n';
	return *this;
}

inline Displayer& Displayer::DisplayTab(unsigned count)
{
	m_buffers[m_bufIndex] << std::string(count, '\t');
	return *this;
}

inline Displayer& Displayer::DisplaySpace(unsigned count)
{
	m_buffers[m_bufIndex] << std::string(count, ' ');
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayLine(const T& content)
{
	m_buffers[m_bufIndex] << content << '\n';
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayOK(const T& content)
{
	m_buffers[MESSAGE_BUFFER_INDEX] << "√ " << content << '\n';
	return *this;
}

template<class T>
inline Displayer& Displayer::DisplayError(const T& content)
{
	m_buffers[MESSAGE_BUFFER_INDEX] << "× " << content << '\n';
	return *this;
}

inline Displayer& Displayer::DisplayIOSFlag(std::ios::fmtflags mask)
{
	m_buffers[m_bufIndex].setf(mask);
	return *this;
}

inline Displayer& Displayer::UnDisplayIOSFlag(std::ios::fmtflags mask)
{
	m_buffers[m_bufIndex].unsetf(mask);
	return *this;
}

inline Displayer& Displayer::CreateDisplayBuffer()
{
	if (CanCreate())
		m_buffers.push_back(std::ostringstream());
	return *this;
}

inline Displayer& Displayer::CreateDisplayBuffer(const std::string& bufName)
{
	if (CanCreate())
	{
		m_buffers.push_back(std::ostringstream());
		m_mappingIndex.insert({ bufName,(BufferIndexType)(m_buffers.size() - 1) });
	}
	return *this;
}

inline bool Displayer::ExistBuffer(BufferIndexType bufIndex)
{
	return bufIndex < m_buffers.size();
}

inline bool Displayer::ExistBuffer(const std::string& bufName)
{
	return m_mappingIndex.find(bufName) != m_mappingIndex.end();
}

inline Displayer::BufferIndexType Displayer::CurrentBuffer() const
{
	return m_bufIndex;
}

inline Displayer& Displayer::Switch(BufferIndexType bufIndex)
{
	CheckBufferPar(bufIndex);
	m_bufIndex = bufIndex;
	return *this;
}

inline Displayer& Displayer::Switch(const std::string& bufName)
{
	CheckBufferPar(bufName);
	m_bufIndex = m_mappingIndex[bufName];
	return *this;
}

inline Displayer& Displayer::SwitchDefault()
{
	m_bufIndex = DEFAULT_BUFFER_INDEX;
	return *this;
}

inline Displayer& Displayer::SwitchMessage()
{
	m_bufIndex = MESSAGE_BUFFER_INDEX;
	return *this;
}

inline Displayer& Displayer::SwitchConcat()
{
	m_bufIndex = CONCAT_BUFFER_INDEX;
	return *this;
}

inline bool Displayer::CanCreate() const
{
	return m_buffers.size() != MAX_BUFFER_COUNT;
}

inline void Displayer::CheckBufferPar(BufferIndexType bufIndex)
{
	if (!ExistBuffer(bufIndex))
	{
		BufferIndexType prev = m_bufIndex;
		std::string&& str = Switch(CONCAT_BUFFER_INDEX).Display("没有编号为 ").Display((size_t)bufIndex).Display(" 的流").FlushOut();
		m_bufIndex = prev;
		throw NoSuchBufferException(str);
	}
}

inline void Displayer::CheckBufferPar(const std::string& bufName)
{
	if (!ExistBuffer(bufName))
		throw NoSuchBufferException("没有名称为 " + bufName + " 的流");
}
