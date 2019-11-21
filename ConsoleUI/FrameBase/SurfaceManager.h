#pragma once

#include <stack>
#include "Surface.h"

/*外部需实现该函数作为整个程序的入口*/
extern void StartFrame();

class SurfaceManager
{
public:

	static SurfaceManager& GetInstance();

	~SurfaceManager();

	Surface* GetActive()const;

	void Start(Surface* surface);

	void CloseTop();

	void CloseAll();

	void PostEvent(char inputText)const;

	bool Empty()const;

private:
	static SurfaceManager instance;
	std::stack<Surface*> m_surfaces;
};
