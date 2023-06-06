# luban(鲁班)

## 为什么需要？
特征处理这个模块会存在在不同场景中，比如：模型训练，模型推理。通常情况下，我们会使用Python来做模型训练，特征的处理也会选址Python来做。但是模型推理的时候，我们对时延的要求是比较高的，通常会选择C++/java/golang等语言。
那么这里就存在一个问题，不同的语言开发特征处理模块，可能存在不一致的问题，同时工作量也变多了。所以，我们打算用C++来开发这么模块，然后可以给不同的语言用。
另外我们使用配置化的方式来进行特征处理，这样使用起来就会更简单些。

## 支持的库和工具
表达式进行特征处理的工具, 支持如下的工具:
1. c库: libluban
2. python库: pyluban

## 配置方式
```json
{
    "transforms": [
        "B = log(A)",
        "C = A * log(A)",
        "E = D * C",
        "G = date_add(F, H)",
        "K = concat(date_add(F, I), \"100\")"
    ],
    "outputs": [
        {
            "name": "A",
            "group": 0
        },
        {
            "name": "G",
            "group": 1
        }
    ]
}
```

### Transforms
这个是表达式的配置方式，具体的配置逻辑在下一节说明。

### Outputs
这个是最终输出的特征，然后每个特征进行编组，组号从0开始连续编码，不可重复。


## 表达式的解析
在安装Python工具后，默认情况下会在/usr/local/bin安装`luban_parser`这个工具。该工具是用来解析上述json配置，生成C++使用的toml格式的配置。
这里的表达式跟Python的语法类似。原本我们想用antlr来自定义DSL的，但是思考过后觉得能少一点以来就少一点。于是，最终决定用Python内置的ast来解析表达式。

### 支持的操作
1. 操作符: +, -, *, /, %, **
2. 数学函数: round, floor, ceil, log, exp, log10, log2, sqrt, abs, sin, sinh, asin,asinh, cos, cosh, acos, acosh, tan, tanh, atan, atanh, sigmoid
3. 聚合函数：min, max, variance, stddev, average, norm
4. 时间日期函数: year, month, day, hour, minute, second, date, now, date_diff, date_add, date_sub, from_unixtime, unix_timestamp
5. 字符串处理函数: concat, substr, lower, upper, cross, reverse
6. 不同类型的topk函数: topki, topkf, topks
7. 常见的函数: min_max, z_score, binarize, bucketize, box_cox, normalize 


## 使用方法
0. 根据下一节的安装提示，编译和安装好工具
1. C/C++/golang: include/luban.h引入头文件，添加libluban到动态链接路径
2. 步骤：
    1. step1: 配置json文件
    2. `luban_parser`处理json配置文件，生成toml格式的配置文件
    3. 将toml格式的配置文件作为C/C++/Golang/Python的配置输入


## Install

### protobuf安装
下面是centos7下的安装脚本, 其他系统类似处理
```shell
#!/bin/shell

yum install -y git wget
yum install -y openssl openssl-devel gcc-c++
yum install -y snappy snappy-devel autoconf automake libtool
yum install -y bzip2 bzip2-devel lz4-devel libzstd-devel
yum install -y epel-release gflags gflags-devel which 

# install cmake
# use cmake to compile this project for c/c++ lib
cd /tmp 
wget https://github.com/Kitware/CMake/releases/download/v3.18.2/cmake-3.18.2.tar.gz
tar -xvf cmake-3.18.2.tar.gz
cd cmake-3.18.2
./bootstrap
gmake && gmake install

# instal libunwind
cd /tmp
wget http://download.savannah.gnu.org/releases/libunwind/libunwind-1.5.0.tar.gz
tar -xvf libunwind-1.5.0.tar.gz
cd libunwind-1.5.0
CFLAGS=-fPIC ./configure
make CFLAGS=-fPIC && make CFLAGS=-fPIC install 

# install gperftools and tcmalloc
cd /tmp
wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.7/gperftools-2.7.tar.gz
tar -xvf gperftools-2.7.tar.gz 
cd gperftools-2.7
./configure
make -j6 && make install 

# install protobuf
cd /tmp
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf 
git checkout v3.8.0 && git submodule update --init --recursive
./autogen.sh
./configure 
make && make install
ldconfig

rm -rf /tmp/*
```

### pyluban安装
```shell
python setup.py install --install-scripts=/usr/local/bin
```