### C++
#### 命名规则
* 文件命名规则
    * 文件名全部小写，可以含下划线或连字符，按项目约定命名,且尽量保证文件名明确。比如：
        cmd_save_player_info_class.cpp
        my_use_full_class.cpp
    * 定义类的文件名一般是成对出现，如：
        foo_bar.h   
        foo_bar.cc  
    * 若是类中含大量内联函数，我们还可使用-ini.h文件，使之文件内容更加清晰，于是又如：
        url_table.h     
        url_table.cc  
        url-table-ini.h

* 类命名规则
    * 类型命名每个单词首字母大写，不含下划线，以名词形式。比如： 
        MyPalyerManage
    * 所有类型命名一样，类结构体，枚举，类定义都是如此，比如：
        MyExcitingEnum

* 变量命名规则  
    * 变量名一律小写，单词用下划线相连，例如：
        int player_id
        string table_name 
    * 特殊的是类成员变量，后跟下划线区别普通变量，比如： 
        player_name_
        player_id_
    * 全局变量则以g_  开头，比如:
        g_system_time
    * 当然结构体成员变量还是和普通变量一样,比如：
        string name
        int num_entries
* 常量命名规则
    * k后面跟大写字母开头的单词，比如：
        const int kDaysInAWeek=7
        const string kCompanyName="Tecent"
* 函数命名规则
    * 常规函数每个单词首字母大写，使用命令式语气，比如：
        OpenFile()
        CheckFileName()
    * 存取函数或短小的内联函数使用小写加下划线，且与访问变量相吻合，比如：
        set_num_errors();
        class Player{ 
            public:   
                void set_player_id(const int player_id){return player_id_=player_id;}   
                int get_player_id() 
                const{return player-id_;} 
            private:   
                int palyer_id_;
        };
* 名字空间命名
    * 命名空间全小写，并基于项目名称和目录结构，比如：
        google_awesome_project
* 枚举命名规则
    * 枚举类名属于类型名，按类命名，枚举值全大写加下划线，比如：
        ENUM_NAME
* 宏变量命名规则
    * 如果你一定要用到宏，全大写加下划线，比如：
    /#define PI_ROUND 3.0


#### 编译
* MingGW
    * https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/
* bazel
    * https://github.com/bazelbuild/bazel/releases
* MSYS2
    * http://repo.msys2.org/distrib/msys2-x86_64-latest.exe
* 基础环境变量
    * PATH
    * LIBRARY_PATH
    * LD_LIBRARY_PATH
    * CPLUS_INCLUDE_PATH
`

#### 第三方库安装

* C++标准库，包括了STL容器，算法和函数等。
    C++ Standard Library：是一系列类和函数的集合，使用核心语言编写，也是C++ISO自身标准的一部分。
    Standard Template Library：标准模板库
    C POSIX library ： POSIX系统的C标准库规范
    ISO C++ Standards Committee ：C++标准委员会

* C++通用框架和库
    Apache C++ Standard Library：是一系列算法，容器，迭代器和其他基本组件的集合
    ASL ：Adobe源代码库提供了同行的评审和可移植的C++源代码库。
    Boost ：大量通用C++库的集合。
    BDE ：来自于彭博资讯实验室的开发环境。
    Cinder：提供专业品质创造性编码的开源开发社区。
    Cxxomfort：轻量级的，只包含头文件的库，将C++ 11的一些新特性移植到C++03中。
    Dlib：使用契约式编程和现代C++科技设计的通用的跨平台的C++库。
    EASTL ：EA-STL公共部分
    ffead-cpp ：企业应用程序开发框架
    Folly：由Facebook开发和使用的开源C++库
    JUCE ：包罗万象的C++类库，用于开发跨平台软件
    libPhenom：用于构建高性能和高度可扩展性系统的事件框架。
    LibSourcey ：用于实时的视频流和高性能网络应用程序的C++11 evented IO
    LibU ： C语言写的多平台工具库
    Loki ：C++库的设计，包括常见的设计模式和习语的实现。
    MiLi ：只含头文件的小型C++库
    openFrameworks ：开发C++工具包，用于创意性编码。
    Qt ：跨平台的应用程序和用户界面框架
    Reason ：跨平台的框架，使开发者能够更容易地使用Java，.Net和Python，同时也满足了他们对C++性能和优势的需求。
    ROOT ：具备所有功能的一系列面向对象的框架，能够非常高效地处理和分析大量的数据，为欧洲原子能研究机构所用。
    STLport：是STL具有代表性的版本
    STXXL：用于额外的大型数据集的标准模板库。
    Ultimate++ ：C++跨平台快速应用程序开发框架
    Windows Template Library：用于开发Windows应用程序和UI组件的C++库
    Yomm11 ：C++11的开放multi-methods.

#### 环境搭建
```
WORK_SPACE=/mnt/d/Code/see/src/
cd ${WORK_SPACE}
cd thirdparty
git clone https://gitee.com/zhangweiguo717/glog.git
git clone https://gitee.com/zhangweiguo717/googletest.git
git clone https://gitee.com/zhangweiguo717/gflags.git
sudo apt-get install libtool
sudo apt-get install autoconf
sudo apt-get install ninja
git config --global core.autocrlf false
```
