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
默认使用**（system("cls")）**来清空控制台，如果不在Windows上或者有其他方法，可以调用以下方法设置清空控制台的方法
```
static void SetConsoleClearCall(std::function<void()> onConsoleClear);
```
# 其他的内容晚些时候再更新
