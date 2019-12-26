#include "Displayer.h"
Displayer cp(std::cout);
Displayer& Displayer::Flush()
{
	m_out << m_buffers[m_bufIndex].str() << std::flush;
	m_buffers[m_bufIndex].str("");
	return *this;
}

Displayer& Displayer::Flush(BufferIndexType bufIndex)throw(NoSuchBufferException)
{
	CheckBufferPar(bufIndex);
	m_out << m_buffers[DEFAULT_BUFFER_INDEX].str() << std::flush;
	m_buffers[DEFAULT_BUFFER_INDEX].str("");
	return *this;
}

Displayer& Displayer::Flush(const std::string& bufName)throw(NoSuchBufferException)
{
	CheckBufferPar(bufName);
	m_out << m_buffers[m_mappingIndex[bufName]].str() << std::flush;
	m_buffers[m_mappingIndex[bufName]].str("");
	return *this;
}

Displayer& Displayer::FlushDefault()
{
	m_out << m_buffers[DEFAULT_BUFFER_INDEX].str() << std::flush;
	m_buffers[DEFAULT_BUFFER_INDEX].str("");
	return *this;
}

Displayer& Displayer::FlushMessages()
{
	m_out << m_buffers[MESSAGE_BUFFER_INDEX].str() << std::flush;
	m_buffers[MESSAGE_BUFFER_INDEX].str("");
	return *this;
}

Displayer& Displayer::FlushTo(BufferIndexType bufIndex) throw(NoSuchBufferException)
{
	CheckBufferPar(bufIndex);
	m_buffers[bufIndex] << m_buffers[m_bufIndex].str() << std::flush;
	m_buffers[m_bufIndex].str("");
	return *this;
}

Displayer& Displayer::FlushTo(const std::string& bufName) throw(NoSuchBufferException)
{
	CheckBufferPar(bufName);
	m_buffers[m_mappingIndex[bufName]] << m_buffers[m_bufIndex].str() << std::flush;
	m_buffers[m_bufIndex].str("");
	return *this;
}

std::string Displayer::FlushOut()
{
	std::string&& data = m_buffers[m_bufIndex].str();
	m_buffers[m_bufIndex].str("");
	return data;
}

std::string Displayer::GetBuffer()
{
	return m_buffers[m_bufIndex].str();
}
