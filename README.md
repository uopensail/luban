# luban

## Why Need?

The feature-processing module is used in different scenarios, such as: model training and model inference. Usually, we use Python to train models, and feature-processing is also done in Python. However, in model inference, the shorter the inference run time, the better. So we usually use C++/java/golang or other languages to do model inference. If feature-process module is written in many different languages, they may be inconsistencies. So, we decide to develop such a module in C++, which can be used by other different languages. In addition, we use a json configuration for feature processing, so that it will be easier to use.



## Supported libraries and tools

1. c/c++ lib: libluban
2. python lib: pyluban



## Configuration

```json
{
    "user_features": [
        {
            "name": "U1",
            "expr": "A*B",
            "type": 1,
            "hash": false,
            "padding": 0,
            "dim": 1
        },
        {
            "name": "U2",
            "expr": "C[int64]+D[int64]",
            "type": 0,
            "hash": false,
            "padding": 0,
            "dim": 1
        }
    ],
    "item_features": [
        {
            "name": "I1",
            "expr": "F[int64]+G[int64]",
            "type": 0,
            "hash": false,
            "padding": 0,
            "dim": 1
        },
        {
            "name": "I2",
            "expr": "concat(H[string],\"TEST\")",
            "type": 0,
            "hash": true,
            "padding": 0,
            "dim": 1
        }
    ],
    "groups": [
        [
            "U1"
        ],
        [
            "U2"
        ],
        [
            "I1",
            "I2"
        ]
    ]
}
```

### features

Configure the relevant properties for each feature.

| Properties | Type          | Empty | Default | Description                                                                                            |
| ---------- | ------------- | ----- | ------- | ------------------------------------------------------------------------------------------------------ |
| name       | string        | N     | -       | name of feature                                                                                        |
| expr       | string        | Y     | ""      | how to process feature                                                                                 |
| type       | int           | N     | -       | 0: int64,<br/>1: float3,<br/>2: string<br/>3: vector\<int64\><br/>4: vector\<float\><br/>5: vector\<string\> |
| hash       | bool          | Y     | false   | hash or not                                                                                            |
| padding    | int64/float32 | Y     | 0/0.0   | fill value                                                                                             |
| dim        | int           | Y     | 1       | dim of result                                                                                          |

### groups

After processing, different features may be placed in different groups. Because the downstream user is pytorch, different types of data are inconvenient to put in a tensor. The final output order of the features is the same as the order configured in the group. **All features configured in groups need to be configured in the features above**.



## Parse Expression

After installing the Python tool, the `luban_parser` tool is installed by default in /usr/local/bin. This tool is used to parse the above JSON configuration and generate the configuration using the JSON format used by C++. The expressions here are similar to Python's syntax. Originally, we wanted to use antlr to customize the DSL, but after thinking about it, we thought that it could be as simple as possible. As a result, it was finally decided to parse expressions using Python's built-in ast.



### Operators

1. opr: +, -, *, /, %, **
2. math function: round, floor, ceil, log, exp, log10, log2, sqrt, abs, sin, sinh, asin,asinh, cos, cosh, acos, acosh, tan, tanh, atan, atanh, sigmoid
3. aggravate function: min, max, variance, stddev, average, norm
4. time function: year, month, day, hour, minute, second, date, now, date_diff, date_add, date_sub, from_unixtime, unix_timestamp
5. string function: concat, substr, lower, upper, cross, reverse
6. topk function: topki, topkf, topks
7. other function: min_max, z_score, binarize, bucketize, box_cox, normalize



```c
// The following is a list of supported operations
// If it is a template type, you can add the type feature when configuring the function name
// e.g.: _add<int64_t>, _add<float>
// If you do not add type information, the float type is defaulted
// In practice, the tool does some built-in implicit data type conversions

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

## Usage

1. Follow the installation prompts in the next section to compile and install the tool

2. Include header files and add libluban to dynamic link paths

3. Steps:
   
   1. step1: Configure the JSON file
   2. use `luban_parser` to process JSON configuration files and generate configuration files in JSON format
   3. Use the configuration file in JSON format as a configuration input for C/C++/Golang/Python



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

### Features & FeaturesList

The structure of the features is as follows:

```json
// json format
{
    "A": {                        // key is the name of feature
        "type": 0,                // type listed above
        "value": 10               // certain value
    },
    "B": {
        "type": 1,                
        "value": 10.9                
    },
    ...
}
```

how to parse features in python

```python
import json
import pyluban
feas = {"A": {"type": 0,"value": 10},...}

feas_str = json.dumps(feas)

# arg is str
features = pyluban.Features(feas_str)

# arg is str list
features = pyluban.Features([feas_str1, feas_str2, feas_str3])

# arg is nil
features = pyluban.Features()

# create a featureslist
l = pyluban.FeatuersList()

# add a value
l.append(features)

print(l[0])
print(l)

l[0] = features
```

## Function

This is the configuration of function processing, usually, **users generally do not use**, will be used when doing unit tests, and are also explained here.

```json
{
    "func":"_add<int64_t>",
    "name":"X",
    "flag":3,
    "args": [],
    "vars": ["A","B"]
}
```

| Properties | Type         | Description                                                                                                                                   |
| ---------- | ------------ | --------------------------------------------------------------------------------------------------------------------------------------------- |
| func       | string       | function name                                                                                                                                 |
| name       | string       | name of generated feature                                                                                                                     |
| flag       | int64        | Flag bits for each parameter: In the binary bit of flag, the ith bit is 1 to indicate that the input is a variable; Otherwise, it is constant |
| args       | list of dict | same structure of features                                                                                                                    |
| vars       | list of str  | variable name list                                                                                                                            |

## Toolkit

By configuring the class for feature processing entry, the consumer mainly uses this class to process features.

```python
import pyluban
import luban_parser
# this is the original config file
"""
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
"""

luban_parser.parser(input_file: str, output_file: str)

# this is the output file of luban_parser

"""
{
    "transforms": [
        {
            "func": "_add<int64_t>",
            "name": "anonymous_0",
            "flag": 3,
            "args": [],
            "vars": [
                "B",
                "C"
            ]
        },
        {
            "func": "_mul<int64_t>",
            "name": "A",
            "flag": 3,
            "args": [],
            "vars": [
                "anonymous_0",
                "F"
            ]
        },
        {
            "func": "concat",
            "name": "D",
            "flag": 2,
            "args": [
                {
                    "type": 2,
                    "value": "prefix-"
                }
            ],
            "vars": [
                "E"
            ]
        }
    ],
    "groups": [
        {
            "id": 0,
            "width": 2,
            "type": 0
        },
        {
            "id": 1,
            "width": 3,
            "type": 0
        }
    ],
    "features": [
        {
            "name": "A",
            "type": 0,
            "padding": 0,
            "group": 0,
            "offset": 0,
            "hash": false,
            "dim": 1
        },
        {
            "name": "B",
            "type": 0,
            "padding": 0,
            "group": 0,
            "offset": 1,
            "hash": false,
            "dim": 1
        },
        {
            "name": "D",
            "type": 0,
            "padding": 0,
            "group": 1,
            "offset": 0,
            "hash": true,
            "dim": 3
        }
    ]
}
"""
# config.json is the file after configuration processing, 
# and the above is an example of the file content of the configuration
toolkit = pyluban.Toolkit("config.json")
```

#### Rows

In the case of only one feature, Rows is a list object with a length of the number of groups configured above, which can be accessed via subscripts and then converted into numpy structures.

```python
# r pyluban:Rows type
r = toolkit.process(features: pyluban.Features)


for i in range(len(r)):
    print(np.asarray(r[i]))
```

#### Matrices

Matrices is a list object with a length of the number of groups configured above, which can be accessed by subscripts and then converted into a numpy structure.

```python
l = pyluban.FeaturesList()
l.append(features: pyluban.Features)

m = self.toolkit.process(l)

for i in range(len(m)):
    print(np.asarray(m[i]))
```