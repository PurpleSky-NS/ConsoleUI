#include "SurfaceManager.h"
SurfaceManager SurfaceManager::instance;
SurfaceManager& SurfaceManager::GetInstance()
{
	return instance;
}

SurfaceManager::~SurfaceManager()
{
	CloseAll();
}

Surface* SurfaceManager::GetActive()const
{
	if (m_surfaces.empty())
		return nullptr;
	return m_surfaces.top();
}

void SurfaceManager::Start(Surface* surface)
{
	m_surfaces.push(surface);
	surface->OnStart();
	surface->Refresh();
}

void SurfaceManager::CloseTop()
{
	if (m_surfaces.empty())
		return;
	m_surfaces.top()->OnClose();
	delete m_surfaces.top();
	m_surfaces.pop();
	if (!m_surfaces.empty())
		m_surfaces.top()->Refresh();
}

void SurfaceManager::CloseAll()
{
	while (!m_surfaces.empty())
	{
		m_surfaces.top()->OnClose();
		delete m_surfaces.top();
		m_surfaces.pop();
	}
}

void SurfaceManager::PostEvent(char inputText)const
{
	if (m_surfaces.empty())
		return;
	m_surfaces.top()->PostEvent(inputText);
}

bool SurfaceManager::Empty()const
{
	return m_surfaces.empty();
}
