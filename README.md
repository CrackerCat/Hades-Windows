#### 方案：

##### Kernel

&emsp;&emsp;x64内核探针粗糙分为两类技术方案：

- 第一种基于Intel-x/d虚拟化技术，绕过PG保护做花式Hook，功能强大-系统无痕。
- 第二种基于微型过滤框架和注册回调，兼容性好/快速开发/接口完善。

&emsp;&emsp;项目采用过滤驱动+注册回调，这种方案中规中矩。后续可能会集成VT接口，优先支持EPT HOOK和寄存器/内存数据探测，详细的技术实现请跳转具体项目查阅ReadMe。

&emsp;&emsp;示例适用Win7/Win10 x64下内核态数据采集，需要测试与系统的兼容性，可以选择不开启内核采集，x32系统版本需要稍作修改。

##### User

&emsp;&emsp;系统敏感数据采集和ETW事件采集。

#### 框架:
![image](image-windows.png)

<center><h3>v1.0</h3></center>

##### WFP(开发中)：

| 网络层       | 描述  |
| :--------- | :---- |
| Established层 | ProcessInfo |
| 传输层     | TCP - UDP |
| 网络层   | IP |
| 数据链路层 | OS >= Windows10 |

Json配置流量规则(未生效):

 ```
 (流量规则)
 Json:
  {
  Bypass:
 	1 - 单要素：目标 port 或者 ip 
 	2 - 双要素：目标 ip:port  
 	3 - 重定向标志位 - 暂时不开启(流量隔离)
  }
 ```

##### 内核回调上抛事件：

| 事件   | 描述  |
| :----- | :---- |
| 进程   | 进程创建 - 销毁 - 进程数据 - 签名 |
| 线程   | 线程创建 - 销毁 - 线程数据  |
| 注册表 | 删除 -  修改 - 枚举 - 重命名等（缺少具体的包解析） |
| 模块 | DLL -  驱动 |
| 会话 | 用户登录/退出/Session切换 |
| WMI    | 待定(应用层etw实现) |
| 文件 | 文件读写访问  OS <= Windows7  (Windows10 对象回调(文件对象)会有几率触发PG) |

Json配置内核上抛事件管理(未生效):

```
{
    1. 添加进程白名单，允许从内核态过滤控某进程数据
    2. xxxxx
}
```

##### 内核接口采集事件：

| 事件       | 描述  |
| :----------- | :---- |
| 进程 - 线程 - 模块 - 内存 | 系统已运行的进程 - 线程 - 模块 - 进程内存 - 应用层钩子检测(待定) |
| IDT     | 系统IDT - (原始偏移 - 内存已加载偏移)  HOOK检测 |
| MouseKeyBoard | 鼠标键盘 Hook检测 |
| DpcTimer | 遍历系统 DpcTimer |
| Hive | hive注册表 - 开发中 |
| Ntfs | ntfs文件解析 - 开发中 |
| Network | Nsi提取IP:PORT |
| Fsd | FastFat/ntfs HOOK检测 |
| SSDT     | 系统SSDT - (原始偏移 - 内存已加载偏移) HOOK检测 |
| GDT | 系统GDT - (原始偏移 - 内存已加载偏移) HOOK检测 |
| 驱动     | 系统已加载的驱动 |
| 回调检测   | 枚举系统注册的回调 |

##### GRPC：

Windows对于很多第三方生态逐步容纳，Grpc github cmake编译仍会出现很多问题，最好的办法:

```
vcpkg install grpc
```

&emsp;&emsp;配置vs2019 工具 --> 选项 --> NuGet管理即可，详细可以参考网上教程，注意vcpkg 安装的是release grpc，所以debug模式调试会有问题。

C++ Grpc请参考官方文档：https://grpc.io/docs/languages/cpp/basics/

**See Code: grpc.h grpc.cpp**

#### 规划：

&emsp;&emsp;**项目处于入门级，需要花时间打磨和重构。内核上抛和WFP技术详细请跳转子项目页面，ReadMe查看对应的功能实现。关于Minifilter驱动，仅文件监控不会引入该模块，应用层ETW日志弥补，后期如果有文件隔离相关规划将会纳入。**

&emsp;&emsp;**它并不是以产品形态诞生，希望日后它变得更加灵活更健壮，以插件提供lib/dll，集成至任意终端产品，包括办公软件 - 游戏音频，提供更多软件第三方安全能力建设和数据检测。**

##### v2.0

- Rootkit完善

- 兼容Win7/Win10 x64大部分版本，稳定性测试
- 指定进程授权非隔离分析 - 类沙箱做inlinehook来监控运行周期
- electron + vue 跨平台终端简单界面

##### v3.0

- 流量隔离：进程指定重定向和bypass
- 敏感行为备份：如Cmd - Powershell - Vbs等下载文件，Minifilter自动备份至指定目录。

#### 参考：

- Github开源Rootkit工具，但不局限于工具。
- 看雪论坛帖子
- OpenEdr & Netfilter SDK & Sandboxie框架模型

```c++
std::cout << "项目将零散代码组织到一起，业余投入精力并不多。部分cpp可能以前学习中编写，遗憾的是时间太久，忘记了具体引用的项目，部分代码中有参考github_url，有兴趣可以去学习一番。" << std::endl
```
