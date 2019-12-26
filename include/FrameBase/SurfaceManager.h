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

	/*获取当前栈顶（活动）界面*/
	Surface* GetActive()const;

	/*启动一个界面并且压入界面栈*/
	void Start(Surface* surface);

	/*关闭栈顶（活动）界面*/
	void CloseTop();

	/*关闭所有界面*/
	void CloseAll();

	/*向栈顶揭秘那传入事件字符*/
	void PostEvent(char inputText)const;

	/*界面栈是否为空*/
	bool Empty()const;

private:
	static SurfaceManager instance;
	std::stack<Surface*> m_surfaces;
};
