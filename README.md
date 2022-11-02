# luban(鲁班)

表达式进行特征处理的工具, 支持c库-libluban/python库-pylunban

## pyluban

基于swig封装的python库，python setup.py install即可
依赖protobuf库

## libtoolkit

生成了c封装的动态链接库

## 说明

### 支持的操作
```plaintext
+, -, *, /, %, **
floor, ceil, log, exp
min_max, z_score, binarize, bucketize, box_cox, normalize
topki, topkf, topks
concat, substr
timestamp, date, identity
```

### 使用方法
1. c: include/luban.h引入头文件
2. python: 安装pylunban库, 参考test/test.py
3. 步骤
1) step1: 配置json文件
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
2) json配置转化为toml, 生成toml格式配置
```bash
# step1
cd ./script
python3 configure_parser.py -i $input -o $output

```

3) 作为c/python的配置输入

### 添加自定义函数
1. 函数的定义如下
```c++
// 一元的map操作, ArgsType支持0~N个
template <typename T, typename U, typename... ArgsType>
T (*func)(U &, ArgsType &...)

// 一元的agg操作, ArgsType支持0~N个
template <typename T, typename U, typename... ArgsType>
std::vector<T> *(*func)(std::vector<U> &,ArgsType &...)

// 二元的map操作, ArgsType支持0~N个
template <typename T, typename U, typename W, typename... ArgsType>
T (*func)(U &, W &, ArgsType &...)

// 二元的agg操作, ArgsType支持0~N个
template <typename T, typename U, typename W, typename... ArgsType>
std::vector<T> *(*func)(std::vector<U> &, std::vector<W> &, ArgsType...)
```
2. 变量说明
1) 最多支持两个变量  
1.1 一个变量的情形: 必须放在第一个位置  
1.2 两个变量的情形: 必须放在第一二个位置
