# luban(鲁班)

## 为什么需要？

特征处理这个模块会存在在不同场景中，比如：模型训练，模型推理。通常情况下，我们会使用Python来做模型训练，特征的处理也会选址Python来做。但是模型推理的时候，我们对时延的要求是比较高的，通常会选择C++/java/golang等语言。那么这里就存在一个问题，不同的语言开发特征处理模块，可能存在不一致的问题，同时工作量也变多了。所以，我们打算用C++来开发这么模块，然后可以给不同的语言用。另外我们使用配置化的方式来进行特征处理，这样使用起来就会更简单些。

## 支持的库和工具

表达式进行特征处理的工具, 支持如下的工具:

1. c库: libluban
2. python库: luban

## 配置方式

```json
{
    "features": [
        {
            "name": "A",
            "expr": "(B+C)*F",
            "type": 0,
            "hash": false,
            "padding": 0,
            "dim": 1
        },
        {
            "name": "B",
            "type": 0,
            "padding": 0,
            "dim": 1
        },
        {
            "name": "C",
            "type": 0,
            "padding": 0,
            "dim": 1
        },
        {
            "name": "D",
            "expr": "concat(\"prefix-\", E)",
            "type": 0,
            "hash": true,
            "padding": 0,
            "dim": 3
        }
    ],
    "groups": [
        [
            "A",
            "B"
        ],
        [
            "D"
        ]
    ]
}
```

### features

配置每一个特征的相关属性。

| 属性      | 类型     | 是否为空 | 默认值   | 说明                                                                  |
| ------- | ------ | ---- | ----- | ------------------------------------------------------------------- |
| name    | 字符串    | 否    | -     | 特征的名字                                                               |
| expr    | 字符串    | 是    | 空字符串  | 处理的表达式，没有就不处理(即外部输入特征)                                              |
| type    | 整型     | 否    | -     | 0: 整型,<br/>1: 浮点型,<br/>2: 字符串<br/>3: 整型数组<br/>4: 浮点型数组<br/>5: 字符串数组 |
| hash    | 布尔类型   | 是    | false | 是否经过hash处理                                                          |
| padding | 整型或浮点型 | 否    | 0/0.0 | 填充值                                                                 |
| dim     | 整型     | 否    | 1     | 数据的维度                                                               |

### groups

经过处理后，不同的特征可能会放在不同的组。因为下游使用者是pytorch,不同类型的数据是不便放在一个tensor里面的。特征的最后的输出顺序是按照group里面配置的顺序进行输出的。





## 表达式的解析

在安装Python工具后，默认情况下会在/usr/local/bin安装`luban_parser`这个工具。该工具是用来解析上述json配置，生成C++使用的json格式的配置。
这里的表达式跟Python的语法类似。原本我们想用antlr来自定义DSL的，但是思考过后觉得能少一点以来就少一点。于是，最终决定用Python内置的ast来解析表达式。

### 支持的操作

1. 操作符: +, -, *, /, %, **
2. 数学函数: round, floor, ceil, log, exp, log10, log2, sqrt, abs, sin, sinh, asin,asinh, cos, cosh, acos, acosh, tan, tanh, atan, atanh, sigmoid
3. 聚合函数：min, max, variance, stddev, average, norm
4. 时间日期函数: year, month, day, hour, minute, second, date, now, date_diff, date_add, date_sub, from_unixtime, unix_timestamp
5. 字符串处理函数: concat, substr, lower, upper, cross, reverse
6. 不同类型的topk函数: topki, topkf, topks
7. 常见的函数: min_max, z_score, binarize, bucketize, box_cox, normalize 

 

```c
// 下面是支持的操作列表
// 如果是模板类型，则可以在配置函数名的时候加入类型特征
// e.g.: _add<int64_t>, _add<float>
// 如果不添加类型信息，则默认是float类型
// 在实际执行的时候，工具会进行一些内置隐式数据类型转换，

T _add(T &a, T &b);
T _sub(T &a, T &b);
T _mul(T &a, T &b);
T _div(T &a, T &b);
int64_t _mod(int64_t &a, int64_t &b);
float _pow(float &a, float &b);
int64_t _round(float &x);
int64_t _floor(float &x);
int64_t _ceil(float &x);
float _log(float &x);
float _exp(float &x);
float _log10(float &x);
float _log2(float &x);
float _sqrt(float &x);
float _abs(float &x);
float _sin(float &x);
float _asin(float &x);
float _sinh(float &x);
float _asinh(float &x);
float _cos(float &x);
float _acos(float &x);
float _cosh(float &x);
float _acosh(float &x);
float _tan(float &x);
float _atan(float &x);
float _tanh(float &x);
float _atanh(float &x);
float _sigmoid(float &x);
T min(std::vector<float> &src);
T max(std::vector<T> &src);
float average(std::vector<T> &src);
float variance(std::vector<T> &src);
float stddev(std::vector<T> &src);
float norm(std::vector<T> &src, float &n);
std::string year();
std::string month();
std::string day();
std::string date();
std::string hour();
std::string minute();
std::string second();
int64_t now();
std::string from_unixtime(int64_t &ts, std::string &format);
int64_t unix_timestamp(std::string &t, std::string &format);
std::string date_add(std::string &s, int64_t &n);
std::string date_sub(std::string &s, int64_t &n);
int64_t date_diff(std::string &d1, std::string &d2);
std::string reverse(std::string &s);
std::string upper(std::string &s);
std::string lower(std::string &s);
std::string substr(std::string &s, int64_t &start, int64_t &len);
std::string concat(std::string &a, std::string &b);
float min_max(T &v, T &min, T &max);
float z_score(float &v, float &mean, float &stdv);
int64_t binarize(T &v, T &threshold);
int64_t bucketize(T &v, std::vector<T> &boundaries);
float box_cox(float &v, float &lambda);
std::vector<float> normalize(std::vector<T> &src, float &norm);
std::vector<T> topk(std::vector<T> &src, int64_t &k);
std::vector<std::string> cross(std::vector<std::string> &srcA,
                               std::vector<std::string> &srcB);

```





## 使用方法

0. 根据下一节的安装提示，编译和安装好工具
1. C/C++/golang: include/luban.h引入头文件，添加libluban到动态链接路径
2. 步骤：
   1. step1: 配置json文件
   2. `luban_parser`处理json配置文件，生成新的json格式的配置文件
   3. 将json格式的配置文件作为C/C++/Golang/Python的配置输入

## Install

### install On MacOS ARM

```shell
pip install pyluban
```

### Unix Like

```shell
python setup.py install --install-scripts=/usr/local/bin

pip install pyluban
```



## Examples

### Features

特征的结构如下:

```json
// json形式
[
    {"type": 0,                // 同上述的类型列表
    "name":"int_val",          // 特征的名字，不可重复
    "value": 10                // 具体的值
    }, 
    {"type": 1,              
    "name":"float_val",      
    "value": 10.3            
    }, 
    {"type": 2,              
    "name":"str_val",      
    "value": "10.3"          
    }, 
    {"type": 3,              
    "name":"int_list",      
    "value": [1,2,3,4]    
    }, 
    {"type": 4,              
    "name":"float_list",      
    "value": [1.1,2.2,3.3,4.4]    
    }, 
    {"type": 5,              
    "name":"str_list",      
    "value": ["1","2","3","4"]    
    }, 
]
```

python代码中如何解析特征

```python
import json
import pyluban
feas = [{"type": 0, "name":"int_val",          
    "value": 10                // 具体的值
    }, ] 

feas_str = json.dumps(feas)

#支持一个字符串
features = pyluban.Features(feas_str)

# 支持多个字符串
features = pyluban.Features([feas_str1, feas_str2, feas_str3])

# 不输入就生成空
features = pyluban.Features()
```

## Function

这个是函数处理的配置，通常情况下，使用者不需要使用到，在做单元测试的时候会使用到，这里也加以说明。

```json
{
    "func": func,
    "name": name,
    "flag": flag,
    "args": args,
    "vars": vars
}
```

| 属性   | 类型    | 说明                                         |
| ---- | ----- | ------------------------------------------ |
| func | 字符串   | 函数处理的名名字                                   |
| name | 字符串   | 生成的新特征的名字                                  |
| flag | 整型    | 每一个参数的标志位：在flag的二进制位中，第i位为1则表示该输入是变量；否则为常量 |
| args | 字典列表  | 同features结构,可以不填name字段                     |
| vars | 字符串列表 | 变量的名字列表                                    |