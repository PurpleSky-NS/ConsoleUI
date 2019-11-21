#include "Surface.h"
Surface::~Surface()
{
	Clear();
}

void Surface::Refresh()
{
	system("cls");
	for (auto& i : m_components)
		i->OnPrint();
	cp.DisplayLine().Flush();
	cp.FlushMessages();
}

void Surface::PostEvent(char inputText)
{
	if (m_onEventCom != nullptr)
		m_onEventCom->OnEvent(inputText);
}

void Surface::Add(UIComponent* com)
{
	m_components.push_back(com);
	com->m_context = this;
	if (com->WillRegisterEvnet())
		m_onEventCom = com;
}

void Surface::Remove(UIComponent* com)
{
	for (auto i = m_components.begin(); i != m_components.end(); ++i)
		if (*i == com)
		{
			m_components.erase(i);
			break;
		}
}

void Surface::RemoveAll()
{
	m_components.clear();
}

void Surface::Clear()
{
	for (auto& i : m_components)
		delete i;
	m_components.clear();
}
