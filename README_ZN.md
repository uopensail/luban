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
            "slot": 0,
            "length": 2,
        },
        {
            "name": "G",
            "slot": 1
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

### install On MacOS ARM

```shell
brew install protobuf
pip install pyluban
```

### Unix Like

```shell
pip install pyluban
```

## Q&A

1. 出现这个问题：`Fatal Python error: type_traverse() called for non-heap type 'Entity'`
这个是[python内部的bug](https://bugs.python.org/issue28709)，在后续的python3.8及以上版本修复了。