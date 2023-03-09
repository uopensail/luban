# luban(鲁班)
表达式进行特征处理的工具, 支持如下的工具:
1. c库: libluban
2. python库: pylunban

## 说明

### 表达式的解析
`luban_parser`是安装在/usr/local/bin下的, 主要是为了解析表达式, 生成c++可读的配置。
我们是利用python中的ast库来解析表达式的, 即表达式要遵循python的语法。

### 支持的操作
1. 常见的操作符: +, -, *, /, %, **
2. 常见的数学函数: floor, ceil, log, exp
3. 常见的函数: min_max, z_score, binarize, bucketize, box_cox, normalize
4. 不同类型的topk函数: topki, topkf, topks
5. 字符串处理函数: concat, substr
6. 内置的一些函数: timestamp, date, identity

### 使用方法
1. c: include/luban.h引入头文件
2. python: 安装pylunban库, 参考test/test.py
3. 步骤
1. step1: 配置json文件
expr: 表达式
gid: 特征组编号
```josn
{
    "features":[
        
        {
            "expr":"d_c_date= topks(concat(d_s_countryCode, concat(\"-\",date())), 2)",
            "gid":1
        }
    ]
}
```
2. 作为c/python的配置输入


## Install

### protobuf安装
在centos7下的安装脚本, 其他系统类似处理
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