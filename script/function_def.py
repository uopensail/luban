#!/usr/bin/python3
# -*- coding: UTF-8 -*-
"""
author: random
descripton: 利用Python将配置文件进行解析,给C++代码使用.

定义配置的文件的示例:

`feature_1 = feature_0 + 100`

具体的说明如下:
1. 语句的解析是按照Python的语法来解析的
2. 必须是赋值语句, 左边作为特征的名字, 右边是计算表达式
3. 表达式支持嵌套调用, 函数名称
4. 支出Python内置的一些操作符
    +,-,*,/,%,**

"""

from typing import List


def _add(a: float, b: float) -> float:
    pass


def _sub(a: float, b: float) -> float:
    pass


def _mul(a: float, b: float) -> float:
    pass


def _div(a: float, b: float) -> float:
    pass


def _mod(a: int, b: int) -> int:
    pass


def _pow(a: int, b: int) -> float:
    pass


def floor(v: float) -> float:
    pass


def ceil(v: float) -> float:
    pass


def log(v: float) -> float:
    pass


def exp(v: float) -> float:
    pass


def timestamp() -> int:
    pass


def date() -> str:
    pass


def min_max(v: float, min: float, max: float) -> float:
    pass


def z_score(v: float, mean: float, std: float) -> float:
    pass


def binarize(v: float, threshold: float) -> float:
    pass


def box_cox(a: float, b: float) -> float:
    pass


def bucketize(a: float, b: List[float]) -> int:
    pass


def substr(v: str, pos: int, len: int) -> str:
    pass


def concat(a: str, b: str) -> str:
    pass


def c_concat(a: List[str], b: List[str]) -> List[str]:
    pass


def normalize(a: List[float], n: int) -> List[float]:
    pass


def topk(a: List[str], k: int) -> List[str]:
    pass
