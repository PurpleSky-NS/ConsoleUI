#include "Displayer.h"
Displayer cp(std::cout);
void Displayer::Flush()
{
	m_out << m_buf.str() << std::flush;
	m_buf.str("");
	m_buf.clear();
}

void Displayer::FlushMessages()
{
	m_out << m_msgBuf.str() << std::flush;
	m_msgBuf.str("");
	m_msgBuf.clear();
}
