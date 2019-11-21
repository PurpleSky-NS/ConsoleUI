# ConsoleUI
一个c++的控制台框架，帮助你快速开发出一个（好看的，好用的）控制台应用程序。

## 配置
配置很简单，在工程中包含ConsoleUI目录即可，不要更改里面的目录结构<br>
所有的源文件都在对应的头文件目录下，根据各个IDE要求进行配置<br>
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