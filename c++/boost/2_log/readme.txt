https://www.codenong.com/18419003/

Boost.Log linking errors under GNU/Linux

我想测试Boost.Log库。 不幸的是，我收到链接错误。

我使用Arch Linux，并通过内置的软件包管理器pacman安装了Boost头文件和库：

提升1.54.0-3
Boost-libs 1.54.0-2
通过g++ log.cpp -lboost_log -lpthread从官方站点编译简单示例时，出现以下错误：

1
2
3
log.cpp:(.text+0x42): undefined reference to `boost::log::v2s_mt_posix::trivial::logger::get()'
log.cpp:(.text+0x9b): undefined reference to `boost::log::v2s_mt_posix::trivial::logger::get()'
...

我已经阅读了为什么我的应用程序无法与Boost.Log链接？但是我无法解决链接错误。 它只是给我的提示是boost::log::v2s_mt_posix::trivial::logger::get()所在的库是静态链接的。 但是在目录/usr/lib/下，仅动态链接了扩展名为.so的Boost库。

也许有人知道这里出了什么问题。

谢谢。

 相关讨论
链接升压日志教程时链接器错误的可能重复(未定义的引用)
您需要定义BOOST_LOG_DYN_LINK：

1
g++ -DBOOST_LOG_DYN_LINK blog.cpp -lboost_log -lpthread

如果您使用的是cmake，则：

1
2
find_package(Boost REQUIRED COMPONENTS system log)
target_link_libraries(credential ${Boost_SYSTEM_LIBRARY} ${Boost_LOG_LIBRARY})
并使用：

1
#define BOOST_LOG_DYN_LINK 1


https://www.boost.org/doc/libs/1_81_0/libs/log/example/doc/tutorial_file.cpp
