# luban
[中文文档](README_ZN.md)
## Why Need?

The feature-processing module is used in different scenarios, such as: model training and model inference. Usually, we use Python to train models, and feature-processing is also done in Python. However, in model inference, the shorter the inference run time, the better. So we usually use C++/java/golang or other languages to do model inference.

If feature-process module is written in many different languages, they may be inconsistencies. So, we decide to develop such a module in C++, which can be used by other different languages. In addition, we use a yaml configuration for feature processing, so that it will be easier to use.

## Supported libraries and tools

1. c lib: libluban
2. python lib: pyluban

## Configuration
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

This is the configuration of the expression, and is explained in the next section.

### Outputs

This is the final output of features, and then each feature has a group id, and the group id is continuously encoded from 0 and is not repeatable.

## Parse Expression

After installing the Python tool, the `luban_parser` tool is installed by default in /usr/local/bin. 
This tool is used to parse the above JSON configuration and generate the configuration using the toml format used by C++.
The expressions here are similar to Python's syntax. Originally, we wanted to use antlr to customize the DSL, but after thinking about it, we thought that it could be as simple as possible. As a result, it was finally decided to parse expressions using Python's built-in ast.

### Operators

1. opr: +, -, *, /, %, **
2. math function: round, floor, ceil, log, exp, log10, log2, sqrt, abs, sin, sinh, asin,asinh, cos, cosh, acos, acosh, tan, tanh, atan, atanh, sigmoid
3. aggravate function: min, max, variance, stddev, average, norm
4. time function: year, month, day, hour, minute, second, date, now, date_diff, date_add, date_sub, from_unixtime, unix_timestamp
5. string function: concat, substr, lower, upper, cross, reverse
6. topk function: topki, topkf, topks
7. other function: min_max, z_score, binarize, bucketize, box_cox, normalize


## Usage
1. Follow the installation prompts in the next section to compile and install the tool

2. Include header files and add libluban to dynamic link paths

3. Steps:
    1. step1: Configure the JSON file
    2. use `luban_parser` to process JSON configuration files and generate configuration files in TOML format
    3. Use the configuration file in TOML format as a configuration input for C/C++/Golang/Python


## Install

### install protobuf 
The installation script under CentOS 7 is below, other systems is similar.

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

### install pyluban
```shell
python setup.py install --install-scripts=/usr/local/bin
```

## Q&A

`Fatal Python error: type_traverse() called for non-heap type 'Entity'`
this is python's bug, please upgrade python to python3.8 or higher