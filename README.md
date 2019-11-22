# ConsoleUI
一个c++的控制台框架，帮助你快速开发出一个（好看的，好用的）控制台应用程序。

## 配置
配置很简单，在工程中包含ConsoleUI目录即可，不要更改里面的目录结构<br>
请确保你的IDE或者编译器开启了c++11标准，或者加入如下编译参数<br>
``` 
-std=c++11
```

## 包含
包含UIFrame.h文件即可，默认包含了整个框架工程
```c++
#include "UIFrame.h"
```

## 说明
整个工程文件使用UTF-8编码，但是工程与程序中使用了中文，根据IDE配置编译参数，或者自行转码都行

## 快速入门
该框架目前由几部分组成:<br>
* 框架核心
* 封装的一些界面<br>
<br>
- 每一个项目都是由界面(**Surface**)构成，而每个界面都是由UI组件构成(**UIComponent**)<br>
- 界面由界面管理器(**SurfaceManager**)所管理，该类是一个单例类<br>
- 输出由**Displayer**所封装，该类引用ostream来将输出内容写入，并且该类提供了缓冲机制，以及连缀函数的形式方便使用<br>
- 框架提供一个默认引用cout的Displayer：'cp'来进行控制台文本输出操作

### 界面
Surface为界面的基类，自定义的界面最好继承自该类<br>
<br>
该类作为UI组件的容器，有如下方法操作组件<br>
```
/*加入的顺序即为显示的优先级顺序*/
void Add(UIComponent* com);

/*删除后不会释放内存*/
void Remove(UIComponent* com);

/*全部删除，不会释放内存*/
void RemoveAll();

/*全部删除，会释放内存*/
void Clear();
```
子类可以重写如下方法<br>
```
/*传递事件字符*/
virtual void PostEvent(char inputText);

/*界面被添加到界面栈中回调*/
virtual void OnStart() {}

/*界面从界面栈中删除时回调*/
virtual void OnClose() {}
```
当用户有输入的时候，会通过PostEvent方法来传递用户输入字符(一次一个)<br>
默认的PostEvent方法，处理了**事件UI组件**的流程，如果不需要这套机制可以重写，或者在它的基础上做一些处理<br>
<br>
在任何地方都可以调用该方法来刷新界面
```
void Refresh();
```
默认使用(**system("cls")**)来清空控制台，如果不在Windows上或者有其他方法，可以调用以下方法设置清空控制台的方法
```
static void SetConsoleClearCall(std::function<void()> onConsoleClear);
```
<br>

### 界面管理器
SurfaceManager管理类，提供一个界面栈，并且是一个单例类<br>
用SurfaceManager的静态方法来获取界面管理器的实例：
```
static SurfaceManager& GetInstance();
```
可以使用如下方法操作界面管理器：
```
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
```
比如你可以使用下面的代码启动或者关闭你的一个界面：
```
SurfaceManage::GetInstance().Start(new MySurface);
SurfaceManage::GetInstance().CloseTop();
```
### UI组件
UIComponent为UI组件的基类，自定义的UI组件最好继承自该类<br>
子类可以重写下面的函数:<br>
* OnPrint:界面要求UI组件输出时调用
* OnEvent:事件传递机制，用户输入时调用，每个界面只有一个EventUI(获取事件的UI组件)
* WillRegisterEvnet:重写该方法并且返回true，以注册成为EventUI
```
virtual void OnPrint() {}

virtual void OnEvent(char inputText) {}

virtual bool WillRegisterEvnet()const { return false; }
```
**EventUI**:每个界面只有一个获取事件的UI组件，通过重写@WillRegisterEvnet方法来注册成为该界面唯一的接收事件的UI组件<br>
如果有多个UI组件要注册成为EventUI，那么按照界面添加UI组件的顺序选取最后一个作为UI组件<br>
事件通过Surface::PostEvent方法传递给EventUI，如果界面重写了这个方法就会使这套事件机制失效<br>

UI组件可以通过如下方法获取当前容器的界面:
```
Surface* GetContext()const;
```
### Displayer
这是一个方便文本输出的类，通过引用ostream，来讲输出内容输出到ostream中，内置一个缓冲区<br>
可以用如下方法构造:
```
/*通过引用一个ostream构造*/
Displayer(std::ostream& out);
```
以下是输出方法：<br>
```
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

template<class T>
/*输出到Message缓冲区中，每条消息为一行，并且在前面加入标志*/
inline Displayer& DisplayOK(const T& content);

/*输出到Message缓冲区中，每条消息为一行，并且在前面加入标志*/
template<class T>
inline Displayer& DisplayError(const T& content);

/*设置ios::flags @ostream::setf(std flag[eg.std::ios::scientific])*/
inline Displayer& DisplayIOSFlag(int mask);

/*取消ios::flags @ostream::unsetf(std flag[eg.std::ios::scientific])*/
inline Displayer& UnDisplayIOSFlag(int mask);
```
使用如下方法将输出内容刷新到ostream中，如果不调用则会一直缓存：
* Flush:将除了DisplayError和DisplayerOK的内容刷新到ostream中
* FlushMessage:将DisplayError和DisplayerOK的内容刷新到ostream中，并加上标志以及
```
void Flush();

void FlushMessages();

```
默认提供一个引用cout的Displayer:'**cp**'，你可以在UI组件中使用它。
### 入口函数
该框架提供一个入口函数，用户需要定义该函数：
```
void StartFrame();
```
在该函数中你需要启动至少一个界面，否则程序将直接退出。
### 定义一个UI组件
首先你需要你的界面类继承自Surface
```
class MyCom :public UIComponent
{
};
```
通过在OnPrint使用cp来实现输出<br>
注意：
* 如果需要立即刷入ostream则可以调用Flush，否则可以不调用<br>
* 尽量不要调用FlushMessage，界面会在刷新时自动调用这个方法，否则可能输出混乱
```
/*该组件只有显示功能*/
class MyCom :public UIComponent
{
  void OnPrint()
  {
    cp.DisplayTab(2).DisplayLine("你好界面")
      .DisplaySpace(4).DisplayLine("这是一个UI组件的输出")
      .DisplaySpace(4).DisplayLine("这是第二行")
      .DisplaySpace(4).Display("输出小数：").DisplayLine(1.523)
      .DisplaySpace(4).Display("输出2位小数：").DisplayPrecision(1.523, 2).DisplayLine()
      .DisplaySpace(4).Display("输出科学计数法小数：").DisplayScientific(1.523).DisplayLine()
      .DisplaySpace(4).Display("输出对齐：").DisplayRoom("Hello", 10).DisplayRoom(10).Display("World").DisplayLine()
      .DisplaySpace(4).Display("输出左对齐：").DisplayLeft().DisplayRoom("Hello", 10).DisplayRoom(10).Display("World").DisplayLine().DisplayRight()
      .DisplaySpace(4).Display("输出IOS flag：").Display(std::fixed).Display(std::setprecision(2)).Display(std::left).Display(std::setw(10)).Display("Hello?").UnDisplayIOSFlag(std::ios::fixed).DisplayLine();
  }
};
```
可以注册这个UI组件来接受事件
```
/*该组件会将输入内容显示出来*/
class EventCom :public UIComponent
{
  void OnPrint()
  {
    cp.Display("你的输入：").DisplayLine(m_text);
  }

  bool WillRegisterEvnet()const
  {
    return true;
  }

  void OnEvent(char input)
  {
    m_text += input;
  }

private:
  std::string m_text;
};
```
### 定义一个界面
首先你需要你的界面类继承自Surface
```
class MySurface : public Surface
{
};
```
为你的界面添加UI组件：
```
class MySurface : public Surface
{
class MyCom;
class EventCom;
public:
	MySurface()
	{
		Add(new MyCom);
		Add(new EventCom);
	}
};
```
完整界面测试代码：
```
//TestSurface.h
#pragma once

#include "UIFrame.h"

class MySurface : public Surface
{
	class MyCom :public UIComponent
	{
		void OnPrint()
		{
			cp.DisplayTab(2).DisplayLine("你好界面")
				.DisplaySpace(4).DisplayLine("这是一个UI组件的输出")
				.DisplaySpace(4).DisplayLine("这是第二行")
				.DisplaySpace(4).Display("输出小数：").DisplayLine(1.523)
				.DisplaySpace(4).Display("输出2位小数：").DisplayPrecision(1.523, 2).DisplayLine()
				.DisplaySpace(4).Display("输出科学计数法小数：").DisplayScientific(1.523).DisplayLine()
				.DisplaySpace(4).Display("输出对齐：").DisplayRoom("Hello", 10).DisplayRoom(10).Display("World").DisplayLine()
				.DisplaySpace(4).Display("输出对齐：").DisplayRoom("Hi", 10).DisplayRoom(10).Display("Sky").DisplayLine()
				.DisplaySpace(4).Display("输出左对齐：").DisplayLeft().DisplayRoom("Hello", 10).DisplayRoom(10).Display("World").DisplayLine().DisplayRight()
				.DisplaySpace(4).Display("输出左对齐：").DisplayLeft().DisplayRoom("Hi", 10).DisplayRoom(10).Display("Sky").DisplayLine().DisplayRight()
				.DisplaySpace(4).Display("输出IOS flag：").Display(std::fixed).Display(std::setprecision(2)).Display(std::left).Display(std::setw(10)).Display("Hello ").Display(1.523).UnDisplayIOSFlag(std::ios::fixed).DisplayLine();
		}
	};
	class EventCom :public UIComponent
	{
		void OnPrint()
		{
			cp.Display("你的输入：").DisplayLine(m_text);
		}

		bool WillRegisterEvnet()const
		{
			return true;
		}

		void OnEvent(char input)
		{
			m_text += input;
			GetContext()->Refresh();//获取容器界面并且刷新界面
		}

	private:
		std::string m_text;
	};

public:
	MySurface()
	{
		/*必须时动态分配的，最后会自动释放内存*/
		Add(new MyCom);
		Add(new EventCom);
	}
};
```
### 运行程序
在源文件编写程序入口，并且启动测试界面
```
//MyMain.cpp
#include "UIFrame.h"
#include "TestSurface.h"

void StartFrame()
{
	SurfaceManager::GetInstance().Start(new MySurface);
}
```
**注意：**
没有使用命令行输入缓冲，使用的是 *_getch* 函数传递事件字符，所以，退格字符等事件需要自己编写<br>

![运行程序](/image/Test0.png) <br>
![输入](/image/Test1.png)

### 内置的界面类
提供了一些写好的界面类以便用户快速开发
#### MenuSurface
一个菜单界面，用户只需要继承自该类并且做相关配置即可使用<br>
1. 继承自该类并且调用父类构造函数指定标题
```
class MyMenu :public MenuSurface
{
public:
	MyMenu() :
		MenuSurface("我的标题")
	{}
};
```
2. 调用MenuSurface的该方法添加菜单子项，第一个参数是菜单的标题，第二个参数是一个回调函数（函数返回值为void，参数为空）
```
void AddItem(const std::string& itemName, std::function<void()> onSelected);
```
* 测试代码如下：
```
//TestMenu.h
#pragma once

#include "UIFrame.h"

class MyMenu :public MenuSurface
{
public:
	MyMenu() :
		MenuSurface("我的标题")//必须指定标题
	{
		/*添加子项，传入名称和一个lambda表达式*/
		AddItem("打印一个错误信息", [&]()
			{
				cp.DisplayError("一个错误信息");
				Refresh();
			});
		AddItem("打印一个提示信息", [&]()
			{
				cp.DisplayOK("一个提示信息");
				Refresh();
			});
	}
};

//MyMain.cpp
#include "TestMenu.h"
void StartFrame()
{
	SurfaceManager::GetInstance().Start(new MyMenu);
}
```
![菜单](/image/TestMenu0.png) <br>
![选择选项](/image/TestMenu1.png)

# 其他的内容晚些时候再更新使用的
