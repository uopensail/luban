#!/usr/bin/python3
# -*- coding: UTF-8 -*-
#
# `LuBan` - 'c++ tool for transformating and hashing features'.
# Copyright (C) 2019 - present uopensail <timepi123@gmail.com>
#
# This file is part of `LuBan`.
#
# `LuBan` is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# `LuBan` is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with `LuBan`.  If not, see <http://www.gnu.org/licenses/>.
#
#

import os
import sys
import enum
import types
import inspect
from typing import Any, List, Union


class VariableType(enum.IntEnum):
    """
    变量的类型.

    VT_Int: 字面整型常量
    VT_Float: 字面浮点型常量
    VT_String: 字面字符串常量, 以引号作为字符串的开始和结束
    VT_IntList: 字面整型常量数组
    VT_FloatList: 字面浮点型常量数组
    VT_StringList: 字面字符串常量数组
    VT_Original_Feature: 原始特征, tffeature类型的值, TensorFlow中定义
    VT_Selected_Feature: 命名的变量, tffeature类型的值
    VT_Anonymous_Feature: 匿名变量, tffeature类型的值
    """

    VT_Not_Defined = 0
    VT_Int = 1
    VT_Float = 2
    VT_String = 3
    VT_IntList = 4
    VT_FloatList = 5
    VT_StringList = 6
    VT_Origin_Feature = 7
    VT_Selected_Feature = 8
    VT_Anonymous_Feature = 9

    def is_constant_scalar(self) -> bool:
        """是否是常量."""
        if self.value in (VariableType.VT_Int, VariableType.VT_Float,
                          VariableType.VT_String):
            return True
        return False

    def is_constant_array(self) -> bool:
        """是否是常量数组."""
        if self.value in (VariableType.VT_IntList, VariableType.VT_FloatList,
                          VariableType.VT_StringList):
            return True
        return False

    def is_constant(self) -> bool:
        """是否是常量——标量或向量."""
        return self.is_constant_scalar() or self.is_constant_array()

    def is_variable(self) -> bool:
        """是否是变量."""
        if self.value in (VariableType.VT_Anonymous_Feature,
                          VariableType.VT_Origin_Feature,
                          VariableType.VT_Selected_Feature):
            return True
        return False


class FunctionType(enum.IntEnum):
    """函数的类型."""

    FT_Not_Defined = 0
    FT_Simple_Func = 1
    FT_Unary_Mapper_Func = 2
    FT_Unary_Aggregate_Func = 3
    FT_Binary_Mapper_Func = 4
    FT_Binary_Aggregate_Func = 5


class Parameter:
    """
    参数定义.

    index: 函数调用时候的第几个参数, 下标从0开始计数
    type: 值的类型
    value: 对应的值, 如果是常量,value就是常量的值;
                    如果是原始变量, 就是对应变量的名字;
                    如果是生成型变量, 就是对应的算子的名字
    """

    def __init__(self, index: int, vtype: VariableType, value: Any):
        self.index, self.type, self.value = index, vtype, value


def python_type_to_variable_type(t: type) -> VariableType:
    """python type 转VariableType."""

    if t == float:
        return VariableType.VT_Float
    elif t == int:
        return VariableType.VT_Int
    elif t == str:
        return VariableType.VT_String
    elif t == List[float]:
        return VariableType.VT_FloatList
    elif t == List[int]:
        return VariableType.VT_IntList
    elif t == List[str]:
        return VariableType.VT_StringList
    raise TypeError(f"not support type:{t}")


class FunctionCenter:
    FunctionDefintionDict = {}

    @staticmethod
    def load_function_definitions(path: str):
        """加载函数定义."""
        dir_path, base_name = os.path.dirname(path), os.path.basename(path)
        assert (base_name.endswith('.py'))
        name = base_name[:-3]
        sys.path.append(dir_path)
        exec(f"import {name}")

        function_def_module = sys.__dict__['modules'][name]
        for k, func_value in function_def_module.__dict__.items():
            if isinstance(func_value, types.FunctionType):
                assert (func_value.__name__ ==
                        k and "return" in func_value.__annotations__)

                tmp = {"return": python_type_to_variable_type(
                    func_value.__annotations__["return"]), "argvs": []}

                sig = inspect.signature(func_value)
                if len(sig.parameters) != len(func_value.__annotations__)-1:
                    raise RuntimeError(
                        f'params must be annotated: {func_value.__annotations__}')

                for name in sig.parameters:
                    tmp['argvs'].append(
                        python_type_to_variable_type(func_value.__annotations__[name]))
                FunctionCenter.FunctionDefintionDict[k] = tmp
