#pragma once

#include <cstdlib>
#include <vector>
#include <functional>
#include "Displayer.h"
#include "UIComponent.h"

/*一个界面*/
class Surface
{
public:

	/*设置刷新控制台的回调*/
	static void SetConsoleClearCall(std::function<void()> onConsoleClear);

	virtual ~Surface();

	/*刷新界面*/
	void Refresh();

	/*加入的顺序即为显示的优先级顺序*/
	void Add(UIComponent* com);

	/*删除后不会释放内存*/
	void Remove(UIComponent* com);

	/*全部删除，不会释放内存*/
	void RemoveAll();

	/*全部删除，会释放内存*/
	void Clear();

	/*传递事件字符*/
	virtual void PostEvent(char inputText);

	/*界面被添加到界面栈中回调*/
	virtual void OnStart() {}

	/*界面从界面栈中删除时回调*/
	virtual void OnClose() {}

private:
	static std::function<void()> m_onConsoleClear;
	UIComponent* m_onEventCom = nullptr;
	std::vector<UIComponent*> m_components;
};
